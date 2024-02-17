#include "ship_weapon.h"
#include "hp_resistances.h"
#include "eve_math.h"

#include <memory>
#include <assert.h>

namespace eve {

using std::shared_ptr;
using std::make_unique;

// *************************************************************************
// -- Ammo Implementation for Weapons
// *************************************************************************

MissileAmmo::MissileAmmo(float velocity, float flight_time, 
                         const DamageProfile* dmg_profile)
    : velocity_(velocity), 
      flight_time_(flight_time), 
      Ammo(dmg_profile) {}

shared_ptr<Ammo> MissileAmmo::Copy() const {
  return make_unique<MissileAmmo>(velocity_, flight_time_, &dmg_profile_);
}

TurretAmmo::TurretAmmo(float falloff_modifier, float optimal_modifier,
                       float tracking_modifier, 
                       const DamageProfile* dmg_profile)
    : falloff_modifier_(falloff_modifier),
      optimal_modifier_(optimal_modifier),
      tracking_modifier_(tracking_modifier),
      Ammo(dmg_profile) {}

shared_ptr<Ammo> TurretAmmo::Copy() const {
  return make_unique<TurretAmmo>(falloff_modifier_, optimal_modifier_,
                                 tracking_modifier_, &dmg_profile_);
}

// End of Ammo Implementation for Weapons


// *************************************************************************
// -- Weapon Base Implementation
// *************************************************************************

Weapon::Weapon(float rof, float reload_time, float weapon_amount, 
               const DamageProfile* dmg_profile)
    : rof_(rof),
      reload_time_(reload_time), 
      weapon_amount_(weapon_amount),
      base_dmg_profile_(*dmg_profile),
      curr_dmg_profile_(*dmg_profile) {}

// End of Weapon Base Implementation

// *************************************************************************
// -- Missile Weapons Implementation
// *************************************************************************

MissileWeapon::MissileWeapon(float rof, float reload_time, 
                             float weapon_amount,
                             const DamageProfile* dmg_profile)
    : Weapon(rof, reload_time, weapon_amount, dmg_profile),
      ammo_(nullptr) {}

float MissileWeapon::Dps(const ResistanceProfile* res) const {
  const DamageProfile* dmg_profile = DmgProfile();
  float dps = ((DecreaseByPercent(dmg_profile->Em(), res->Em()) +
               DecreaseByPercent(dmg_profile->Thermal(), res->Thermal()) +
               DecreaseByPercent(dmg_profile->Kinetic(), res->Kinetic()) + 
               DecreaseByPercent(dmg_profile->Explosive(), res->Explosive())) *
               weapon_amount_) / rof_;
  return dps;
}

void MissileWeapon::LoadAmmo(const shared_ptr<Ammo>& ammo) {
  shared_ptr<MissileAmmo> missile_ammo = 
    std::dynamic_pointer_cast<MissileAmmo>(ammo);

  assert(missile_ammo);

  ammo_ = missile_ammo;
  ApplyAmmoBonuses();
}

void MissileWeapon::UnloadAmmo() {
  SetRange(0);
  SetDmgProfile(BaseDmgProfile());
}

void MissileWeapon::ApplyAmmoBonuses() {
  SetRange(ammo_->Velocity() * ammo_->FlightTime());
  SetDmgProfile(ammo_->DmgProfile());
  // SetApplication() TODO: Maybe we can create a function
  // which will calculate a application to NPC's with different
  // hull parameters like signature radius or current ship velocity.
}

shared_ptr<Weapon> MissileWeapon::Copy() const {
  shared_ptr<Weapon> m = make_unique<MissileWeapon>(
      rof_, reload_time_, weapon_amount_, &base_dmg_profile_);
      
  if (ammo_) {
    m->LoadAmmo(ammo_->Copy());
  }

  return m;
}

// End of Missile Weapons Implementation

// *************************************************************************
// -- Turret Weapons Implementation
// *************************************************************************

TurretWeapon::TurretWeapon(float rof, float reload_time, float weapon_amount,
                           float dmg_multiplier, float optimal,
                           float falloff, float tracking,
                           const DamageProfile* dmg_profile)
    : Weapon(rof, reload_time, weapon_amount, dmg_profile),
      dmg_multiplier_(dmg_multiplier),
      optimal_(optimal),
      falloff_(falloff),
      tracking_(tracking),
      ammo_(nullptr) {}

float TurretWeapon::Dps(const ResistanceProfile* res) const {
  const DamageProfile* dmg_profile = DmgProfile();
  float dps = (((DecreaseByPercent(dmg_profile->Em(), res->Em()) +
               DecreaseByPercent(dmg_profile->Thermal(), res->Thermal()) +
               DecreaseByPercent(dmg_profile->Kinetic(), res->Kinetic()) + 
               DecreaseByPercent(dmg_profile->Explosive(), res->Explosive())) *
               weapon_amount_) / rof_) * dmg_multiplier_;
  return dps;
}

void TurretWeapon::LoadAmmo(const shared_ptr<Ammo>& ammo) {
  // Try to dynamicly cast Ammo -> TurretAmmo
  shared_ptr<TurretAmmo> turret_ammo = 
    std::dynamic_pointer_cast<TurretAmmo>(ammo);
  
  // Check if cast succseed
  assert(turret_ammo);

  ammo_ = turret_ammo;
  ApplyAmmoBonuses();
}

void TurretWeapon::UnloadAmmo() { 
  SetRange(falloff_ + optimal_);
  SetApplication(tracking_);
  SetDmgProfile(BaseDmgProfile());
}

void TurretWeapon::ApplyAmmoBonuses() {
  // The modifier values have to be wirtten in range 1 - 0
  // E.g OptimalModifier = 0.75 - this value means that
  // our ammo decreasing turret optimal for 25%.
  float optimal = optimal_ * ammo_.get()->OptimalModifier();
  float falloff = falloff_ * ammo_.get()->FalloffModifier();
  float tracking = tracking_ * ammo_.get()->TrackingModifier();

  // Sets the Application that was modfied with TrackingModifier()
  // from Ammo.
  SetApplication(tracking);

  // These values should looks like ((26.0 Optimal) + (3.0 Falloff))
  // The falloff value should represent only falloff range
  // without optimal included.
  SetRange(optimal + falloff);

  // Simply sets DmgProfile from ammo_ to TurretWeapon.
  SetDmgProfile(ammo_.get()->DmgProfile());
}

shared_ptr<Weapon> TurretWeapon::Copy() const {
  shared_ptr<TurretWeapon> w = 
      make_unique<TurretWeapon>(rof_, reload_time_, weapon_amount_,
                                dmg_multiplier_, optimal_, falloff_, tracking_,
                                &base_dmg_profile_);

  w->ammo_ = std::static_pointer_cast<TurretAmmo>(ammo_);

  return w;
}

WeaponContainer::WeaponContainer(vector<shared_ptr<Weapon>>& weapons) {
  Init(weapons);
}

vector<shared_ptr<Weapon>>* WeaponContainer::GetWeaponsByType(
    Weapon::Type weapon_type) 
{
  auto weapons_it = weapons_map_.find(weapon_type);

  if (weapons_it == weapons_map_.end()) {
    return nullptr;
  }

  return &weapons_it->second;
}

void WeaponContainer::Init(const vector<shared_ptr<Weapon>>& weapons) {
  if (weapons.empty())
    return;

  int i = 0;
  while (i < static_cast<int>(Weapon::Type::Total)) {
    Weapon::Type type = static_cast<Weapon::Type>(i);

    vector<shared_ptr<Weapon>> specific_weapons;

    for (const auto& weapon : weapons) {
      if (weapon->GetType() == type) {
        specific_weapons.emplace_back(weapon);
      }
    }

    if (!specific_weapons.empty()) {
      weapons_map_.emplace(type, specific_weapons);
    }
    ++i;
  }
}

WeaponContainer WeaponContainer::Copy() const {
  vector<shared_ptr<Weapon>> c;
  for (auto& wpn_type : weapons_map_) {
    for (int i = 0; i < wpn_type.second.size(); i++) {
      c.push_back(wpn_type.second[i]->Copy());
    }
  }
  return WeaponContainer(c);
}

// End of Turret Weapons Implementation

} // namespace eve
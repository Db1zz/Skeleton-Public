#include "ship_weapon.h"
#include "hp_resistances.h"
#include "eve_math.h"

#include <assert.h>

namespace eve {

// *************************************************************************
// -- Ammo Implementation for Weapons
// *************************************************************************

MissileAmmo::MissileAmmo(float velocity, float flight_time, 
                         const DamageProfile* dmg_profile)
    : velocity_(velocity), 
      flight_time_(flight_time), 
      Ammo(dmg_profile) {}

TurretAmmo::TurretAmmo(float falloff_modifier, float optimal_modifier,
                       float tracking_modifier, 
                       const DamageProfile* dmg_profile)
    : falloff_modifier_(falloff_modifier),
      optimal_modifier_(optimal_modifier),
      tracking_modifier_(tracking_modifier),
      Ammo(dmg_profile) {}

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

void MissileWeapon::LoadAmmo(const shared_ptr<Ammo> ammo) {
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

// End of Missile Weapons Implementation

// *************************************************************************
// -- Turret Weapons Implementation
// *************************************************************************

TurretWeapon::TurretWeapon(float rof, float reload_time, float weapon_amount,
                           float dmg_multiplier, float base_optimal,
                           float base_falloff, float base_tracking,
                           const DamageProfile* dmg_profile)
    : Weapon(rof, reload_time, weapon_amount, dmg_profile),
      dmg_multiplier_(dmg_multiplier),
      base_optimal_(base_optimal),
      base_falloff_(base_falloff),
      base_tracking_(base_tracking),
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

void TurretWeapon::LoadAmmo(const shared_ptr<Ammo> ammo) {
  // Try to cast Ammo -> TurretAmmo
  shared_ptr<TurretAmmo> turret_ammo = 
    std::dynamic_pointer_cast<TurretAmmo>(ammo);
  
  // Check if cast succseed
  assert(turret_ammo);

  ammo_ = turret_ammo;
  ApplyAmmoBonuses();
}

void TurretWeapon::UnloadAmmo() { 
  SetRange(base_falloff_ + base_optimal_);
  SetApplication(base_tracking_);
  SetDmgProfile(BaseDmgProfile());
}

void TurretWeapon::ApplyAmmoBonuses() {
  // The modifier values have to be wirtten in range 1 - 0
  // E.g OptimalModifier = 0.75 - this value means that
  // our ammo decreasing turret optimal for 25%.
  float optimal = base_optimal_ * ammo_.get()->OptimalModifier();
  float falloff = base_falloff_ * ammo_.get()->FalloffModifier();
  float tracking = base_tracking_ * ammo_.get()->TrackingModifier();

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

// End of Turret Weapons Implementation

} // namespace eve
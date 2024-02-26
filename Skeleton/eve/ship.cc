#include "ship.h"
#include "../abyss_bot/bot.h"
#include "effects.h"
#include "eve_math.h"
#include <iostream>
#include <utility>

namespace eve {

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::vector;

ShipDefense::ShipDefense(const ShipResistances& ship_res, float armor_hp,
                         float shield_hp, float hull_hp, float hps)
    : ship_res_(ship_res),
      armor_hp_(armor_hp),
      shield_hp_(shield_hp),
      hull_hp_(hull_hp),
      hps_(hps) {}

// ShipDefense::ShipDefense(const ShipDefense& d)
//     : ShipDefense(d.ship_res_, d.armor_hp_, d.shield_hp_, 
//                   d.hull_hp_, d.hps_) {}

void ShipDefense::ApplyDps(std::pair<string, float> dps) {
  hps_ -= dps.second;
  applied_dps_.push_back(dps);
}

bool ShipDefense::RemoveDps(std::pair<string, float> dps) {
  for (int i = 0; i < applied_dps_.size(); i++) {
    if (applied_dps_[i].first == dps.first &&
        applied_dps_[i].second == dps.second)
    {
      hps_ += dps.second;
      applied_dps_.erase(applied_dps_.begin() + i);
      return true;
    }
  }
  return false;
}

float ShipDefense::Ehp(const WeaponContainer& weapons) const {
  int weapon_amount = weapons.TotalWeapons();
  int ehp_sum = 0;

  for (const auto& specifc_weapon_vector : weapons) {
    for (const auto& weapon : specifc_weapon_vector.second) {
      ehp_sum += Ehp(weapon->DmgProfile());
    }
  }

  // The expression (ehp_sum / weapon_amount) calculates average ehp.
  return ehp_sum / weapon_amount;
}

float ShipDefense::Ehp(const DamageProfile* dmg_prof) const {
  float total_ehp = 0;
  int curr_raw_hp_index = 0;
  vector<float> raw_hp{shield_hp_, armor_hp_, hull_hp_};

  // Note: П О Ч Е М У
  const ShipResistances& res = ship_res_;

  for (const auto profile : res) {
    int dmg_type_amount = 0;
    float res_multiplier = 0;

    for (int i = 0; i < profile.Size(); i++) {
      if ((*dmg_prof)[i] > 0) {
        dmg_type_amount += 1;
        res_multiplier += profile[i];
      }
    }

    if (res_multiplier > 0) {
      res_multiplier = 1 / ((1 - res_multiplier) / dmg_type_amount);
      total_ehp += raw_hp[curr_raw_hp_index] * res_multiplier;
    }
    ++curr_raw_hp_index;
  }

  return total_ehp;
}

unique_ptr<ShipDefense> ShipDefense::Copy() const {
  return make_unique<ShipDefense>(*this);
}

ShipTargeting::ShipTargeting(float range)
    : base_range_(range), range_(range) {}

ShipTargeting::ShipTargeting(const ShipTargeting& t)
    : ShipTargeting(t.range_) {}

unique_ptr<ShipTargeting> ShipTargeting::Copy() const {
  return make_unique<ShipTargeting>(*this);
}

ShipCapacitor::ShipCapacitor(float recharge_rate, float amount)
    : base_recharge_rate_(recharge_rate),
      base_amount_(amount),
      recharge_rate_(recharge_rate), 
      amount_(amount) {}

ShipCapacitor::ShipCapacitor(const ShipCapacitor& c)
    : ShipCapacitor(c.recharge_rate_, c.amount_) {}

unique_ptr<ShipCapacitor> ShipCapacitor::Copy() const {
  return make_unique<ShipCapacitor>(*this);
}

ShipEngine::ShipEngine(float velocity)
    : velocity_(velocity), base_velocity_(velocity) {}

ShipEngine::ShipEngine(const ShipEngine& e)
    : ShipEngine(e.velocity_) {}

unique_ptr<ShipEngine> ShipEngine::Copy() const {
  return make_unique<ShipEngine>(*this);
}

ShipHull::ShipHull(float signature_radius, float warp_speed, float align_time)
    : signature_radius_(signature_radius),
      warp_speed_(warp_speed),
      align_time_(align_time) {}

ShipHull::ShipHull(const ShipHull& c)
    : ShipHull(c.SignatureRadius(), c.WarpSpeed(), c.AlignTime()) {}

unique_ptr<ShipHull> ShipHull::Copy() const {
  return make_unique<ShipHull>(*this);
}

Ship::Ship(unique_ptr<ShipEngine>& engine,
           unique_ptr<ShipCapacitor>& capacitor,
           unique_ptr<ShipTargeting>& targeting,
           unique_ptr<ShipDefense>& defense,
           unique_ptr<ShipHull>& hull,
           vector<shared_ptr<EwarModule>>& ewar_module_list,
           vector<shared_ptr<Weapon>>& weapon_list)
    : engine_(std::move(engine)),
      capacitor_(std::move(capacitor)), 
      targeting_(std::move(targeting)),
      defense_(std::move(defense)),
      hull_(std::move(hull)),
      ewar_(ewar_module_list),
      weapons_(weapon_list),
      effect_map_(this) {}

Ship::Ship(unique_ptr<ShipEngine>& engine,
           unique_ptr<ShipCapacitor>& capacitor,
           unique_ptr<ShipTargeting>& targeting,
           unique_ptr<ShipDefense>& defense,
           unique_ptr<ShipHull>& hull,
           EwarContainer& ewar_container,
           WeaponContainer& weapon_container)
    : engine_(std::move(engine)),
      capacitor_(std::move(capacitor)),
      targeting_(std::move(targeting)),
      defense_(std::move(defense)),
      hull_(std::move(hull)),
      ewar_(ewar_container.Copy()),
      weapons_(weapon_container.Copy()),
      effect_map_(this) {}

Ship::Ship(const Ship& c)
    : engine_(c.Engine()->Copy()),
      capacitor_(c.Capacitor()->Copy()),
      targeting_(c.Targeting()->Copy()),
      defense_(c.Defense()->Copy()),
      ewar_(c.ewar_.Copy()),
      weapons_(c.weapons_.Copy()),
      effect_map_(this) {}

void Ship::ApplyEffect(const shared_ptr<Effect>& effect) {
  effect_map_.AddEffect(effect);
}

void Ship::ApplyEffect(const shared_ptr<EwarModule>& ewar) {
  for (const auto& effect : *ewar.get()) {
    ApplyEffect(effect);
  }
}

bool Ship::RemoveEffect(const Effect* effect) {
  return effect_map_.RemoveEffect(effect);
}

bool Ship::RemoveEffect(const EwarModule* ewar) {
  bool result = false;
  for (const auto& effect : *ewar) {
    result = RemoveEffect(effect.get());
  }

  return result;
}

float Ship::Dps(const ShipResistances* res) {
  float total_dps = 0;

  for (const auto& weapon_type : weapons_) {
    for (const auto& weapon : weapon_type.second) {
      total_dps += weapon->Dps(res);
    }
  }

  return total_dps;
}

float Ship::Dps(const shared_ptr<abyss::Bot>& target) {
  float total_dps = 0;
  for (const auto& weapon_type : weapons_) {
    for (const auto& weapon : weapon_type.second) {
      total_dps += weapon->Dps(target);
    }
  }
  return total_dps;
}

shared_ptr<Ship> Ship::Copy() const {
  return make_unique<Ship>(*this);
}

} // namespace eve
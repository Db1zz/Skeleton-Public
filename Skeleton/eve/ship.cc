#include "ship.h"
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
      hps_(hps),
      base_hps_(hps) {}

ShipDefense::ShipDefense(const ShipDefense& d)
    : ShipDefense(d.ship_res_, d.armor_hp_, d.shield_hp_, 
                  d.hull_hp_, d.hps_) {}

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

Ship::Ship(unique_ptr<ShipEngine>& engine,
           unique_ptr<ShipCapacitor>& capacitor,
           unique_ptr<ShipTargeting>& targeting,
           unique_ptr<ShipDefense>& defense,
           vector<shared_ptr<EwarModule>>& ewar_module_list,
           vector<shared_ptr<Weapon>>& weapon_list)
    : engine_(std::move(engine)),
      capacitor_(std::move(capacitor)), 
      targeting_(std::move(targeting)),
      defense_(std::move(defense)),
      ewar_(ewar_module_list),
      weapons_(weapon_list),
      effect_map_(this) {}

Ship::Ship(unique_ptr<ShipEngine>& engine,
           unique_ptr<ShipCapacitor>& capacitor,
           unique_ptr<ShipTargeting>& targeting,
           unique_ptr<ShipDefense>& defense,
           EwarContainer& ewar_container,
           WeaponContainer& weapon_container)
    : engine_(std::move(engine)),
      capacitor_(std::move(capacitor)),
      targeting_(std::move(targeting)),
      defense_(std::move(defense)),
      ewar_(ewar_container),
      weapons_(weapon_container),
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

bool Ship::RemoveEffect(const Effect* effect) {
  return effect_map_.RemoveEffect(effect);
}

shared_ptr<Ship> Ship::Copy() const {
  return make_unique<Ship>(*this);
}

} // namespace eve
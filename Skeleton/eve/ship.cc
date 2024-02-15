#include "ship.h"
#include "effects.h"
#include "eve_math.h"
#include <iostream>
#include <utility>

namespace eve {

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

ShipDefense::ShipDefense(const ShipResistances& ship_res, float armor_hp,
                         float shield_hp, float hull_hp, float hps)
    : ship_res_(ship_res),
      armor_hp_(armor_hp),
      shield_hp_(shield_hp),
      hull_hp_(hull_hp),
      hps_(hps),
      base_hps_(hps) {}

ShipTargeting::ShipTargeting(float range)
    : base_range_(range), range_(range) {}

ShipCapacitor::ShipCapacitor(float recharge_rate, float amount)
    : base_recharge_rate_(recharge_rate),
      base_amount_(amount),
      recharge_rate_(recharge_rate), 
      amount_(amount) {}

ShipEngine::ShipEngine(float velocity)
    : velocity_(velocity), base_velocity_(velocity) {}

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

void Ship::ApplyEffect(const shared_ptr<Effect>& effect) {
  effect_map_.AddEffect(effect);
}

bool Ship::RemoveEffect(const Effect* effect) {
  return effect_map_.RemoveEffect(effect);
}

} // namespace eve
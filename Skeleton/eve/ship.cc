#include "ship.h"
#include "effects.h"
#include "eve_math.h"
#include <iostream>

namespace eve {

ShipDefense::ShipDefense(const ShipResistances& ship_res, float armor_hp,
                         float shield_hp, float hull_hp, float hps)
    : ship_res_(ship_res),
      armor_hp_(armor_hp),
      shield_hp_(shield_hp),
      hull_hp_(hull_hp),
      hps_(hps),
      max_hps_(hps) {}

ShipTargeting::ShipTargeting(float range)
    : max_range_(range), range_(range) {}

ShipCapacitor::ShipCapacitor(float recharge_rate, float amount)
    : max_recharge_rate_(recharge_rate),
      max_amount_(amount),
      recharge_rate_(recharge_rate), 
      amount_(amount) {}

Ship::Ship(shared_ptr<ShipEngine> engine,
           shared_ptr<ShipCapacitor> capacitor, 
           shared_ptr<ShipTargeting> targeting,
           shared_ptr<ShipDefense> defense,
           shared_ptr<ShipEwarVector<>> ewar)
    : engine_(engine),
      capacitor_(capacitor),
      targeting_(targeting),
      defense_(defense),
      effect_map_(std::make_unique<ShipEffectsMap>(this)),
      ewar_(ewar) {}

void Ship::ApplyEffect(shared_ptr<ShipEffect> effect) {
  effect_map_.get()->AddEffect(effect);
}

} // namespace eve

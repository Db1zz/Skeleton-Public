#include "ship.h"
#include "effects.h"
#include "eve_math.h"
#include <iostream>
#include <utility>

namespace eve {

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

ShipEwarModule::ShipEwarModule(float optimal, float falloff, float rof, 
                               const shared_ptr<ShipEffect>& effect)
    : optimal_(optimal),
      falloff_(falloff),
      rof_(rof),
      effect_(effect) {}

ShipEwarVector::ShipEwarVector(
    vector<shared_ptr<ShipEwarModule>>& ewar_module_list) {
  for (auto &element : ewar_module_list) { 
    AddEwarModule(element); 
  }
}

vector<ShipEwarModule*> ShipEwarVector::FindEwarByType(ShipEffect::Type type) {
  vector<ShipEwarModule*> result;

  for (const auto& module : ewar_modules_) {
    if (module->Effect()->GetType() == type) {
      result.push_back(module.get());
    }
  }
  return result;
}

void ShipEwarVector::AddEwarModule(shared_ptr<ShipEwarModule>& ewar_module) {
  ewar_modules_.push_back(ewar_module);
}

bool ShipEwarVector::RemoveEwarModule(ShipEwarModule* ewar_module) {
  ShipEffect* module_effect = ewar_module->Effect().get();

  for (int i = 0; i < Size(); i++) {
    ShipEffect* found_effect = ewar_modules_[i]->Effect().get(); 
    if (found_effect->GetType() == module_effect->GetType() &&
        found_effect->Strength() == module_effect->Strength()) 
    {
      ewar_modules_.erase(begin() + i);
      return true;
    }
  }
  return false;
}

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

Ship::Ship(unique_ptr<ShipEngine>& engine,
           unique_ptr<ShipCapacitor>& capacitor,
           unique_ptr<ShipTargeting>& targeting,
           unique_ptr<ShipDefense>& defense,
           vector<shared_ptr<ShipEwarModule>>& ewar_module_list,
           vector<shared_ptr<Weapon>>& weapon_list)
    : engine_(std::move(engine)),
      capacitor_(std::move(capacitor)), 
      targeting_(std::move(targeting)),
      defense_(std::move(defense)),
      ewar_(ewar_module_list),
      weapons_(weapon_list),
      effect_map_(this) {}

void Ship::ApplyEffect(const shared_ptr<ShipEffect>& effect) {
  effect_map_.AddEffect(effect);
}

} // namespace eve

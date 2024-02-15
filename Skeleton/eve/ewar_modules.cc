#include "ewar_modules.h"
#include "effects.h"
#include "ship.h"

namespace eve {

using std::make_unique;

EwarModule::EwarModule(
  string source, Type ewar_type, float optimal, float falloff,
  float rof, const vector<shared_ptr<Effect>>& effects)
    : source_(source), 
      ewar_type_(ewar_type), 
      optimal_(optimal),
      falloff_(falloff), 
      rof_(rof),
      effects_(effects) {}

// EnegryNeutralizerEwar::EnegryNeutralizerEwar(
//   string source, float optimal, float falloff, 
//   float rof, vector<float> effects_strengths)
//     : EwarModule(source, Type::EnergyNeutralizer,
//                  optimal, falloff, rof, effects_strengths),
//       cap_rech_decr_effect_(
//         make_unique<EwarCapacitorRegenDecreaseEffect>(
//             source, effects_strengths[0])) {}
      

// void EnegryNeutralizerEwar::ApplyEwar(Ship* target) {
//   target->ApplyEffect(cap_rech_decr_effect_);
// }

// void EnegryNeutralizerEwar::RemoveEwar(Ship* target) {

// }

EwarContainer::EwarContainer(
    vector<shared_ptr<EwarModule>>& ewar_module_list) {
  for (auto &element : ewar_module_list) { 
    AddEwarModule(element); 
  }
}

vector<EwarModule*> EwarContainer::FindEwarByType(EwarModule::Type type) {
  vector<EwarModule*> result;

  for (const auto& module : ewar_modules_) {
    if (module->GetType() == type) {
      result.push_back(module.get());
    }
  }
  return result;
}

void EwarContainer::AddEwarModule(shared_ptr<EwarModule>& ewar_module) {
  ewar_modules_.push_back(ewar_module);
}

bool EwarContainer::RemoveEwarModule(EwarModule* ewar_module) {
  const vector<shared_ptr<Effect>>* effects = ewar_module->GetEffects();

  for (int i = 0; i < Size(); i++) {
    const vector<shared_ptr<Effect>>* search = ewar_modules_[i]->GetEffects();
    if (search->size() == effects->size()) {
      for (int j = 0; j < effects->size(); j++) {
        if ((*search)[j]->GetType() == (*effects)[j]->GetType() &&
            (*search)[j]->GetSource() == (*effects)[j]->GetSource() &&
            (*search)[j]->GetStrength() == (*effects)[j]->GetStrength()) {
          ewar_modules_.erase(begin() + i);
          return true;
        }
      }
    }
  }
  return false;
}

};
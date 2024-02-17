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

shared_ptr<Effect>* EwarModule::GetSpecifcEffect(EffectType type) {
  for (auto& effect : effects_) {
    if (effect->GetType() == type) {
      return &effect;
    }
  }
  return nullptr;
}

void EwarModule::ApplyEwar(Ship* target) {
  for (const auto& effect : effects_) {
    target->ApplyEffect(effect);
  }
}

void EwarModule::RemoveEwar(Ship* target) {
  for (const auto& effect: effects_) {
    target->RemoveEffect(effect.get());
  }
}

shared_ptr<EwarModule> EwarModule::Copy() const {
  vector<shared_ptr<Effect>> effects;
  for (auto& effect : effects_) {
    effects.push_back(effect->Copy());
  }

  shared_ptr<EwarModule> ewar = make_unique<EwarModule>(
      source_, ewar_type_, optimal_, falloff_, rof_, effects);

  return ewar;
}

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

EwarContainer EwarContainer::Copy() const {
  vector<shared_ptr<EwarModule>> ewar_modules;

  for (auto& ewar : ewar_modules_) {
    ewar_modules.push_back(ewar->Copy());
  }

  EwarContainer new_container(ewar_modules);

  return new_container;
}

};
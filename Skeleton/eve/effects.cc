#include "effects.h"
#include "ship.h"
#include "eve_math.h"

namespace eve {

using std::shared_ptr;

bool ShipEffectVector::InsertEffect(const shared_ptr<ShipEffect>& effect) {
  int i = 0;
  for (; i < Size(); i++) {
    if (effect->Strength() >= effect_array_[i]->Strength()) {
      break;
    }
  }
  effect_array_.insert(begin() + i, effect);
  effect_str_vector_.insert(effect_str_vector_.begin() + i, effect->Strength());
  return true;
}

bool ShipEffectVector::EraseEffect(const ShipEffect* effect) {
  for (int i = 0; i < Size(); i++) {
    if (effect_array_[i]->Source() == effect->Source()) {
      effect_array_.erase(begin() + i);
      return true;
    }
  }
  return false;
}

bool EffectManager::RemoveEffect(const ShipEffect* effect) {
  if (EraseEffect(effect)) {
    CalculateApplyEffect();
    return true;
  }
  return false;
}

void StasisWebifierManager::ApplyEffect(const shared_ptr<ShipEffect>& effect) {
  InsertEffect(effect);
  CalculateApplyEffect();
}

void StasisWebifierManager::CalculateApplyEffect() {
  float max_ship_velocity = ship_->Engine()->MaxVelocity();
  float new_velocity = ApplyStackingPenalty(max_ship_velocity,
                                            effect_str_vector_);
  ship_->Engine()->SetVelocity(new_velocity);
}

void ShipEffectsMap::AddEffect(const shared_ptr<ShipEffect>& effect) {
  auto manager_it = effects_map_.find(effect->GetType());

  if (manager_it == effects_map_.end()) {
    shared_ptr<EffectManager> s;

    switch (effect->GetType()) {
      case ShipEffect::StasisWebifier: {
        s = make_unique<StasisWebifierManager>(ship_);
      }
    }

    s->ApplyEffect(effect);
    effects_map_.insert({effect->GetType(), s});

  } else {
    manager_it->second->ApplyEffect(effect);
  }
}

bool ShipEffectsMap::RemoveEffect(const ShipEffect* effect) {
  auto found_it = GetEffectManager(effect);
  if (!found_it)
    return false;

  return found_it->EraseEffect(effect);
}

shared_ptr<EffectManager> ShipEffectsMap::GetEffectManager(
    const ShipEffect* effect) {
  auto effect_it = effects_map_.find(effect->GetType());
  if (effect_it == effects_map_.end()) {
    return nullptr;
  }
  return effect_it->second;
}

} // namespace eve

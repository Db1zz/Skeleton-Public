#include "effects.h"
#include "ship.h"
#include "eve_math.h"

namespace eve {

using std::shared_ptr;

bool ShipEffectVector::InsertEffect(shared_ptr<ShipEffect> effect) {
  int i = 0;
  for (; i < Size(); i++) {
    if (effect->Strength() >= effect_array_[i].get()->Strength()) {
      break;
    }
  }
  effect_array_.insert(begin() + i, effect);
  effect_str_vector_.insert(effect_str_vector_.begin() + i,
                            effect.get()->Strength());
  return true;
}

bool ShipEffectVector::EraseEffect(shared_ptr<ShipEffect> effect) {
  for (int i = 0; i < Size(); i++) {
    if (effect_array_[i].get()->Source() == effect.get()->Source()) {
      effect_array_.erase(begin() + i);
      return true;
    }
  }
  return false;
}

bool EffectManager::RemoveEffect(shared_ptr<ShipEffect> effect) {
  if (EraseEffect(effect)) {
    CalculateApplyEffect();
    return true;
  }
  return false;
}

void StasisWebifierManager::ApplyEffect(shared_ptr<ShipEffect> effect) {
  InsertEffect(effect);
  CalculateApplyEffect();
}

void StasisWebifierManager::CalculateApplyEffect() {
  float max_ship_velocity = ship_->Engine().get()->MaxVelocity();
  float new_velocity = ApplyStackingPenalty(max_ship_velocity,
                                            effect_str_vector_);
  ship_->Engine().get()->SetVelocity(new_velocity);
}

void ShipEffectsMap::AddEffect(shared_ptr<ShipEffect> effect) {
  auto manager_it = effects_map_.find(effect.get()->GetType());

  if (manager_it == effects_map_.end()) {
    shared_ptr<StasisWebifierManager> s =
        make_unique<StasisWebifierManager>(ship_);

    s.get()->ApplyEffect(effect);
    effects_map_.insert({effect.get()->GetType(), s});

  } else {
    manager_it->second.get()->ApplyEffect(effect);
  }
}

bool ShipEffectsMap::RemoveEffect(shared_ptr<ShipEffect> effect) {
  auto found_it = GetEffectManager(effect);
  if (!found_it)
    return false;

  return found_it.get()->EraseEffect(effect);
}

shared_ptr<EffectManager> ShipEffectsMap::GetEffectManager(
    shared_ptr<ShipEffect> effect) {
  auto effect_it = effects_map_.find(effect.get()->GetType());
  if (effect_it == effects_map_.end()) {
    return nullptr;
  }
  return effect_it->second;
}

} // namespace eve

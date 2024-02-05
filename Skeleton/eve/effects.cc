#include "effects.h"

namespace eve {

using std::shared_ptr;

ShipEffect::ShipEffect(Type type, std::string source, float strength) 
    : type_(type), source_(source), strength_(strength) {}

bool ShipEffectVector::InsertEffect(shared_ptr<ShipEffect> effect) {
  if (Size() == 0) {
    effect_array_.insert(Begin(), effect);
    return true;
  }

  for (int i = 0; i < Size(); i++) {
    if (effect->GetStrength() >= effect_array_[i].get()->GetStrength()) {
      effect_array_.insert(Begin() + i, effect);
      return true;
    }
  }
  return false;
}

bool ShipEffectVector::RemoveEffect(std::string& source) {
  for (int i = 0; i < Size(); i++) {
    if (effect_array_[i].get()->GetSource() == source) {
      effect_array_.erase(Begin() + i);
      return true;
    }
  }
  return false;
}

bool ShipEffectsMap::AddEffect(shared_ptr<ShipEffect> effect) {
  ShipEffectVector* effects = GetEffect(*effect);
  if (!effects) {
    effects_map_.insert({effect.get()->GetType(), {effect}});
  } else {
    effects->InsertEffect(effect);
  }
  return true;
}

bool ShipEffectsMap::RemoveEffect(ShipEffect::Type type, std::string& source) {
  for (auto& effect : effects_map_) {
    if (effect.first == type) {
      return effect.second.RemoveEffect(source);
    }
  }
  return false;
}

ShipEffectVector* ShipEffectsMap::GetEffect(ShipEffect effect) {
  auto effect_it = effects_map_.find(effect.GetType());
  if (effect_it == effects_map_.end()) {
    return nullptr;
  }
  return &effect_it->second;
}

} // namespace eve

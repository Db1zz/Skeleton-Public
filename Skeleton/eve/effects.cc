#include "effects.h"
#include "ship.h"

#include <utility>

namespace eve {

using std::make_unique;

Effect::Effect(string source, EffectType type, float strength)
    : source_(source), 
      strength_(strength),
      type_(type) {}


void EwarCapacitorRegenDecreaseEffect::ApplyToTarget(
    Ship* target, float strength) 
{
  target->Capacitor()->SetRechargetRate((
      target->Capacitor()->RechargeRate() - strength));
}

void EwarCapacitorRegenDecreaseEffect::RemoveFromTarget(
    Ship* target, float strength) 
{
  target->Capacitor()->SetRechargetRate((
      target->Capacitor()->RechargeRate() + strength));
}

shared_ptr<Effect> EwarCapacitorRegenDecreaseEffect::Copy() const {
  return make_unique<EwarCapacitorRegenDecreaseEffect>(
      source_, strength_);
}

void EwarVelocityDecreaseEffect::ApplyToTarget(Ship* target, float strength) {
  target->Engine()->SetVelocity(target->Engine()->Velocity() * strength);
}

void EwarVelocityDecreaseEffect::RemoveFromTarget(Ship* target, float strength){
  target->Engine()->SetVelocity(target->Engine()->Velocity() / strength);
}

shared_ptr<Effect> EwarVelocityDecreaseEffect::Copy() const {
  return make_unique<EwarVelocityDecreaseEffect>(
      source_, strength_);
}

void EwarLockRangeDecreaseEffect::ApplyToTarget(Ship* target, float strength) {
  target->Targeting()->SetRange((target->Targeting()->Range() * strength));
}

void EwarLockRangeDecreaseEffect::RemoveFromTarget(
    Ship* target, float strength) 
{
  target->Targeting()->SetRange((target->Targeting()->Range() / strength));
}

shared_ptr<Effect> EwarLockRangeDecreaseEffect::Copy() const {
  return make_unique<EwarLockRangeDecreaseEffect>(
      source_, strength_);
}

void EwarArmorRestorationEffect::ApplyToTarget(Ship* target, float strength) {
  // Accepts only HP/s, DON'T USE raw values.
  target->Defense()->SetHPs(target->Defense()->HPs() + strength);
}

void EwarArmorRestorationEffect::RemoveFromTarget(Ship* target, float strength){
  // Accepts only HP/s, DON'T USE raw values.
  target->Defense()->SetHPs(target->Defense()->HPs() - strength);
}

shared_ptr<Effect> EwarArmorRestorationEffect::Copy() const {
  return make_unique<EwarArmorRestorationEffect>(
      source_, strength_);
}

EffectManager::EffectManager(EffectType effect_type, Ship* target)
    : effect_type_(effect_type), target_(target) {}

void EffectManager::ApplyEffect(const shared_ptr<Effect>& effect) {
  int index = 0;
  for (; index < effects_.size(); index++) {
    if (effect->GetStrength() >= effects_[index].second->GetStrength()) {
      break;
    }
  }
  UnapplyEffects(effects_.size() - 1, index);
  effects_.insert(effects_.begin() + index, {0, effect});
  RecalcEffectsAndApply(index, effects_.size());
}

bool EffectManager::RemoveEffect(const Effect* to_remove) {
  int i = 0;
  size_t size = effects_.size();
  for (; i < size; i++) {
    shared_ptr<Effect>& found = effects_[i].second;
    if (found->GetType() == to_remove->GetType() &&
        found->GetSource() == to_remove->GetSource() &&
        found->GetStrength() == to_remove->GetStrength()) {
      
      UnapplyEffects(size - 1, i);

      effects_.erase(effects_.begin() + i);
      break;
    }
  }
  RecalcEffectsAndApply(i, effects_.size());
  return false;
}

void EffectManager::RecalcEffectsAndApply(int start, int end) {
  if (effects_.size() == 0)
    return;

  assert(start <= end);

  for (; start < end; start++) {
    float str = 0;

    if (effects_[start].second->IsPercentageEffect()) {
      str = ApplyStackingPenalty(effects_[start].second->GetStrength(), start);
    } else {
      str = effects_[start].second->GetStrength();
    }

    effects_[start].first = str;
    effects_[start].second->ApplyToTarget(target_, str);
  }
}

void EffectManager::UnapplyEffects(int start, int end) {
  if (effects_.size() == 0)
    return;

  assert(start >= end);

  for (; start >= end; start--) {
    effects_[start].second->RemoveFromTarget(target_, 
                                             effects_[start].first);
  }
}

void EffectsContainer::AddEffect(const shared_ptr<Effect>& effect) {
  // Try to find manager in managers_map_
  auto manager_it = managers_map_.find(effect->GetType());

  // Check if effect manager is not exist in the managers_map_.
  if (manager_it == managers_map_.end()) {
    // Dynamicly allocate new manager
    unique_ptr<EffectManager> manager = 
        std::make_unique<EffectManager>(effect->GetType(), target_);

    // Apply effect to target_
    manager->ApplyEffect(effect);

    // Emplace manager in managers_map_
    managers_map_.emplace(effect->GetType(), std::move(manager));
  } else {
    // Simply use ApplyEffect if manager already exist in managers_map_ 
    manager_it->second->ApplyEffect(effect);
  }
}

bool EffectsContainer::RemoveEffect(const Effect* effect) {
  // Get pointer to the manager
  EffectManager* manager = GetEffectManager(effect);

  // Check if GetEffectManager() returned nullptr
  if (!manager)
    return false;

  // Return result of RemoveEffect()
  return manager->RemoveEffect(effect);
}

EffectManager* EffectsContainer::GetEffectManager(const Effect* effect) {
  // Obtain map iterator for manager
  auto effect_it = managers_map_.find(effect->GetType());

  // Check if find() returned unordered_map::end()
  if (effect_it == managers_map_.end()) {
    return nullptr;
  }

  // Return pointer to the EffectManager
  return effect_it->second.get();
}

unique_ptr<EffectManager> EffectManager::Copy() {
  unique_ptr<EffectManager> manager = 
      make_unique<EffectManager>(effect_type_, target_);

  for (auto& effect : effects_) {
    manager->effects_.push_back({effect.first, effect.second->Copy()});
  }

  return std::move(manager);
}

EffectsContainer EffectsContainer::Copy() {
  EffectsContainer container(target_);
  for (const auto& manager : managers_map_) {
    container.managers_map_.emplace(manager.first, manager.second->Copy());
  }
  
  return container;
}

} // namespace eve
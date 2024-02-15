#ifndef EFFECTS_H_
#define EFFECTS_H_

#include "eve_math.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace eve {

class Ship;
class EffectManager;

using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_map;

enum class EffectType {
  EwarCapacitorRegenDecreaseEffect,
  EwarVelocityDecreaseEffect,
  EwarLockRangeDecreaseEffect,
  EwarArmorRestorationEffect,
};

enum class EffectSource {
  AbyssEnironmentEffect,
  TrackingDisruptor,
};

class Effect {
  public:
    Effect(EffectSource source, EffectType type, float base_strength);

    inline EffectSource GetSource() const {
      return source_;
    }

    inline EffectType GetType() const {
      return type_;
    }

    inline float GetStrength() const {
      return base_strength_;
    }

  private:
    virtual void ApplyToTarget(Ship* target, float strength) = 0;

    virtual void RemoveFromTarget(Ship* target, float strength) = 0;

    virtual bool IsPercentageEffect() const = 0;

    friend EffectManager;

    EffectSource source_;
    EffectType type_;
    const float base_strength_;
};

class EwarCapacitorRegenDecreaseEffect : public Effect {
  public:
    EwarCapacitorRegenDecreaseEffect(EffectSource source, float base_strength)
        : Effect(source, EffectType::EwarCapacitorRegenDecreaseEffect, 
                 base_strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    inline bool IsPercentageEffect() const override {
      return false;
    }
};

class EwarVelocityDecreaseEffect : public Effect {
  public:
    EwarVelocityDecreaseEffect(EffectSource source, float strength)
        : Effect(source, EffectType::EwarVelocityDecreaseEffect, strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    inline bool IsPercentageEffect() const override {
      return true;
    }
};

class EwarLockRangeDecreaseEffect : public Effect {
  public:
    EwarLockRangeDecreaseEffect(EffectSource source, float strength)
        : Effect(source, EffectType::EwarLockRangeDecreaseEffect, strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    inline bool IsPercentageEffect() const override {
      return true;
    }
};

class EwarArmorRestorationEffect : public Effect {
  public:
    EwarArmorRestorationEffect(EffectSource source, float strength)
        : Effect(source, EffectType::EwarArmorRestorationEffect, strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    inline bool IsPercentageEffect() const override {
      return false;
    }
};

class EffectManager {
  public:
    EffectManager(EffectType effect_type, Ship* target);

    inline EffectType GetType() const {
      return effect_type_;
    }

    void ApplyEffect(const shared_ptr<Effect>& effect);

    bool RemoveEffect(const Effect* to_remove);

  private:
    void RecalcEffectsAndApply(int start, int end);

    void UnapplyEffects(int start, int end);

    Ship* target_;
    EffectType effect_type_;
    vector<std::pair<float, shared_ptr<Effect>>> effects_;
};

class EffectsContainer {
  public:
    EffectsContainer(Ship* target) : target_(target) {}

    void AddEffect(const shared_ptr<Effect>& effect);

    bool RemoveEffect(const Effect* effect);

    EffectManager* GetEffectManager(const Effect* effect);

    inline auto begin() const {
      return managers_map_.begin();
    }

    inline auto end() const {
      return managers_map_.end();
    }

  private:
    unordered_map<EffectType, unique_ptr<EffectManager>> managers_map_;
    Ship* target_;
};

} // namespace eve

#endif // EFFECTS_H_
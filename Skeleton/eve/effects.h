#ifndef EFFECTS_H_
#define EFFECTS_H_

#include "eve_math.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <string>

namespace eve {

class Ship;
class EffectManager;

using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_map;
using std::string;

enum class EffectType {
  EwarCapacitorRegenDecreaseEffect,
  EwarVelocityDecreaseEffect,
  EwarLockRangeDecreaseEffect,
  EwarArmorRestorationEffect,
};

class Effect {
  public:
    Effect(string source, EffectType type, float strength);

    inline string GetSource() const {
      return source_;
    }

    inline EffectType GetType() const {
      return type_;
    }

    inline float GetStrength() const {
      return strength_;
    }

    virtual shared_ptr<Effect> Copy() const = 0;

  protected:
    virtual void ApplyToTarget(Ship* target, float strength) = 0;

    virtual void RemoveFromTarget(Ship* target, float strength) = 0;

    virtual bool IsPercentageEffect() const = 0;

    friend EffectManager;

    string source_;
    EffectType type_;
    const float strength_;
};

class EwarCapacitorRegenDecreaseEffect : public Effect {
  public:
    EwarCapacitorRegenDecreaseEffect(string source, float strength)
        : Effect(source, EffectType::EwarCapacitorRegenDecreaseEffect, 
                 strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    shared_ptr<Effect> Copy() const override;

    inline bool IsPercentageEffect() const override {
      return false;
    }
};

class EwarVelocityDecreaseEffect : public Effect {
  public:
    EwarVelocityDecreaseEffect(string source, float strength)
        : Effect(source, EffectType::EwarVelocityDecreaseEffect, strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    shared_ptr<Effect> Copy() const override;

    inline bool IsPercentageEffect() const override {
      return true;
    }
};

class EwarLockRangeDecreaseEffect : public Effect {
  public:
    EwarLockRangeDecreaseEffect(string source, float strength)
        : Effect(source, EffectType::EwarLockRangeDecreaseEffect, strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    shared_ptr<Effect> Copy() const override;

    inline bool IsPercentageEffect() const override {
      return true;
    }
};

class EwarArmorRestorationEffect : public Effect {
  public:
    EwarArmorRestorationEffect(string source, float strength)
        : Effect(source, EffectType::EwarArmorRestorationEffect, strength) {}

    void ApplyToTarget(Ship* target, float strength) override;

    void RemoveFromTarget(Ship* target, float strength) override;

    shared_ptr<Effect> Copy() const override;

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

    unique_ptr<EffectManager> Copy();

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

    EffectsContainer Copy();

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
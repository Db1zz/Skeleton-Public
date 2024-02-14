#include "../Skeleton/eve/eve_math.h"

#include <vector>
#include <functional>
#include <memory>
#include <cassert>

using namespace eve;

using std::vector;
using std::shared_ptr;

enum EffectType {
  TurretOptimalDecrease,
  StasisWebVelocityDecrease
};

enum EffectSource {
  AbyssEnironmentEffect,
  TrackingDisruptor,
};

class Ship {
  public:
    float FirstParam() const {
      return first_param_;
    }

    void SetFirstParam(float new_value) {
      first_param_ = new_value;
    }

    float SecondParam() const {
      return second_param_;
    }

    void SetSecondParam(float new_value) {
      second_param_ = new_value;
    }

  private:
    float first_param_ = 17.4;
    float second_param_ = 925;
};

class Effect {
  public:
    Effect(EffectSource source, EffectType type, float base_strength)
        : source_(source), 
          base_strength_(base_strength),
          // current_strength_(base_strength),
          type_(type) {}

    EffectSource Source() const {
      return source_;
    }

    EffectType Type() const {
      return type_;
    }

    float BaseStrength() const {
      return base_strength_;
    }

    // float CurrentStrength() const {
    //   return current_strength_;
    // }

    // void SetStrength(float new_value) {
    //   current_strength_ = new_value;
    // }

    // virtual void ApplyToTarget(Ship* target, float value) = 0;
    // virtual void RemoveFromTarget(Ship* target);

    virtual void ApplyToTarget(Ship* target, float strength) = 0;
    virtual void RemoveFromTarget(Ship* target, float strength) = 0;

    virtual bool IsPercentageEffect() const = 0;

  private:
    EffectSource source_;
    EffectType type_;
    const float base_strength_;
    // float current_strength_;
};

class EnergyNeutEffect : public Effect {
  public:
    EnergyNeutEffect(EffectSource source, EffectType type, float base_strength)
        : Effect(source, type, base_strength) {}

    // Calc % by stacking penalty
    // Apply % to target

    // The strength of Effect should be calculated
    // in the effectmanager and then set to Effect
    // And only then we will be able to apply it on target.
    void ApplyToTarget(Ship* target, float strength) override {
      target->SetFirstParam(target->FirstParam() - strength);
    }

    void RemoveFromTarget(Ship* target, float strength) override {
     target->SetFirstParam(target->FirstParam() + strength); 
    }

    bool IsPercentageEffect() const override {
      return false;
    }
};

class StasisWebVelocityDecreaseEffect : public Effect {
  public:
    StasisWebVelocityDecreaseEffect(EffectSource source, float strength)
        : Effect(source, EffectType::StasisWebVelocityDecrease, strength) {}

    void ApplyToTarget(Ship* target, float strength) override {
      target->SetSecondParam(target->SecondParam() * strength);
    }

    void RemoveFromTarget(Ship* target, float strength) override {
     target->SetSecondParam(target->SecondParam() / strength);
    }

    bool IsPercentageEffect() const override {
      return true;
    }
};

class EffectManager {
  public:
    EffectManager(EffectType effect_type, Ship* target)
        : effect_type_(effect_type), target_(target) {}

    EffectType Type() const {
      return effect_type_;
    }

    void ApplyEffect(shared_ptr<Effect> effect) {
      int index = 0;
      for (; index < effects_.size(); index++) {
        if (effect->BaseStrength() >= effects_[index].second->BaseStrength()) {
          break;
        }
      }
      UnapplyEffects(effects_.size() - 1, index);
      effects_.insert(effects_.begin() + index, {0, effect});
      RecalcEffectsAndApply(index, effects_.size());
    }

    bool RemoveEffect(Effect* to_remove) {
      int i = 0;
      size_t size = effects_.size();
      for (; i < size; i++) {
        shared_ptr<Effect>& found = effects_[i].second;
        if (found->Type() == to_remove->Type() &&
            found->Source() == to_remove->Source() &&
            found->BaseStrength() == to_remove->BaseStrength()) {
          
          UnapplyEffects(size - 1, i);

          effects_.erase(effects_.begin() + i);
          break;
        }
      }
      RecalcEffectsAndApply(i, effects_.size());
      return false;
    }

    void RecalcEffectsAndApply(int start, int end) {
      if (effects_.size() == 0)
        return;

      assert(start <= end);

      for (; start < end; start++) {
        float new_str = 
            ApplyStackingPenalty(effects_[start].second->BaseStrength(), start);
        effects_[start].first = new_str;
        effects_[start].second->ApplyToTarget(target_, new_str);
      }
    }

    void UnapplyEffects(int start, int end) {
      if (effects_.size() == 0)
        return;

      assert(start >= end);

      for (; start >= end; start--) {
        effects_[start].second->RemoveFromTarget(target_, 
                                                 effects_[start].first);
      }
    }

  private:
    Ship* target_;
    EffectType effect_type_;
    vector<std::pair<float, shared_ptr<Effect>>> effects_;
};
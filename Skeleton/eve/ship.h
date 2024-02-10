#ifndef SHIP_H_
#define SHIP_H_

#include "hp_resistances.h"
#include "effects.h"

#include <memory>
#include <iostream>
#include <vector>
#include <utility>

namespace eve {

using std::shared_ptr;
using std::vector;

class ShipEffect;
class ShipEffectsMap;

// TODO: Add DroneBay class to manage and 
// store data about Ship DroneBay

class ShipEwarModule {
  public:
    ShipEwarModule(float optimal, float falloff, float rof, 
                   shared_ptr<ShipEffect> effect)
        : optimal_(optimal),
          falloff_(falloff),
          rof_(rof),
          effect_(effect) {}
  
  inline shared_ptr<ShipEffect> Effect() const {
    return effect_;
  }

  inline float Optimal() const {
    return optimal_;
  }

  inline float Falloff() const {
    return falloff_;
  }

  private:
    shared_ptr<ShipEffect> effect_;
    float optimal_;
    float falloff_;
    float rof_;
};

template <typename... Args>
class ShipEwarVector {
  public:
    ShipEwarVector() = default;

    ShipEwarVector(Args&... args) 
        : ewar_modules_{args...} {}
  
    ShipEwarModule& operator[](int index) {
      return ewar_modules_[index];
    }

    vector<shared_ptr<ShipEwarModule>> FindEwarByType(ShipEffect::Type&& type) {
      vector<shared_ptr<ShipEwarModule>> result;
      
      for (const auto& module : ewar_modules_) {
        if (module.get()->Effect().get()->GetType() == type) {
          result.push_back(module);
        }
      }
      return result;
    }

    void AddEwarModule(shared_ptr<ShipEwarModule> ewar_module) {
      ewar_modules_.push_back(ewar_module);
    }

    bool RemoveEwarModule(shared_ptr<ShipEwarModule> ewar_module) {
      const ShipEffect* module_effect = ewar_module.get()->Effect().get();

      for (int i = 0; i < Size(); i++) {
        const ShipEffect* found_effect = ewar_modules_[i].get()->Effect().get();
        if (found_effect->GetType() == module_effect->GetType() &&
            found_effect->Strength() == module_effect->Strength()) 
        {
          ewar_modules_.erase(begin() + i);
          return true;
        }
      }
      return false;
    }

    inline int Size() {
      return ewar_modules_.size();
    }

    inline bool Empty() {
      return ewar_modules_.size();
    }

    inline auto begin() {
      return ewar_modules_.begin();
    }

    inline auto end() {
      return ewar_modules_.end();
    }

  private:
    vector<shared_ptr<ShipEwarModule>> ewar_modules_{};
};

class ShipDefense {
  public:
    ShipDefense(const ShipResistances& ship_res, float armor_hp,
                float shield_hp, float hull_hp, float hps);

    virtual ~ShipDefense() = default;

    inline ShipResistances& ShipRes() {
      return ship_res_;
    }

    inline void SetShipRes(ShipResistances new_val) {
      ship_res_ = new_val;
    }

    inline float ArmorHp() const {
      return armor_hp_;
    }

    inline float ShieldHp() const {
      return shield_hp_;
    }

    inline float HullHp() const {
      return hull_hp_;
    }

    inline float HPs() const {
      return hps_;
    }

    inline void SetHPs(float new_val) {
      hps_ = new_val;
    } 

    inline float MaxHPs() const {
      return max_hps_;
    } 

  private:
    ShipResistances ship_res_;
    const float armor_hp_;  
    const float shield_hp_;
    const float hull_hp_;
    const float max_hps_;
    float hps_;
};

class ShipTargeting {
  public:
    ShipTargeting(float range);

    virtual ~ShipTargeting() = default;

    inline float Range() const {
      return range_;
    }

    inline float MaxRange() const {
      return max_range_;
    }

    inline void SetRange(float new_val) {
      range_ = new_val;
    }

  private:
    const float max_range_;
    float range_;
};

class ShipCapacitor {
  public:
    ShipCapacitor(float recharge_rate, float amount);
    
    virtual ~ShipCapacitor() = default;

    inline float RechargeRate() const  {
      return recharge_rate_;
    }

    inline float Amount() const {
      return amount_;
    }

    inline float MaxAmount() const {
      return max_amount_;
    }

    inline float MaxRechargeRate() const {
      return max_recharge_rate_;
    }

    inline void SetAmount(float new_val) {
      amount_ = new_val;
    }

    inline void SetRechargetRate(float new_val) {
      recharge_rate_ = new_val;
    }

  private:
    const float max_recharge_rate_;
    const float max_amount_;
    float recharge_rate_;
    float amount_;
};

class ShipEngine {
  public:
    ShipEngine(float velocity)
        : velocity_(velocity), max_velocity_(velocity) {}

    virtual ~ShipEngine() = default;

    inline float Velocity() const {
      return velocity_;
    }

    inline float MaxVelocity() const {
      return max_velocity_;
    }

    inline void SetVelocity(float new_value) {
      velocity_ = new_value;
    }

  private:
    float velocity_;
    const float max_velocity_;
};

class Ship {
  public:
    Ship(shared_ptr<ShipEngine> engine,
         shared_ptr<ShipCapacitor> capacitor, 
         shared_ptr<ShipTargeting> targeting,
         shared_ptr<ShipDefense> defense,
         shared_ptr<ShipEwarVector<>> ewar);

    virtual ~Ship() = default;

    virtual void ApplyEffect(shared_ptr<ShipEffect> effect);

    inline virtual ShipEffectsMap* EffectMap() const {
      return effect_map_.get();
    }

    inline virtual ShipEngine* Engine() const {
      return engine_.get();
    }

    inline virtual ShipCapacitor* Capacitor() const {
      return capacitor_.get();
    }

    inline virtual ShipTargeting* Targeting() const {
      return targeting_.get();
    }

    inline virtual ShipDefense* Defense() const {
      return defense_.get();
    }

    inline virtual ShipEwarVector<>* Ewar() const {
      return ewar_.get();
    }

  private:
    shared_ptr<ShipEngine> engine_;
    shared_ptr<ShipCapacitor> capacitor_;
    shared_ptr<ShipTargeting> targeting_;
    shared_ptr<ShipDefense> defense_;
    shared_ptr<ShipEffectsMap> effect_map_;
    shared_ptr<ShipEwarVector<>> ewar_;
};

} // namespace eve

#endif // SHIP_H_

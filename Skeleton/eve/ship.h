#ifndef SHIP_H_
#define SHIP_H_

#include "hp_resistances.h"
#include "effects.h"
#include "ship_weapon.h"

#include <memory>
#include <iostream>
#include <vector>
#include <utility>

namespace eve {

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

class ShipEffect;
class ShipEffectsMap;

// TODO: Add DroneBay class to manage and 
// store data about Ship DroneBay

class ShipEwarModule {
  public:
    ShipEwarModule(float optimal, float falloff, float rof, 
                   const shared_ptr<ShipEffect>& effect);
                     
    inline shared_ptr<ShipEffect>& Effect() {
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

class ShipEwarVector {
  public:
    ShipEwarVector(const ShipEwarVector&) = delete;

    ShipEwarVector &operator=(const ShipEwarVector&) = delete;

    ShipEwarVector(vector<shared_ptr<ShipEwarModule>>& ewar_module_list);

    ShipEwarModule* operator[](int index) {
      return ewar_modules_[index].get();
    }

    vector<ShipEwarModule*> FindEwarByType(ShipEffect::Type type);

    void AddEwarModule(shared_ptr<ShipEwarModule>& ewar_module);

    bool RemoveEwarModule(ShipEwarModule* ewar_module);

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
    Ship(unique_ptr<ShipEngine>& engine,
         unique_ptr<ShipCapacitor>& capacitor,
         unique_ptr<ShipTargeting>& targeting,
         unique_ptr<ShipDefense>& defense,
         vector<shared_ptr<ShipEwarModule>>& ewar_module_list,
         vector<shared_ptr<Weapon>>& weapon_list);

    virtual ~Ship() = default;

    virtual void ApplyEffect(const shared_ptr<ShipEffect>& effect);

    inline virtual ShipEffectsMap* EffectMap() {
      return &effect_map_;
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

    inline virtual ShipEwarVector* Ewar() {
      return &ewar_;
    }

  private:
    unique_ptr<ShipEngine> engine_;
    unique_ptr<ShipCapacitor> capacitor_;
    unique_ptr<ShipTargeting> targeting_;
    unique_ptr<ShipDefense> defense_;
    ShipEffectsMap effect_map_;
    ShipEwarVector ewar_;
    vector<shared_ptr<Weapon>> weapons_;
};

} // namespace eve

#endif // SHIP_H_

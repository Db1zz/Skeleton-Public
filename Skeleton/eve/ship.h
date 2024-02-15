#ifndef SHIP_H_
#define SHIP_H_

#include "effects.h"
#include "ewar_modules.h"
#include "hp_resistances.h"
#include "ship_weapon.h"

#include <memory>
#include <iostream>
#include <vector>
#include <utility>

namespace eve {

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

// TODO: Add DroneBay class to manage and 
// store data about Ship DroneBay

class Effect;
class EffectManager;
class EffectsContainer;
class EwarModule;
class EwarContainer;

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

    inline float BaseHPS() const {
      return base_hps_;
    } 

  private:
    ShipResistances ship_res_;
    const float armor_hp_;  
    const float shield_hp_;
    const float hull_hp_;
    const float base_hps_;
    float hps_;
};

class ShipTargeting {
  public:
    ShipTargeting(float range);

    virtual ~ShipTargeting() = default;

    inline float Range() const {
      return range_;
    }

    inline float BaseRange() const {
      return base_range_;
    }

    inline void SetRange(float new_val) {
      range_ = new_val;
    }

  private:
    const float base_range_;
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

    inline float BaseAmount() const {
      return base_amount_;
    }

    inline float BaseRechargeRate() const {
      return base_recharge_rate_;
    }

    inline void SetAmount(float new_val) {
      amount_ = new_val;
    }

    inline void SetRechargetRate(float new_val) {
      recharge_rate_ = new_val;
    }

  private:
    const float base_recharge_rate_;
    const float base_amount_;
    float recharge_rate_;
    float amount_;
};

class ShipEngine {
  public:
    ShipEngine(float velocity);

    virtual ~ShipEngine() = default;

    inline float Velocity() const {
      return velocity_;
    }

    inline float BaseVelocity() const {
      return base_velocity_;
    }

    inline void SetVelocity(float new_value) {
      velocity_ = new_value;
    }

  private:
    float velocity_;
    const float base_velocity_;
};

class Ship {
  public:
    Ship(unique_ptr<ShipEngine>& engine,
         unique_ptr<ShipCapacitor>& capacitor,
         unique_ptr<ShipTargeting>& targeting,
         unique_ptr<ShipDefense>& defense,
         vector<shared_ptr<EwarModule>>& ewar_module_list,
         vector<shared_ptr<Weapon>>& weapon_list);

    virtual ~Ship() = default;

    virtual void ApplyEffect(const shared_ptr<Effect>& effect);

    virtual bool Ship::RemoveEffect(const Effect* effect);

    inline WeaponContainer* Weapons() {
      return &weapons_;
    }

    inline virtual EffectsContainer* EffectMap() {
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

    inline virtual EwarContainer* Ewar() {
      return &ewar_;
    }

  private:
    unique_ptr<ShipEngine> engine_;
    unique_ptr<ShipCapacitor> capacitor_;
    unique_ptr<ShipTargeting> targeting_;
    unique_ptr<ShipDefense> defense_;
    EffectsContainer effect_map_;
    EwarContainer ewar_;
    WeaponContainer weapons_;
};

} // namespace eve

#endif // SHIP_H_
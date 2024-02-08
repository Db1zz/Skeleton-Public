#ifndef SHIP_H_
#define SHIP_H_

#include "hp_resistances.h"
#include <memory>
#include <iostream>

namespace eve {

using std::shared_ptr;

class ShipEffect;
class ShipEffectsMap;

// TODO: Add DroneBay class to manage and 
// store data about Ship DroneBay

class ShipDefense {
  public:
    ShipDefense(const ShipResistances& ship_res, float armor_hp,
                float shield_hp, float hull_hp, float hps);

    virtual ~ShipDefense() = default;

    inline const ShipResistances& ShipRes() const {
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
    Ship(shared_ptr<ShipEngine> engine);

    virtual ~Ship() = default;

    virtual void ApplyEffect(shared_ptr<ShipEffect> effect);

    inline virtual shared_ptr<ShipEngine> Engine() {
      return engine_;
    }

  private:
    shared_ptr<ShipEngine> engine_;
    shared_ptr<ShipEffectsMap> effect_map_;
};

} // namespace eve

#endif // SHIP_H_
#ifndef PLAYER_SHIP_H_
#define PLAYER_SHIP_H_

#include "ship.h"
#include "effects.h"
#include <memory>
#include <vector>
#include <iostream>

namespace eve {

using std::shared_ptr;
using std::vector;

class PlayerShipEngine : public ShipEngine {
  public:
    PlayerShipEngine(float velocity)
        : velocity_(velocity), max_velocity_(velocity) {}

    inline float Velocity() const override {
      return velocity_;
    }

    inline float MaxVelocity() const override {
      return max_velocity_;
    }

    inline void SetVelocity(float new_value) override {
      velocity_ = new_value;
    }

    float velocity_;
    ShipEffectVector effect_vector_{};
    const float max_velocity_;
};

class PlayerShip : public Ship {
  public:
    PlayerShip(shared_ptr<ShipEngine> engine)
        : engine_(engine), effect_map_(this) {}

    virtual shared_ptr<ShipEngine> Engine() override {
      return engine_;
    }

    void ApplyEffect(shared_ptr<ShipEffect> effect) override {
      effect_map_.AddEffect(effect);
    }

  private:
    shared_ptr<ShipEngine> engine_;
    ShipEffectsMap effect_map_;
};
} // namespace eve

#endif // PLAYER_SHIP_H_
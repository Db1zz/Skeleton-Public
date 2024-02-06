#ifndef SHIP_H_
#define SHIP_H_

#include "effects.h"
#include <memory>

namespace eve {

class ShipEffect;

class ShipEngine {
  public:
    virtual ~ShipEngine() = default;

    virtual float Velocity() const = 0;

    virtual float MaxVelocity() const = 0;

    virtual void SetVelocity(float new_value) = 0;
};

class Ship {
  public:
    virtual ~Ship() = default;

    virtual std::shared_ptr<ShipEngine> Engine() = 0;
    virtual void ApplyEffect(std::shared_ptr<ShipEffect> effect) = 0;
};

} // namespace eve

#endif // SHIP_H_
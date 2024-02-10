#ifndef NPC_SHIP_H_
#define NPC_SHIP_H_

#include "ship.h"
#include <memory>

namespace eve {

using std::shared_ptr;

class AIBehavior {
  public:
    AIBehavior(float orbit_range)
        : orbit_range_(orbit_range) {}

    inline float OrbitRange() const {
      return orbit_range_;
    }

  private:
    float orbit_range_;
};

class NpcShip : public Ship {
  public:
    NpcShip(shared_ptr<ShipEngine> engine,
            shared_ptr<ShipCapacitor> capacitor, 
            shared_ptr<ShipTargeting> targeting,
            shared_ptr<ShipDefense> defense,
            shared_ptr<ShipEwarVector<>> ewar,
            shared_ptr<AIBehavior> ai_behavior)
        : Ship(engine, capacitor, targeting, defense, ewar),
          ai_behavior_(ai_behavior) {}

    inline virtual AIBehavior* GetAIBehavior() const {
      return ai_behavior_.get();
    }

  private:
    shared_ptr<AIBehavior> ai_behavior_;
};

} // namespace eve

#endif // NPC_SHIP_H_
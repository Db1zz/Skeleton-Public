#ifndef PLAYER_SHIP_H_
#define PLAYER_SHIP_H_

#include "ship.h"
#include <memory>

namespace eve {

using std::shared_ptr;

class BotBehavior {
  public:
    BotBehavior() = default;

    BotBehavior(float orbit_range)
        : orbit_range_(orbit_range) {}

    inline float GetOrbitRange() const {
      return orbit_range_;
    }

    inline float SetOrbitRange(float new_val) {
      orbit_range_ = new_val;
    }

  protected:
    float orbit_range_{0};
};

class Bot : public Ship, public BotBehavior {
  public:
  Bot(unique_ptr<ShipEngine>& engine,
     unique_ptr<ShipCapacitor>& capacitor,
     unique_ptr<ShipTargeting>& targeting,
     unique_ptr<ShipDefense>& defense,
     vector<shared_ptr<EwarModule>>& ewar_module_list,
     vector<shared_ptr<Weapon>>& weapon_list);

  private:

};

} // namespace eve

#endif // PLAYER_SHIP_H_
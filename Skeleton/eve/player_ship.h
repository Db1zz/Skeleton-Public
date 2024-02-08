#ifndef PLAYER_SHIP_H_
#define PLAYER_SHIP_H_

#include "ship.h"
#include <memory>

namespace eve {

using std::shared_ptr;

class PlayerShip : public Ship {
  public:
    PlayerShip(shared_ptr<ShipEngine> engine)
        : Ship(engine) {}
};

} // namespace eve

#endif // PLAYER_SHIP_H_
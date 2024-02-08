#ifndef NPC_SHIP_H_
#define NPC_SHIP_H_

#include "ship.h"
#include <memory>

namespace eve {

using std::shared_ptr;

class NpcShip : public Ship {
  public:
    NpcShip(shared_ptr<ShipEngine> engine)
        : Ship(engine) {}

  private:
  
};

} // namespace eve

#endif // NPC_SHIP_H_
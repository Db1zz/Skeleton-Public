#ifndef NPC_H_
#define NPC_H_

#include "../eve/ship.h"
#include "../eve/ship_weapon.h"

#include <memory>
#include <vector>
#include <string>

namespace eve {

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::string;

class AIBehavior {
  public:
    AIBehavior(float orbit_range) 
        : orbit_range_(orbit_range) {
      if (orbit_range_ == 0) {
        orbit_range_ = 60000;
      }
    }

    inline float OrbitRange() const {
      return orbit_range_;
    }

  private:
    float orbit_range_;
};

class Npc : public Ship, public AIBehavior {
  public:
    Npc(unique_ptr<ShipEngine>& engine,
        unique_ptr<ShipCapacitor>& capacitor,
        unique_ptr<ShipTargeting>& targeting,
        unique_ptr<ShipDefense>& defense,
        vector<shared_ptr<ShipEwarModule>>& ewar_module_list,
        vector<shared_ptr<Weapon>>& weapon_list,
        float max_orbit_range, const string& name);

    virtual inline const string& GetName() const {
      return name_;
    }

  private:
    const string name_;
};

} // namespace eve

#endif // NPC_H_
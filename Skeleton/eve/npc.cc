#include "npc.h"

namespace eve {

using std::make_unique;

Npc::Npc(unique_ptr<ShipEngine>& engine,
         unique_ptr<ShipCapacitor>& capacitor,
         unique_ptr<ShipTargeting>& targeting,
         unique_ptr<ShipDefense>& defense,
         vector<shared_ptr<EwarModule>>& ewar_module_list,
         vector<shared_ptr<Weapon>>& weapon_list,
         float max_orbit_range, 
         const string& name)
    : Ship(engine, capacitor, targeting, defense, ewar_module_list,
           weapon_list),
      AIBehavior(max_orbit_range),
      name_(name) {}

Npc::Npc(const Ship* ship, float orbit_range, const string& name)
    : Ship(*ship), AIBehavior(orbit_range), name_(name) {} 

Npc::Npc(const Npc& c)
    : Npc(&c, c.orbit_range_, c.name_) {}

shared_ptr<Ship> Npc::Copy() const {
  return make_unique<Npc>(*this);
}

} // namespace eve
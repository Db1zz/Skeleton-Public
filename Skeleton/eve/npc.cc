#include "npc.h"

namespace eve {

Npc::Npc(unique_ptr<ShipEngine>& engine,
         unique_ptr<ShipCapacitor>& capacitor,
         unique_ptr<ShipTargeting>& targeting,
         unique_ptr<ShipDefense>& defense,
         vector<shared_ptr<ShipEwarModule>>& ewar_module_list,
         vector<shared_ptr<Weapon>>& weapon_list,
         float max_orbit_range, const string& name)
    : Ship(engine, capacitor, targeting, defense, ewar_module_list,
            weapon_list),
      AIBehavior(max_orbit_range),
      name_(name) {}

} // namespace eve
#include "bot.h"

namespace abyss {

using namespace eve;

Bot::Bot(unique_ptr<ShipEngine>& engine,
         unique_ptr<ShipCapacitor>& capacitor,
         unique_ptr<ShipTargeting>& targeting,
         unique_ptr<ShipDefense>& defense,
         unique_ptr<eve::ShipHull>& hull,
         vector<shared_ptr<EwarModule>>& ewar_module_list,
         vector<shared_ptr<Weapon>>& weapon_list)
    : Ship(engine, capacitor, targeting, defense, hull,
           ewar_module_list, weapon_list) {}

Bot::Bot(const Bot& copy)
    : Ship(copy.Ship::Copy())

} // namespace abyss
#ifndef NPC_SHIP_H_
#define NPC_SHIP_H_

#include "../eve/ship.h"
#include "../eve/ship_weapon.h"

#include <vector>
#include <string>

namespace abyss {

using std::vector;
using std::string;

class AbyssNpcEngine : public eve::ShipEngine {
  public:
    AbyssNpcEngine(const vector<string>& v);
};

class AbyssNpcTurretWeapon : public eve::TurretWeapon {
  public:
    AbyssNpcTurretWeapon(const vector<string>& v, 
                         const eve::DamageProfile* dmg);
};

class AbyssNpcMissileWeapon : public eve::MissileWeapon {
  public:
    AbyssNpcMissileWeapon(const vector<string>& v,
                          const eve::DamageProfile* dmg);
};

} // namespace eve

#endif // NPC_SHIP_H_
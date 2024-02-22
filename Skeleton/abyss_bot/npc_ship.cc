#include "npc_ship.h"

#include <utility>

namespace abyss {

using std::stof;
using namespace eve;

AbyssNpcEngine::AbyssNpcEngine(const vector<string>& v)
    : ShipEngine(stof(v[40])) {}

AbyssNpcTurretWeapon::AbyssNpcTurretWeapon(const vector<string>& v,
                                           const DamageProfile* dmg)
    : TurretWeapon(stof(v[22]), 0, 1, stof(v[18]), stof(v[19]), 
                   stof(v[20]), stof(v[21]), dmg) {}

AbyssNpcMissileWeapon::AbyssNpcMissileWeapon(const vector<string>& v, 
                                             const DamageProfile* dmg)
    : MissileWeapon(stof(v[33]), 1, stof(v[32]), dmg) {}
}

// 31 * 32 / 33
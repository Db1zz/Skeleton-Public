#include "npc_ship.h"

#include <utility>

namespace abyss {

using std::stof;
using namespace eve;

AbyssNpcEngine::AbyssNpcEngine(const vector<string>& v)
    : ShipEngine(stof(v[40])) {}


// Index | Parameter type
// 18 | Turret Damage Mod
// 19 | Turret Optimal
// 20 | Turret Falloff
// 21 | Turret Tracking
// 22 | Turret Rof
// 23 | Turret Spool Dmg Bouns - Triglavian weapon Spool
// 24 | Turret Dmg Multiplier per cycle
AbyssNpcTurretWeapon::AbyssNpcTurretWeapon(const vector<string>& v,
                                           const DamageProfile* dmg)
    : TurretWeapon(stof(v[22]), 0, 1, stof(v[18]), stof(v[19]), 
                   stof(v[20]), stof(v[21]), dmg, stof(v[24]), stof(v[23])) {}



// Index | Parameter type
// 32 | Missile Dmg Multiplier
// 33 | Missile Rof
AbyssNpcMissileWeapon::AbyssNpcMissileWeapon(const vector<string>& v, 
                                             const DamageProfile* dmg)
    : MissileWeapon(stof(v[33]), 1, stof(v[32]), dmg) {}
}
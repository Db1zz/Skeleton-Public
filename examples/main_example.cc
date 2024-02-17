#include "../Skeleton/eve/player_ship.h"
#include "../Skeleton/eve/effects.h"
#include "../Skeleton/eve/ship_weapon.h"
#include "../Skeleton/abyss_bot/npc_builder.h"
#include "../Skeleton/eve/type_converter.h"
#include "../Skeleton/eve/eve_math.h"
#include "spawns.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <cassert>


// TODO: Threat Score System Calculation
// Description: The system should apply ewar of NPC's
// to the player Ship and check if any of those are killing us
// and calculate a score of a difference between base Ship stats
// and Ship stats that was affected by NPC'c ewar.

// [Spawn][Ewar] -> [PlayerShip]

void CalcThreat() {

}

// Velocity example
// base score - 100(score with no effects)
// 925 - 800 = .12%(change in velocity)
// 100 * .12 = 12(score)

// Lock range example
// Base score - 100
// 72.5km - 50km = .32%
// 100 * .32

// Velocity
// Capacitor
// Lock Range
// Dps Tank
// Dps

using namespace eve;
using namespace abyss;

int main() {
  // Create NPC dictionary

  shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
    {"Blastgrip Tessera", "Embergrip Tessera", 
    "Strikegrip Tessera", "Strikegrip Tessera"});

  // Run Tests Here
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
#include "../Skeleton/third_party/img_reader/reader.h"
#include "../Skeleton/abyss_bot/spawn_evaluator.h"
#include "../Skeleton/eve/effects.h"
#include "../Skeleton/eve/ship_weapon.h"
#include "../Skeleton/abyss_bot/npc_builder.h"
#include "../Skeleton/eve/type_converter.h"
#include "../Skeleton/eve/eve_math.h"
#include "../Skeleton/eve/npc.h"
#include "../Skeleton/abyss_bot/bot.h"
#include "../Skeleton/third_party/wincap/wincap.h"
#include "../Skeleton/third_party/vision/vision_object.h"
#include "spawns.h"
#include "allheaders.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <utility>
#include <cassert>

using std::make_unique;

using namespace eve;
using namespace abyss;

int main() {
  std::cout << "Start \n";

  // Build projected ship
  // unique_ptr<ShipEngine> engine = 
  //     make_unique<ShipEngine>(925);
  // unique_ptr<ShipCapacitor> capacitor = 
  //     make_unique<ShipCapacitor>(40.8, 4300);
  // unique_ptr<ShipTargeting> targeting = 
  //     make_unique<ShipTargeting>(72500);

  // ShipResistances ship_res(  
  //   ResistanceProfile(.519, .705, .778, .815),
  //   ResistanceProfile(.278, .532, .363, .235),
  //   ResistanceProfile(.25, .25, .25, .25));
  
  // unique_ptr<ShipDefense> defense = make_unique<ShipDefense>(ship_res, 2750, 4000, 3110, 152.0);

  // vector<shared_ptr<EwarModule>> ewar_module_list;

  // DamageProfile dmg = {0, 0, 0, 0};

  // shared_ptr<MissileWeapon> wpn = 
  //     make_unique<MissileWeapon>(4, 0, 6, &dmg);

  // DamageProfile missile_dmg = {0, 0, 600, 0};

  // shared_ptr<MissileAmmo> missile = 
  //     make_unique<MissileAmmo>(10000, 20, &missile_dmg);

  // wpn->LoadAmmo(missile);

  // vector<shared_ptr<Weapon>> weapon_list;

  // weapon_list.push_back(wpn);
  
  // unique_ptr<ShipHull> hull = make_unique<ShipHull>(152.0f, 4.0f, 10.0f);

  // shared_ptr<eve::Ship> ship_base = 
  //   make_unique<eve::Ship>(engine, capacitor, targeting, defense, hull,
  //                          ewar_module_list, weapon_list);

  // shared_ptr<abyss::BotBehavior> bot_behavior 
  //     = make_unique<abyss::BotBehavior>(18000);

  // shared_ptr<abyss::Bot> projected_bot 
  //     = make_unique<abyss::Bot>(ship_base, bot_behavior);

  // shared_ptr<NpcContainer> NPC_DICTIONARY = AbyssNpcBuilder::Build();

  // Create NPC spawn
  // shared_ptr<NpcContainer> spawn = spawns::BuildSpawn(
  //   {"Renewing Rodiva", "Renewing Rodiva", 
  //    "Renewing Rodiva", "Renewing Rodiva",
  //    "Spotlighter Tessella", "Spotlighter Tessella",
  //    "Spotlighter Tessella", "Spotlighter Tessella",
  //    "Sparklance Tessella", "Strikelance Tessella",
  //    "Blastlance Tessella", "Blastlance Tessella",
  //    "Blastlance Tessella", "Blastlance Tessella"},
  //   NPC_DICTIONARY);

  // shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
  //   {"Lucifer Fury", "Lucifer Fury",
  //    "Lucifer Ixion", "Lucifer Ixion", 
  //    "Elite Lucifer Cynabal", "Elite Lucifer Cynabal",
  //    "Elite Lucifer Cynabal", "Elite Lucifer Cynabal",
  //    "Lucifer Cynabal", "Lucifer Cynabal"},
  //   NPC_DICTIONARY);

  // shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
  // {"Attacker Marshal Disparu Troop", "Attacker Marshal Disparu Troop",
  // "Marker Enforcer Disparu Troop","Assault Enforcer Disparu Troop",
  // "Marker Pacifier Disparu Troop", "Marker Pacifier Disparu Troop",
  // "Arrester Pacifier Disparu Troop", "Arrester Pacifier Disparu Troop"}, 
  // NPC_DICTIONARY);

  // shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
  //   {"Starving Vedmak", "Starving Vedmak",
  //    "Starving Vedmak", "Ghosting Damavik",
  //    "Ghosting Damavik", "Ghosting Damavik",
  //    "Ghosting Damavik", "Starving Damavik",
  //    "Tangling Damavik", "Starving Vedmak",
  //    "Tangling Damavik", "Tangling Damavik"},
  //   NPC_DICTIONARY);

// shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
//   {"Renewing Rodiva", "Renewing Rodiva", 
//   "Renewing Rodiva", "Tangling Kikimora",
//   "Tangling Kikimora", "Ghosting Kikimora",
//   "Ghosting Kikimora", "Tangling Damavik",
//   "Striking Kikimora"},
//   NPC_DICTIONARY);

// shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
//     {"Renewing Leshak", "Renewing Leshak",
//      "Renewing Leshak", "Renewing Leshak",
//      "Starving Leshak", "Blinding Leshak",
//      "Snarecaster Tessella"},
//     NPC_DICTIONARY);

// shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
//     {"Blinding Leshak", "Blinding Leshak",
//      "Tangling Leshak", "Blinding Leshak",
//      "Starving Leshak", "Blinding Leshak",
//      "Snarecaster Tessella"},
//     NPC_DICTIONARY);

// shared_ptr<NpcContainer> spawn = spawns::BuildSpawn(
//   {"Ephialtes Confuser", "Ephialtes Illuminator",
//    "Ephialtes Illuminator", "Ephialtes Illuminator",
//    "Ephialtes Spearfisher", "Ephialtes Spearfisher",
//    "Ephialtes Entangler", "Ephialtes Entangler",
//    "Ephialtes Entangler", "Drifter Assault Battleship"},
//    NPC_DICTIONARY);

// shared_ptr<NpcContainer> spawn =  spawns::BuildSpawn(
//     {"Arrester Pacifier Disparu Troop"}, NPC_DICTIONARY);

// SpawnEvaluator spawn_evaulator;
// spawn_evaulator.EvaulateSpawn(spawn, projected_bot);

  // string slash_img = "/Users/georgiyonischenko/Desktop/Screenshots/Screenshot 2024-02-28 at 00.43.55.png";

  // string slash_field_img = "/Users/georgiyonischenko/Desktop/Screenshots/Screenshot 2024-02-28 at 00.43.58.png";

  // VisionObject slash_field(slash_field_img);
  // VisionObject slash(slash_img);
  // Wincap wincap;

  // cv::Mat img = wincap.GetDesktopScreenshot(0, 0, 800, 1080);

  // vector<cv::Rect> find_slashes_fields = slash_field.FindObjectOnImage(img);
  // vector<cv::Rect> result 
  //     = slash.FindObjectOnImageWithinRoi(img, find_slashes_fields);
  // slash.HighlightRectangles(img, result);

  // imshow("test", img);
  // cv::waitKey(0);
  
  //
  // //a
  //
  //
  //
  // //
  ////

  // Run Tests Here
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();

  std::cout << "End \n";
}
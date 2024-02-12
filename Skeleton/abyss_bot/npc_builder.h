#ifndef NPC_BUILDER_H_
#define NPC_BUILDER_H_

#include "../db/db_loader.h"
#include "../eve/npc.h"
#include "../eve/type_converter.h"
#include "../eve/npc_container.h"
#include "npc_ship.h"

#include <iostream>
#include <utility>


namespace abyss {

using std::vector;
using std::string;
using std::make_unique;
using std::move;
using std::stof;
using namespace eve;
using namespace converter;

class AbyssNpcBuilder {
  public:
    unique_ptr<ShipEngine> BuildEngine(const vector<string>& s) { 
      return make_unique<ShipEngine>(stof(s[40]));
    }

    unique_ptr<ShipCapacitor> BuildCapacitor(const vector<string>& s) {
      return make_unique<ShipCapacitor>(0, 0);
    }

    unique_ptr<ShipTargeting> BuildTargeting(const vector<string>& s) {
      return make_unique<ShipTargeting>(100000000);
    }

    unique_ptr<ShipDefense> BuildDefense(const vector<string>& s) {
      ShipResistances res = {
        ResistanceProfile(stof(s[3]), stof(s[4]), stof(s[5]), stof(s[6])),
        ResistanceProfile(stof(s[7]), stof(s[8]), stof(s[9]), stof(s[10])),
        ResistanceProfile(stof(s[11]), stof(s[12]), stof(s[13]), stof(s[14]))
      };

      unique_ptr<ShipDefense> defense = make_unique<ShipDefense>(
        res, stof(s[16]), stof(s[15]), stof(s[17]), stof(s[67]));
      return defense;
    }

    shared_ptr<ShipEffect> BuildEffect(
        const vector<string>& s, int type_index, int str_index) 
    {
      return make_unique<ShipEffect>(
          StrToType(s[type_index]), stof(s[str_index]), s[0]);
    }

    shared_ptr<ShipEwarModule> BuildEwarModule(
        const vector<string>& s, int type_index, 
        const shared_ptr<ShipEffect>& effect) const 
    {
      return make_unique<ShipEwarModule>(
          stof(s[type_index+3]), stof(s[type_index+4]), 
          stof(s[type_index+2]), effect);
    }

    shared_ptr<ShipEwarModule> BuildRemoteRepairModule(
        const vector<string>& s, const shared_ptr<ShipEffect>& effect) {
      return make_unique<ShipEwarModule>(
        stof(s[47]), stof(s[48]), stof(s[49]), effect);
    }

    shared_ptr<NpcContainer> Build() {
      db::CsvDatabaseParser db("/Users/georgiyonischenko/Desktop/Programming/Skeleton/Skeleton/abyss_bot/csv/encoded-AbyssNpc_database.csv");
      db.Connect();

      NpcContainer npc_dictionary;

      vector<vector<string>> parsed_db = db.ParseCsv();
      for (const auto& line : parsed_db) {
        vector<shared_ptr<ShipEwarModule>> ewar_module_list;
        if (line[53][0] != '0') {
          ewar_module_list.push_back(
              BuildEwarModule(line, 53, BuildEffect(line, 53, 54)));
        } 

        if (line[58][0] != '0') {
          ewar_module_list.push_back(
              BuildEwarModule(line, 58, BuildEffect(line, 58, 59)));
        }

        if (line[45][0] != '0') {
          ewar_module_list.push_back(
              BuildRemoteRepairModule(line, BuildEffect(line, 45, 66)));
        }

        vector<shared_ptr<Weapon>> weapon_list{};
        if (line[64][0] != '0') {
          DamageProfile dmg(TurretStrToDmgProfile(line, 26));
          weapon_list.push_back(make_unique<AbyssNpcTurretWeapon>(line, &dmg));
        }

        if (line[69][0] != '0') {
          DamageProfile dmg(MissileStrToDmgProfile(line[31]));
          weapon_list.push_back(make_unique<AbyssNpcMissileWeapon>(line, &dmg));
        }

        float orbit_range = stof(line[25]);
        string name = line[0];

        unique_ptr<ShipEngine> engine(BuildEngine(line));
        unique_ptr<ShipCapacitor> capacitor(BuildCapacitor(line));
        unique_ptr<ShipTargeting> targeting(BuildTargeting(line));
        unique_ptr<ShipDefense> defense(BuildDefense(line));

        shared_ptr<Npc> npc = make_unique<Npc>(
            engine, capacitor, targeting, defense, ewar_module_list, 
            weapon_list, orbit_range, name);

        npc_dictionary.AddNpc(npc);
    }
    return make_unique<NpcContainer>(npc_dictionary);
  }
};
} // namespace abyss

#endif // NPC_BUILDER_H_
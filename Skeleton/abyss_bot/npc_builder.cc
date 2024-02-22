#include "npc_builder.h"

namespace abyss {

using std::make_unique;
using std::stof;
using namespace converter;

unique_ptr<ShipEngine> AbyssNpcBuilder::BuildEngine(const vector<string>& s) { 
  return make_unique<ShipEngine>(stof(s[38]));
}

unique_ptr<ShipCapacitor> AbyssNpcBuilder::BuildCapacitor(
    const vector<string>& s) {
  return make_unique<ShipCapacitor>(0, 0);
}

unique_ptr<ShipTargeting> AbyssNpcBuilder::BuildTargeting(
    const vector<string>& s) {
  return make_unique<ShipTargeting>(100000000);
}

unique_ptr<ShipDefense> AbyssNpcBuilder::BuildDefense(const vector<string>& s) {
  ShipResistances res = {
    ResistanceProfile(stof(s[3]), stof(s[4]), stof(s[5]), stof(s[6])),
    ResistanceProfile(stof(s[7]), stof(s[8]), stof(s[9]), stof(s[10])),
    ResistanceProfile(stof(s[11]), stof(s[12]), stof(s[13]), stof(s[14]))
  };

  float shield_hp = stof(s[15]) - stof(s[42]);
  float armor_hp = stof(s[16]) - stof(s[41]);
  float hull_hp = stof(s[17]) - stof(s[43]);

  unique_ptr<ShipDefense> defense = make_unique<ShipDefense>(
    res, armor_hp, shield_hp, hull_hp, stof(s[67]));
  return defense;
}

shared_ptr<EwarModule> AbyssNpcBuilder::BuildEwarModule
    (const vector<string>& s, int type_index) {
  shared_ptr<EwarModule> result;
  string source = s[0];
  EwarModule::Type type = StrToType(s[type_index]);
  float opt = stof(s[type_index+3]);
  float fal = stof(s[type_index+4]);
  float rof = stof(s[type_index+2]);
  vector<shared_ptr<Effect>> effects;

  if (type == EwarModule::Type::EnergyNeutralizer) {
    float strength = stof(s[type_index+1]);
    effects.push_back(
      make_unique<EwarCapacitorRegenDecreaseEffect>(source, strength / rof));

  } else if (type == EwarModule::Type::StasisWebifier) {
    float strength = stof(s[type_index+1]);
    effects.push_back(
      make_unique<EwarVelocityDecreaseEffect>(source, strength));

  } else if (type == EwarModule::Type::SensorDampener) {
    float strength = stof(s[type_index+1]);
    effects.push_back(
      make_unique<EwarLockRangeDecreaseEffect>(source, strength));
  }

  return make_unique<EwarModule>(source, type, opt, fal, rof, effects);
}

shared_ptr<EwarModule> AbyssNpcBuilder::BuildRemoteRepairModule(
    const vector<string>& s) {
  vector<shared_ptr<Effect>> effects;
  string source = s[0];
  EwarModule::Type type = EwarModule::Type::RemoteRepair;
  float strength = stof(s[66]);
  float opt = stof(s[47]);
  float fal = stof(s[48]);
  float rof = stof(s[49]);

  effects.push_back(
    make_unique<EwarArmorRestorationEffect>(source, strength));

  return make_unique<EwarModule>(source, type, opt, fal, rof, effects);
}

shared_ptr<NpcContainer> AbyssNpcBuilder::Build() {
  db::CsvDatabaseParser db("/Users/georgiyonischenko/Desktop/Programming/Skeleton/Skeleton/abyss_bot/csv/encoded-AbyssNpc_database.csv");
  db.Connect();

  NpcContainer npc_dictionary;

  vector<vector<string>> parsed_db = db.ParseCsv();
  for (const auto& line : parsed_db) {
    vector<shared_ptr<EwarModule>> ewar_module_list;
    if (line[53][0] != '0') {
      ewar_module_list.push_back(BuildEwarModule(line, 53));
    } 

    if (line[58][0] != '0') {
      ewar_module_list.push_back(BuildEwarModule(line, 58));
    }

    if (line[45][0] != '0') {
      ewar_module_list.push_back(BuildRemoteRepairModule(line));
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

} // namespace abyss
#include "type_converter.h"
#include "iostream"

#include <unordered_map>

namespace eve {
namespace converter {

static const std::unordered_map<string, EwarModule::Type> kStrEffectMap = {
  {"Energy Neutralizer", EwarModule::Type::EnergyNeutralizer},
  {"Tracking Disruptor", EwarModule::Type::WeaponDisruptor},
  {"Guidance Disruptor", EwarModule::Type::GuidanceDisruptor},
  {"Stasis Webifier", EwarModule::Type::StasisWebifier},
  {"Warp Scrambler", EwarModule::Type::WarpScrambler},
  {"Target Painter", EwarModule::Type::TargetPainter},
  {"Sensor Dampener", EwarModule::Type::SensorDampener},
  {"Armor", EwarModule::Type::RemoteRepair},
  {"Shield", EwarModule::Type::RemoteRepair}
};

// TODO: implement HullType class|enum 
// static const std::unordered_map<string, HullType> kStrHullMap = {
//   {"battleship", HullType::Battleship},
//   {"battlecruiser", HullType::Battlecruiser},
//   {"cruiser", HullType::Cruiser},
//   {"destroyer", HullType::Destroyer},
//   {"frigate", HullType::Frigate}
// };

EwarModule::Type StrToType(const string &type) {
  auto type_it = kStrEffectMap.find(type);
  if (type_it == kStrEffectMap.end()) {
    return EwarModule::Type::None;
  } 
  return type_it->second;
}

EwarModule::Type StrToEwar(const vector<string> &csv_lines, int index) {
  return StrToType(csv_lines[index]);
}

// TODO: implement HullType class|enum 
// HullType StrToHull(const string &type) {
//   auto type_it = kStrHullMap.find(type);
//     if (type_it == kStrHullMap.end()) {
//       std::cout << "Error: cannot convert StrToHull" << type << '\n';
//       exit(-1);
//   } 
//   return type_it->second;
// }

float StrToFloat(const string &type) {
  return std::stof(type);
}

float StrToFloat(const vector<string> &csv_lines, int index) {
  return std::stof(csv_lines[index]);
}

DamageProfile TurretStrToDmgProfile(const vector<string> &csv_line, int start) {
  DamageProfile res = {
    StrToFloat(csv_line, start),
    StrToFloat(csv_line, start + 1),
    StrToFloat(csv_line, start + 2),
    StrToFloat(csv_line, start + 3),
  };
  return res;
}

DamageProfile MissileStrToDmgProfile(const string& str) {
  DamageProfile result;
  const static vector<string> types{"EM", "THE", "KIN", "EXP"};

  for (const auto& type : types) {
    size_t found = str.find(type);
    if (found != string::npos) {
      string damage;
      for (int i = found - 1; i >= 0; i--) {
        if (damage.size() > 0 && str[i] == ' ' || 
                                 str[i] == '|') {
          break;
        }
        if (str[i] == ' ') continue;
        
        damage.insert(damage.begin() + 0, str[i]);
      }
      if (type == types[0]) {
        result.SetEm(std::stof(damage));
      } else if (type == types[1]) {
        result.SetThermal(std::stof(damage));
      } else if (type == types[2]) {
        result.SetKinetic(std::stof(damage));
      } else if (type == types[3]) {
        result.SetExplosive(std::stof(damage));
      }
    }
  }
  return result;
}

ResistanceProfile StrToResProfile(const vector<string> &csv_line,
                                  int start_index) {
  ResistanceProfile res = {
    StrToFloat(csv_line, start_index),
    StrToFloat(csv_line, start_index + 1),
    StrToFloat(csv_line, start_index + 2),
    StrToFloat(csv_line, start_index + 3),
  };
  return res;
}

}  // namespace converter
}  // namespace eve
#include "type_converter.h"
#include "iostream"

namespace eve {
namespace converter {

static const std::unordered_map<string, ShipEffect::Type> kStrEffectMap = {
  {"Energy Neutralizer", ShipEffect::Type::EnergyNeutralizer},
  {"Tracking Disruptor", ShipEffect::Type::WeaponDisruptor},
  {"Guidance Disruptor", ShipEffect::Type::GuidanceDisruptor},
  {"Stasis Webifier", ShipEffect::Type::StasisWebifier},
  {"Warp Scrambler", ShipEffect::Type::WarpScrambler},
  {"Target Painter", ShipEffect::Type::TargetPainter},
  {"Sensor Dampener", ShipEffect::Type::SensorDampener},
  {"Armor", ShipEffect::Type::RemoteRepair},
  {"Shield", ShipEffect::Type::RemoteRepair}
};

// TODO: implement HullType class|enum 
// static const std::unordered_map<string, HullType> kStrHullMap = {
//   {"battleship", HullType::Battleship},
//   {"battlecruiser", HullType::Battlecruiser},
//   {"cruiser", HullType::Cruiser},
//   {"destroyer", HullType::Destroyer},
//   {"frigate", HullType::Frigate}
// };

ShipEffect::Type StrToEwar(const string &type) {
  auto type_it = kStrEffectMap.find(type);
  if (type_it == kStrEffectMap.end()) {
    return ShipEffect::Type::None;
  } 
  return type_it->second;
}

ShipEffect::Type StrToEwar(vector<string> &csv_lines, int index) {
  return StrToEwar(csv_lines[index]);
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

float StrToFloat(vector<string> &csv_lines, int index) {
  return std::stof(csv_lines[index]);
}

DamageProfile StrToDmgProfile(vector<string> &csv_line, int start) {
  DamageProfile res = {
    StrToFloat(csv_line, start),
    StrToFloat(csv_line, start + 1),
    StrToFloat(csv_line, start + 2),
    StrToFloat(csv_line, start + 3),
  };
  return res;
}

ResistanceProfile StrToResProfile(vector<string> &csv_line, int start_index) {
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
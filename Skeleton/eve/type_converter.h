#ifndef TYPE_CONVERTER_H_
#define TYPE_CONVERTER_H_

#include "hp_resistances.h"
#include "ewar_modules.h"

#include <string>

namespace eve {
namespace converter {

using std::vector;
using std::string;
using std::shared_ptr;

EwarModule::Type StrToType(const string &type);

EwarModule::Type StrToEwar(const vector<string> &csv_lines, int index);

// HullType StrToHull(const string &type);

float StrToFloat(const string &type);

float StrToFloat(const vector<string> &csv_lines, int index);

DamageProfile TurretStrToDmgProfile(const vector<string> &csv_line, int start);

DamageProfile MissileStrToDmgProfile(const string& str);

ResistanceProfile StrToResProfile(const vector<string> &csv_line, int start);

}  // namespace converter
}  // namespace eve

#endif  // TYPE_CONVERTER_H_
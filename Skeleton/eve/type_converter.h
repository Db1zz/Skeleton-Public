#ifndef TYPE_CONVERTER_H_
#define TYPE_CONVERTER_H_

#include "hp_resistances.h"
#include "effects.h"

#include <string>

namespace eve {
namespace converter {

using std::vector;
using std::string;
using std::shared_ptr;

ShipEffect::Type StrToEwar(const string &type);

ShipEffect::Type StrToEwar(vector<string> &csv_lines, int index);

// HullType StrToHull(const string &type);

float StrToFloat(const string &type);

float StrToFloat(vector<string> &csv_lines, int index);

DamageProfile StrToDmgProfile(vector<string> &csv_line, int start);

ResistanceProfile StrToResProfile(vector<string> &csv_line, int start);

}  // namespace converter
}  // namespace eve

#endif  // TYPE_CONVERTER_H_
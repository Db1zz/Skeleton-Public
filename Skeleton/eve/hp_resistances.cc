#include "hp_resistances.h"

namespace eve {

ResistanceProfile::ResistanceProfile(float em, float thermal, 
                                     float kinetic, float explosive)
    : resistances_{GetValidValue(em), GetValidValue(thermal), 
                   GetValidValue(kinetic), GetValidValue(explosive)} {}

ResistanceProfile::ResistanceProfile(const ResistanceProfile& c)
    : ResistanceProfile(*c.em_, *c.thermal_, *c.kinetic_, *c.explosive_) {}

float ResistanceProfile::GetValidValue(float value) {
  if (value > 1) {
    return GetValidValue(value / 10);
  }
  return value;
}

ShipResistances::ShipResistances(ResistanceProfile& shield, 
                                 ResistanceProfile& armor, 
                                 ResistanceProfile& hull)
    : profiles_{shield, armor, hull},
      shield_(&profiles_[0]),
      armor_(&profiles_[1]),
      hull_(&profiles_[2]) {}

ShipResistances::ShipResistances(ResistanceProfile&& shield,
                                 ResistanceProfile&& armor,
                                 ResistanceProfile&& hull)
    : ShipResistances(shield, armor, hull) {}

ShipResistances::ShipResistances(const ShipResistances& copy)
    : ShipResistances(*copy.shield_, *copy.armor_, *copy.hull_) {}

DamageProfile::DamageProfile(float em, float thermal, 
                             float kinetic, float explosive)
    : dmg_types_{em, thermal, kinetic, explosive} {}
    
} // namespace eve
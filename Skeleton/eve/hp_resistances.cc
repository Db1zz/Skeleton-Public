#include "hp_resistances.h"

namespace eve {

DamageProfile::DamageProfile()
    : dmg_types_{0, 0, 0, 0} {}

DamageProfile::DamageProfile(float em, float thermal, 
                             float kinetic, float explosive)
    : dmg_types_{em, thermal, kinetic, explosive} {}

DamageProfile::DamageProfile(const DamageProfile& copy)
    : DamageProfile(copy.Em(), copy.Thermal(), 
                    copy.Kinetic(), copy.Explosive()) {}

ResistanceProfile::ResistanceProfile(float em, float thermal, 
                                     float kinetic, float explosive)
    : DamageProfile(GetValidValue(em), GetValidValue(thermal), 
                    GetValidValue(kinetic), GetValidValue(explosive)) {}

ResistanceProfile::ResistanceProfile(const ResistanceProfile& c)
    : ResistanceProfile(c.Em(), c.Thermal(), c.Kinetic(), c.Explosive()) {}

float ResistanceProfile::GetValidValue(float value) {
  if (value >= 1) {
    return GetValidValue(value / 10);
  }
  return value;
}

ShipResistances::ShipResistances(ResistanceProfile& shield, 
                                 ResistanceProfile& armor, 
                                 ResistanceProfile& hull)
    : profiles_{shield, armor, hull} {}

ShipResistances::ShipResistances(ResistanceProfile&& shield,
                                 ResistanceProfile&& armor,
                                 ResistanceProfile&& hull)
    : ShipResistances(shield, armor, hull) {}

ShipResistances::ShipResistances(const ShipResistances& copy)
    : ShipResistances(copy.Shield(), copy.Armor(), copy.Hull()) {}
    
} // namespace eve
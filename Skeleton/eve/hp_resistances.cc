#include "hp_resistances.h"

namespace eve {

ResistanceProfile::ResistanceProfile(float em, float thermal, 
                                     float kinetic, float explosive)
    : em_(GetValidValue(em)), 
      thermal_(GetValidValue(thermal)), 
      kinetic_(GetValidValue(kinetic)), 
      explosive_(GetValidValue(explosive)) {}

DamageProfile::DamageProfile(float em, float thermal, 
                             float kinetic, float explosive)
    : em_(em), 
      thermal_(thermal), 
      kinetic_(kinetic), 
      explosive_(explosive) {}

} // namespace eve
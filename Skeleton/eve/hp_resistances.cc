#include "hp_resistances.h"

namespace eve {

ResistanceProfile::ResistanceProfile(float em, float thermal, 
                                     float kinetic, float explosive)
    : em_(em), 
      thermal_(thermal), 
      kinetic_(kinetic), 
      explosive_(explosive) {
  IsValidValues();
}

DamageProfile::DamageProfile(float em, float thermal, 
                             float kinetic, float explosive)
    : em_(em), 
      thermal_(thermal), 
      kinetic_(kinetic), 
      explosive_(explosive) {}

} // namespace eve
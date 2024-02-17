// #include "../Skeleton/eve/ship.h"
// #include "../Skeleton/eve/effects.h"
#include "../Skeleton/eve/effects.h"
#include "../Skeleton/eve/ship.h"

#include <memory.h>
#include <gtest/gtest.h>

using namespace eve;

using std::make_unique;

#define STASIS_WEB_EFFECT(str)(make_unique<EwarVelocityDecreaseEffect> ("NONE", str))

class ShipCreator {
  public:
    ShipCreator() = default;

    static shared_ptr<Ship> Create() {
      unique_ptr<ShipEngine> engine = make_unique<ShipEngine>(925);

      unique_ptr<ShipCapacitor> capacitor = 
          make_unique<ShipCapacitor>(40.8,4030);

      unique_ptr<ShipTargeting> targeting = 
          make_unique<ShipTargeting>(72500);

      ShipResistances res(
        ResistanceProfile(0.519, 0.705, 0.778, 0.815),
        ResistanceProfile(0.278, 0.532, 0.363, 0.235),
        ResistanceProfile(0.317, 0.598, 0.598, 0.598)
      );

      unique_ptr<ShipDefense> defense = 
          make_unique<ShipDefense>(res, 2750, 4000, 3110, 152);
      

      vector<shared_ptr<EwarModule>> ewar_module_list;
      vector<shared_ptr<Weapon>> weapon_list;

      shared_ptr<Ship> ship = 
          make_unique<Ship>(engine, capacitor, targeting, defense, 
                            ewar_module_list, weapon_list);

      return ship;
    }
};

TEST(ApplyEwarVelocityDecreaseEffect, HandlesEwarWebEffect) {
  shared_ptr<Ship> ship_smart_ptr = ShipCreator().Create();
  Ship* ship_ptr = ship_smart_ptr.get();
  EffectManager manager(EffectType::EwarVelocityDecreaseEffect, ship_ptr);
  manager.ApplyEffect(STASIS_WEB_EFFECT(50));
  EXPECT_EQ(RoundF(ship_ptr->Engine()->Velocity()), RoundF(462.5));
  manager.ApplyEffect(STASIS_WEB_EFFECT(60)); 
  EXPECT_EQ(RoundF(ship_ptr->Engine()->Velocity()), RoundF(209.2));
  manager.ApplyEffect(STASIS_WEB_EFFECT(55));
  EXPECT_EQ(RoundF(ship_ptr->Engine()->Velocity()), RoundF(138.0));
}

TEST(RemoveEwarVelocityDecreaseEffect, HandlesEwarWebEffect) {
  shared_ptr<Ship> ship_smart_ptr = ShipCreator().Create();
  Ship* ship_ptr = ship_smart_ptr.get();
  EffectManager manager(EffectType::EwarVelocityDecreaseEffect, ship_ptr);
  manager.ApplyEffect(STASIS_WEB_EFFECT(50));
  manager.ApplyEffect(STASIS_WEB_EFFECT(60));
  manager.ApplyEffect(STASIS_WEB_EFFECT(55));
  manager.RemoveEffect(STASIS_WEB_EFFECT(60).get());
  EXPECT_EQ(RoundF(ship_ptr->Engine()->Velocity()), RoundF(235.4));
  manager.RemoveEffect(STASIS_WEB_EFFECT(55).get());
  EXPECT_EQ(RoundF(ship_ptr->Engine()->Velocity()), RoundF(462.5));
  manager.RemoveEffect(STASIS_WEB_EFFECT(50).get());
  EXPECT_EQ(RoundF(ship_ptr->Engine()->Velocity()), RoundF(925));
}

TEST(ApplyEwarCapacitorRegenDecreaseEffect, HandlesNeutEffect) {

}
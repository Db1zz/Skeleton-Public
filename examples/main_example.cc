#include "../Skeleton/eve/player_ship.h"
#include "../Skeleton/eve/effects.h"
#include "../Skeleton/eve/ship_weapon.h"
#include <iostream>
#include <memory>

using namespace eve;

void StackingPenaltyCalculationTest() {
  shared_ptr<ShipEngine> e = std::make_unique<ShipEngine>(925);
  PlayerShip ship(e);

  ShipEffect effect1(ShipEffect::StasisWebifier, 50, "None1");
  ShipEffect effect2(ShipEffect::StasisWebifier, 55, "None2");
  ShipEffect effect3(ShipEffect::StasisWebifier, 60, "None3");
  std::cout << "Normal velocity: " << e.get()->Velocity() << '\n';
  ship.ApplyEffect(std::make_unique<ShipEffect>(effect1));
  std::cout << "Applied 50% effect: " << e.get()->Velocity() << '\n';
  ship.ApplyEffect(std::make_unique<ShipEffect>(effect2));
  std::cout << "Applied 55% effect: " << e.get()->Velocity() << '\n';
  ship.ApplyEffect(std::make_unique<ShipEffect>(effect3));
  std::cout << "Applied 60% effect: " << e.get()->Velocity() << '\n';
}

void DpsCalculationTest() {
  ResistanceProfile res(0.0, 0.0, 0.0, 0.0);

  DamageProfile base_dmg(0, 0, 0, 0);
  DamageProfile dmg_profile(0, 12, 16.8, 0);
  shared_ptr<TurretAmmo> ammo = std::make_unique<TurretAmmo>(
                                    0.75, 0.50, 1.0, &dmg_profile);
  TurretWeapon weapon(4.19, 0, 5, 13.2, 18000, 225000, 11.3, &base_dmg);
  weapon.LoadAmmo(ammo);
  std::cout << "Dps: " << weapon.Dps(&res) << '\n';
}

int main() {
  
  // StackingPenaltyCalculationTest();
  DpsCalculationTest();
}
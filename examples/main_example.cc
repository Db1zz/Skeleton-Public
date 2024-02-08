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
  ResistanceProfile res(0, 0, 0.80, 0);
  DamageProfile dmg_profile(0, 0, 239, 0);
  shared_ptr<Missile> missile = std::make_unique<Missile>(0, 0, &dmg_profile);
  MissileLauncher weapon(3.65, 35.0, 4);
  weapon.LoadAmmo(missile);
  std::cout << "Dps: " << weapon.Dps(&res) << '\n';
}

int main() {
  
  // StackingPenaltyCalculationTest();
  // DpsCalculationTest();

  float bonus = 0.25;
  float c = 100;
  c = c * (1 + bonus);
  std::cout << c << std::endl;
  c = c / (1 + bonus);
  std::cout << c << std::endl;
  return 0;
}
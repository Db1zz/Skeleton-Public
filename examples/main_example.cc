#include "../Skeleton/eve/player_ship.h"
#include <iostream>
#include <memory>

using namespace eve;

int main() {
  shared_ptr<PlayerShipEngine> e = std::make_unique<PlayerShipEngine>(925);
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

  return 0;
}
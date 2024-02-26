#include "bot.h"
#include "../eve/ship.h"

namespace abyss {

using std::make_unique;

using namespace eve;

BotBehavior::BotBehavior(float orbit_range)
    : orbit_range_(orbit_range) {}

BotBehavior::BotBehavior(const BotBehavior& copy)
    : orbit_range_(copy.GetOrbitRange()) {}

shared_ptr<BotBehavior> BotBehavior::Copy() const {
  return make_unique<BotBehavior>(*this);
}

Bot::Bot(shared_ptr<eve::Ship>& ship, shared_ptr<BotBehavior>& behavior)
    : ship_(ship), bot_behavior_(behavior) {}

Bot::Bot(Bot& copy)
    : ship_(copy.Ship()->Copy()),
      bot_behavior_(copy.Behavior()->Copy()) {}

shared_ptr<Bot> Bot::Copy() {
  return make_unique<Bot>(*this);
}

} // namespace abyss
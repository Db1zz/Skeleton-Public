#ifndef BOT_H_
#define BOT_H_

#include "../eve/ship.h"
#include <memory>
#include <vector>

namespace abyss {

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class BotBehavior {
  public:
    BotBehavior() = default;

    BotBehavior(float orbit_range)
        : orbit_range_(orbit_range) {}

    inline float GetOrbitRange() const {
      return orbit_range_;
    }

    inline float SetOrbitRange(float new_val) {
      orbit_range_ = new_val;
    }

  protected:
    float orbit_range_{0};
};

class Bot {
  public:
    Bot(shared_ptr<eve::Ship>& ship, );
    
    Bot(const Bot& copy);

    virtual shared_ptr<Bot> Copy() const {

    }

    inline shared_ptr<eve::Ship> Ship() const {
      return ship_;
    }

  private:
    shared_ptr<eve::Ship> ship_;
};

} // namespace abyss

#endif // BOT_H_
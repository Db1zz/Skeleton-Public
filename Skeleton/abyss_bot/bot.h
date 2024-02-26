#ifndef BOT_H_
#define BOT_H_

#include <memory>
#include <vector>

namespace eve {
  class Ship;
}

namespace abyss {

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class BotBehavior {
  public:
    BotBehavior() = default;

    BotBehavior(float orbit_range);

    BotBehavior(const BotBehavior& copy);

    virtual shared_ptr<BotBehavior> Copy() const;

    inline float GetOrbitRange() const {
      return orbit_range_;
    }

    inline void SetOrbitRange(float new_val) {
      orbit_range_ = new_val;
    }

  protected:
    float orbit_range_{0};
};

class Bot {
  public:
    Bot(shared_ptr<eve::Ship>& ship, shared_ptr<BotBehavior>& behavior);
    
    Bot(Bot& copy);

    virtual shared_ptr<Bot> Copy();

    inline shared_ptr<eve::Ship>& Ship() {
      return ship_;
    }

    shared_ptr<BotBehavior>& Behavior() {
      return bot_behavior_;
    }

  private:
    shared_ptr<eve::Ship> ship_;
    shared_ptr<BotBehavior> bot_behavior_;
};

} // namespace abyss

#endif // BOT_H_
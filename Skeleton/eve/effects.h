#ifndef EFFECTS_H_
#define EFFECTS_H_

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <initializer_list>

namespace eve {

using std::unordered_map;

class ShipEffect {
  public:
    enum Type {
      StasisWebifier,
    };

    ShipEffect(Type type, std::string source, float strength);

    inline virtual Type GetType() const {
      return type_;
    }

    inline virtual float GetStrength() const {
      return strength_;
    }

    inline virtual std::string GetSource() const  {
      return source_;
    }

  private:
    Type type_;
    std::string source_;
    float strength_;
};

class ShipEffectVector {
  public:
    ShipEffectVector(std::initializer_list<std::shared_ptr<ShipEffect>> list)
        : effect_array_(list) {}
    
    bool InsertEffect(std::shared_ptr<ShipEffect> effect);

    bool RemoveEffect(std::string& source);

    inline ShipEffect* GetStrongestEffect() {
      if (IsEmpty()) {
        return nullptr;
      }
      return effect_array_[0].get();
    }

   inline bool IsEmpty() const {
      return effect_array_.empty();
    }
    
    inline ShipEffect* operator[](int index) const {
      return effect_array_[index].get();
    }

    inline int Size() const {
      return effect_array_.size();
    }

    inline auto Begin() {
      return effect_array_.begin();
    }

    inline auto End() {
      return effect_array_.end();
    }

  private:
    std::vector<shared_ptr<ShipEffect>> effect_array_;
};

class ShipEffectsMap {
  public:
    ShipEffectsMap() {}

    bool AddEffect(shared_ptr<ShipEffect> effect);
    
    bool RemoveEffect(ShipEffect::Type type, std::string& source);

    ShipEffectVector* GetEffect(ShipEffect effect);

    inline auto Begin() const {
      return effects_map_.begin();
    }

    inline auto End() const {
      return effects_map_.end();
    }

  private:
    unordered_map<ShipEffect::Type, ShipEffectVector> effects_map_;
};

} // namespace eve

#endif // EFFECTS_H_
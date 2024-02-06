#ifndef EFFECTS_H_
#define EFFECTS_H_

#include "ship.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <initializer_list>

namespace eve {

using std::unordered_map;
using std::shared_ptr;
using std::vector;
using std::make_unique;

class Ship;

class ShipEffect {
  public:
    enum Type {
      StasisWebifier,
    };
    
    ShipEffect(ShipEffect::Type type, float strength, std::string source)
        : type_(type), strength_(strength), source_(source) {}

    virtual ~ShipEffect() = default;
    
    inline ShipEffect::Type GetType() const {
      return type_;
    }

    virtual float Strength() const {
      return strength_;
    }
    
    virtual const std::string& Source() const {
      return source_;
    }
    private:
      ShipEffect::Type type_;
      float strength_;
      std::string source_;
};

class ShipEffectVector {
  public:
    ShipEffectVector() {}

    bool InsertEffect(shared_ptr<ShipEffect> effect);

    bool EraseEffect(shared_ptr<ShipEffect> effect);

    inline ShipEffect* GetStrongestEffect() {
      if (IsEmpty()) {
        return nullptr;
      }
      return effect_array_[0].get();
    }

    inline std::vector<float>* GetEffectStrVector() {
      return &effect_str_vector_;
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

    inline auto begin() {
      return effect_array_.begin();
    }

    inline auto end() {
      return effect_array_.end();
    }

  protected:
    vector<shared_ptr<ShipEffect>> effect_array_;
    vector<float> effect_str_vector_;
};

class EffectManager : public ShipEffectVector {
  public:
    virtual ~EffectManager() = default;

    virtual ShipEffect::Type Type() const = 0;

    virtual void ApplyEffect(shared_ptr<ShipEffect> effect) = 0;

    virtual bool RemoveEffect(shared_ptr<ShipEffect> effect);

    virtual void CalculateApplyEffect() = 0;
};

class StasisWebifierManager : public EffectManager {
  public:
    StasisWebifierManager(Ship* ship)
        : ship_(ship), type_(ShipEffect::StasisWebifier) {}

    inline ShipEffect::Type Type() const override {
      return type_;
    }

    void ApplyEffect(shared_ptr<ShipEffect> effect) override;
    void CalculateApplyEffect() override;

  private:
    const ShipEffect::Type type_;
    Ship* ship_;
};

class ShipEffectsMap {
  public:
    ShipEffectsMap(Ship* ship)
        : ship_(ship) {}

    void AddEffect(shared_ptr<ShipEffect> effect);
    
    bool RemoveEffect(shared_ptr<ShipEffect> effect);

    shared_ptr<EffectManager> GetEffectManager(shared_ptr<ShipEffect> effect);

    inline auto begin() const {
      return effects_map_.begin();
    }

    inline auto end() const {
      return effects_map_.end();
    }

  private:
    unordered_map<ShipEffect::Type, shared_ptr<EffectManager>> effects_map_;
    Ship* ship_;
};

} // namespace eve

#endif // EFFECTS_H_
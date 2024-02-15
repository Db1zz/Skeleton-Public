#ifndef EWAR_MODULES_H_
#define EWAR_MODULES_H_

#include <memory>
#include <vector>
#include <string>

namespace eve {

using std::shared_ptr;
using std::vector;
using std::string;

class Effect;

class EwarModule {
  public:
    enum Type {
      EnergyNeutralizer,
      StasisWebifier,
      SensorDampener,
      RemoteRepair,
      WarpScrambler,
      WeaponDisruptor,
      GuidanceDisruptor,
      TargetPainter,

      None
    };

    EwarModule(string source, Type ewar_type, float optimal, 
               float falloff, float rof, 
               const vector<shared_ptr<Effect>>& effects);

    inline shared_ptr<Effect>* GetSpecifcEffect(EffectType type) {
      for (auto& effect : effects_) {
        if (effect->GetType() == type) {
          return &effect;
        }
      }
    }

    inline vector<shared_ptr<Effect>>* GetEffects() {
      return &effects_;
    }
    virtual void ApplyEwar(Ship* target) {
      for (const auto& effect : effects_) {
        target->ApplyEffect(effect);
      }
    }

    virtual void RemoveEwar(Ship* target) {
      for (const auto& effect: effects_) {
        target->RemoveEffect(effect.get());
      }
    }

    inline const string& GetSource() const {
      return source_;
    }

    inline Type GetType() const {
      return ewar_type_;
    }

    inline float GetOptimal() const {
      return optimal_;
    }

    inline float GetFalloff() const {
      return falloff_;
    }

  private:
    string source_;
    EwarModule::Type ewar_type_;
    vector<shared_ptr<Effect>> effects_;
    float optimal_;
    float falloff_;
    float rof_;
};

// class EnegryNeutralizerEwar : public EwarModule {
//   public:
//     EnegryNeutralizerEwar(string source, float optimal, float falloff, 
//                           float rof, vector<float> effects_strengths);

//     void ApplyEwar(Ship* target) override;
//     void RemoveEwar(Ship* target) override;

//   private:
//     shared_ptr<EwarCapacitorRegenDecreaseEffect> cap_rech_decr_effect_;
// };

class EwarContainer {
  public:
    EwarContainer(const EwarContainer&) = delete;

    EwarContainer &operator=(const EwarContainer&) = delete;

    EwarContainer(vector<shared_ptr<EwarModule>>& ewar_module_list);

    EwarModule* operator[](int index) {
      return ewar_modules_[index].get();
    }

    vector<EwarModule*> FindEwarByType(EwarModule::Type type);

    void AddEwarModule(shared_ptr<EwarModule>& ewar_module);

    bool RemoveEwarModule(EwarModule* ewar_module);

    inline int Size() {
      return ewar_modules_.size();
    }

    inline bool Empty() {
      return ewar_modules_.size();
    }

    inline auto begin() {
      return ewar_modules_.begin();
    }

    inline auto end() {
      return ewar_modules_.end();
    }

  private:
    vector<shared_ptr<EwarModule>> ewar_modules_{};
};

} // namespace eve

#endif // EWAR_MODULES_H_
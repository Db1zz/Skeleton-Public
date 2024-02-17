#ifndef EWAR_MODULES_H_
#define EWAR_MODULES_H_

#include <memory>
#include <vector>
#include <string>

namespace eve {

using std::shared_ptr;
using std::vector;
using std::string;

enum class EffectType;
class Effect;
class Ship;

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

    virtual shared_ptr<Effect>* GetSpecifcEffect(EffectType type);

    virtual void ApplyEwar(Ship* target);
    
    virtual void RemoveEwar(Ship* target);

    inline vector<shared_ptr<Effect>>* GetEffects() {
      return &effects_;
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

    shared_ptr<EwarModule> Copy() const;

  private:
    string source_;
    EwarModule::Type ewar_type_;
    vector<shared_ptr<Effect>> effects_;
    float optimal_;
    float falloff_;
    float rof_;
};

class EwarContainer {
  public:
    EwarContainer() = default;

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

    EwarContainer Copy() const;

  private:
    vector<shared_ptr<EwarModule>> ewar_modules_{};
};

} // namespace eve

#endif // EWAR_MODULES_H_
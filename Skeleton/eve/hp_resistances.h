#ifndef HP_RESISTANCES_H_
#define HP_RESISTANCES_H_

#include <iostream>
#include <vector>

namespace eve {

using std::vector;

class DamageProfile {
  public:
    DamageProfile();

    DamageProfile(float em, float thermal, float kinetic, float explosive);

    DamageProfile(const DamageProfile& copy);

    inline DamageProfile Copy() const {
      return DamageProfile(*this);
    }

    virtual ~DamageProfile() = default;

    virtual inline void SetEm(float new_val) {
      dmg_types_[0] = new_val;
    }

    virtual inline void SetThermal(float new_val) {
      dmg_types_[1] = new_val;
    }

    virtual inline void SetKinetic(float new_val) {
      dmg_types_[2] = new_val;
    }

    virtual inline void SetExplosive(float new_val) {
      dmg_types_[3] = new_val;
    }

    inline float Em() const {
      return dmg_types_[0];
    }

    inline float Thermal() const {
      return dmg_types_[1];
    }
    
    inline float Kinetic() const {
      return dmg_types_[2];
    }

    inline float Explosive() const {
      return dmg_types_[3];
    }

    inline auto begin() const {
      return dmg_types_.begin();
    }

    inline auto end() const {
      return dmg_types_.end();
    }

    inline float operator[](float index) const {
      return dmg_types_[index];
    }

    inline size_t Size() const {
      return dmg_types_.size();
    } 

  protected:
    vector<float> dmg_types_;
};

class ResistanceProfile : public DamageProfile {
  public:
    ResistanceProfile(float em, float thermal, float kinetic, float explosive);

    ResistanceProfile(const ResistanceProfile& copy);

    inline void SetEm(float new_val) override {
      dmg_types_[0] = GetValidValue(new_val);
    }

    inline void SetThermal(float new_val) override {
      dmg_types_[1] = GetValidValue(new_val);
    }

    inline void SetKinetic(float new_val) override {
      dmg_types_[2] = GetValidValue(new_val);
    }

    inline void SetExplosive(float new_val) override {
      dmg_types_[3] = GetValidValue(new_val);
    }

  private:
    float GetValidValue(float value);
};

class ShipResistances {
  public:
    virtual ~ShipResistances() = default;

    ShipResistances(ResistanceProfile& shield, ResistanceProfile& armor,
                    ResistanceProfile& hull);

    ShipResistances(ResistanceProfile&& shield, ResistanceProfile&& armor,
                    ResistanceProfile&& hull);

    ShipResistances(const ShipResistances& copy);

    inline ResistanceProfile Shield() const {
      return profiles_[0];
    }

    inline ResistanceProfile Armor() const {
      return profiles_[1];
    }

    inline ResistanceProfile Hull() const {
      return profiles_[2];
    }

    inline void SetShield(ResistanceProfile new_val) {
      profiles_[0] = new_val;
    } 

    inline void SetArmor(ResistanceProfile new_val) {
      profiles_[1] = new_val;
    } 

    inline void SetHull(ResistanceProfile new_val) {
      profiles_[2] = new_val;
    }

    inline auto begin() const {
      return profiles_.begin();
    }

    inline auto end() const {
      return profiles_.end();
    }

    ResistanceProfile* operator[](int index) {
      return &profiles_[index];
    }

  private:
    vector<ResistanceProfile> profiles_{};
};

} // namepsace eve

#endif // HP_RESISTANCES_H_
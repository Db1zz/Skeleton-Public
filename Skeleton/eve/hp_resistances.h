#ifndef HP_RESISTANCES_H_
#define HP_RESISTANCES_H_

#include <iostream>
#include <vector>

namespace eve {

using std::vector;

class ResistanceProfile {
  public:
    ResistanceProfile() = default;

    ResistanceProfile(float em, float thermal, float kinetic, float explosive);

    ResistanceProfile(const ResistanceProfile& copy);

    virtual ~ResistanceProfile() = default;

    inline float Em() const {
      return *em_;
    }

    inline float Thermal() const {
      return *thermal_;
    }
    
    inline float Kinetic() const {
      return *kinetic_;
    }

    inline float Explosive() const {
      return *explosive_;
    }

    inline void SetEm(float new_val) {
      resistances_[0] = GetValidValue(new_val);
    }

    inline void SetThermal(float new_val) {
      resistances_[1] = GetValidValue(new_val);
    }

    inline void SetKinetic(float new_val) {
      resistances_[2] = GetValidValue(new_val);
    }

    inline void SetExplosive(float new_val) {
      resistances_[3] = GetValidValue(new_val);
    }

    inline auto begin() const {
      return resistances_.begin();
    }

    inline auto end() const {
      return resistances_.end();
    }

    inline float operator[](float index) const {
      return resistances_[index];
    }

    inline size_t Size() const {
      return resistances_.size();
    } 

  private:
    float GetValidValue(float value);

    vector<float> resistances_{0, 0, 0, 0};
    float* em_{&resistances_[0]};
    float* thermal_{&resistances_[1]};
    float* kinetic_{&resistances_[2]};
    float* explosive_{&resistances_[3]};
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
      return *shield_;
    }

    inline ResistanceProfile Armor() const {
      return *armor_;
    }

    inline ResistanceProfile Hull() const {
      return *hull_;
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
    ResistanceProfile* shield_ = nullptr;
    ResistanceProfile* armor_ = nullptr;
    ResistanceProfile* hull_ = nullptr;
};

class DamageProfile {
  public:
    DamageProfile() = default;

    DamageProfile(float em, float thermal, float kinetic, float explosive);

    virtual ~DamageProfile() = default;

    inline float Em() const {
      return *em_;
    }

    inline float Thermal() const {
      return *thermal_;
    }

    inline float Kinetic() const {
      return *kinetic_;
    }

    inline float Explosive() const {
      return *explosive_;
    }

    inline void SetEm(float new_val) {
      dmg_types_[0] = new_val;
    }

    inline void SetThermal(float new_val) {
      dmg_types_[1] = new_val;
    }

    inline void SetKinetic(float new_val) {
      dmg_types_[2] = new_val;
    }

    inline void SetExplosive(float new_val) {
      dmg_types_[3] = new_val;
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

  private:
    vector<float> dmg_types_{0, 0, 0, 0};
    float* em_{&dmg_types_[0]};
    float* thermal_{&dmg_types_[1]};
    float* kinetic_{&dmg_types_[2]};
    float* explosive_{&dmg_types_[3]};
};

} // namepsace eve

#endif // HP_RESISTANCES_H_
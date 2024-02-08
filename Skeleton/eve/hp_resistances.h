#ifndef HP_RESISTANCES_H_
#define HP_RESISTANCES_H_

#include <iostream>

namespace eve {

class ResistanceProfile {
  public:
    ResistanceProfile() = default;

    ResistanceProfile(float em = 0, float thermal = 0, 
                      float kinetic = 0, float explosive = 0);

    virtual ~ResistanceProfile() = default;

    inline float Em() const {
      return em_;
    }

    inline float Thermal() const {
      return thermal_;
    }
    
    inline float Kinetic() const {
      return kinetic_;
    }

    inline float Explosive() const {
      return explosive_;
    }

    inline void SetEm(float new_val) {
      em_ = new_val;
      IsValidValues();
    }

    inline void SetThermal(float new_val) {
      thermal_ = new_val;
      IsValidValues();
    }

    inline void SetKinetic(float new_val) {
      kinetic_ = new_val;
      IsValidValues();
    }

    inline void SetExplosive(float new_val) {
      explosive_ = new_val;
      IsValidValues();
    }

  private:
    void IsValidValues() const {
      if (em_ > 1 || thermal_ > 1 || kinetic_ > 1 || explosive_ > 1) {
        std::cout << "ResistanceProfile: Contains invalid values > 1\n";
        exit(-1);
      }
    }

    float em_{0};
    float thermal_{0};
    float kinetic_{0};
    float explosive_{0};  
};

class ShipResistances {
  public:
    virtual ~ShipResistances() = default;

    ShipResistances(ResistanceProfile& shield, ResistanceProfile& armor,
                    ResistanceProfile& hull)
        : shield_(shield), armor_(armor), hull_(hull) {}

    inline ResistanceProfile Shield() const {
      return shield_;
    }

    inline ResistanceProfile Armor() const {
      return armor_;
    }

    inline ResistanceProfile Hull() const {
      return hull_;
    }

    inline void SetShield(ResistanceProfile new_val) {
      shield_ = new_val;
    } 

    inline void SetArmor(ResistanceProfile new_val) {
      armor_ = new_val;
    } 

    inline void SetHull(ResistanceProfile new_val) {
      hull_ = new_val;
    }

  private:
    ResistanceProfile shield_;
    ResistanceProfile armor_;
    ResistanceProfile hull_;
};

class DamageProfile {
  public:
    DamageProfile() = default;

    DamageProfile(float em, float thermal, float kinetic, float explosive);

    virtual ~DamageProfile() = default;

    inline float Em() const {
      return em_;
    }

    inline float Thermal() const {
      return thermal_;
    }

    inline float Kinetic() const {
      return kinetic_;
    }

    inline float Explosive() const {
      return explosive_;
    }

  private:
    float em_{0};
    float thermal_{0};
    float kinetic_{0};
    float explosive_{0};
};

} // namepsace eve

#endif // HP_RESISTANCES_H_
#ifndef SHIP_WEAPON_H_
#define SHIP_WEAPON_H_

#include "hp_resistances.h"

namespace eve {

using std::shared_ptr;

class MissileLauncher;
class Weapon;

class Ammo {
  public:
    Ammo(const DamageProfile* dmg_profile)
        : dmg_profile_(*dmg_profile) {}

    Ammo(DamageProfile dmg_profile)
    : dmg_profile_(dmg_profile) {}

    virtual ~Ammo() = default;

    inline const DamageProfile* DmgProfile() const {
      return &dmg_profile_;
    }

  private:
    DamageProfile dmg_profile_;
};

class MissileAmmo : public Ammo {
  public:
    MissileAmmo(float velocity, float flight_time, 
            const DamageProfile* dmg_profile);

    inline float Velocity() const {
      return velocity_;
    }
    
    inline float FlightTime() const {
      return flight_time_;
    }

  private:
    float velocity_;
    float flight_time_;
};

class TurretAmmo : public Ammo {
  public:
    TurretAmmo(float falloff_modifier, float optimal_modifier,
               float tracking_modifier, const DamageProfile* dmg_profile);
  
    inline float OptimalModifier() const {
      return optimal_modifier_;
    }

    inline float FalloffModifier() const {
      return falloff_modifier_;
    }

    inline float TrackingModifier() const {
      return tracking_modifier_;
    }

  private:
    float falloff_modifier_;
    float optimal_modifier_;
    float tracking_modifier_; // NOT USED!
};

class Weapon {
  public:
    Weapon(float rof, float reload_time, float weapon_amount, 
           DamageProfile dmg_profle);

    virtual ~Weapon() = default;

    // virtual WeaponType Type() const = 0; TODO

    virtual void LoadAmmo(const shared_ptr<Ammo> ammo) = 0;

    virtual void UnloadAmmo() = 0;

    virtual float Dps(const ResistanceProfile* res) const = 0;

    inline float WeaponAmount() const {
      return weapon_amount_;
    }

    inline float ReloadTime() const {
      return reload_time_;
    }

    inline const DamageProfile* DmgProfile() const {
      return &curr_dmg_profile_;
    }

    inline const DamageProfile* BaseDmgProfile() const {
      return &base_dmg_profile_;
    }

    inline float Application(float new_val) const {
      return application_;
    }

    inline float Range() const {
      return range_;
    }

    inline void SetRange(float new_val) {
      range_ = new_val;
    }

    inline void SetApplication(float new_val) {
      application_ = new_val;
    }

    inline void SetDmgProfile(const DamageProfile* new_val) {
      curr_dmg_profile_ = *new_val;
    }

  protected:
    float rof_;
    float range_;
    float reload_time_;
    float weapon_amount_;
    float application_;
    DamageProfile curr_dmg_profile_;
    DamageProfile base_dmg_profile_;
};

class MissileWeapon : public Weapon {
  public:
    MissileWeapon(float rof, float reload_time, float weapon_amount) 
        : Weapon(rof, reload_time, weapon_amount, DamageProfile()),
          ammo_(nullptr) {}
  
    float Dps(const ResistanceProfile* res) const override;

    void LoadAmmo(const shared_ptr<Ammo> ammo) override;

    void UnloadAmmo() override;
    
  protected:
    void ApplyAmmoBonuses();

  private:
    shared_ptr<MissileAmmo> ammo_;
    float range_;
};

class TurretWeapon : public Weapon {
  public:
    TurretWeapon(float rof, float reload_time, float weapon_amount,
                 float dmg_multiplier, float base_optimal,
                 float base_falloff, float base_tracking);

    float Dps(const ResistanceProfile* res) const override;

    void LoadAmmo(const shared_ptr<Ammo> ammo);

    void UnloadAmmo() override;

  protected:
    void ApplyAmmoBonuses();

  private:
    shared_ptr<TurretAmmo> ammo_;
    float dmg_multiplier_;
    float base_optimal_;
    float base_falloff_;
    float base_tracking_;
    float range_;
    float application_;
};

} // namespace eve

#endif // SHIP_WEAOPN_H_
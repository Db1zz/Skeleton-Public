#ifndef SHIP_WEAPON_H_
#define SHIP_WEAPON_H_

#include "hp_resistances.h"
#include "../abyss_bot/bot.h"

#include <vector>
#include <unordered_map>

namespace eve {

using std::shared_ptr;
using std::vector;
using std::unordered_map;

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

    virtual shared_ptr<Ammo> Copy() const = 0;

  protected:
    DamageProfile dmg_profile_;
};

class MissileAmmo : public Ammo {
  public:
    MissileAmmo(float velocity, float flight_time, 
                const DamageProfile* dmg_profile);

    inline float Velocity() const {
      return missile_velocity_;
    }
    
    inline float FlightTime() const {
      return flight_time_;
    }

    virtual shared_ptr<Ammo> Copy() const override;

  private:
    float missile_velocity_;
    float missile_velocity_bonus_;
    float missile_flight_time_;
    float missile_flight_time_bouns_;
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

    virtual shared_ptr<Ammo> Copy() const override;

  private:
    float falloff_modifier_;
    float optimal_modifier_;

     // TODO: add function that will calculate
     // application to target. 
     // Note: this is so hard to implement without
     // reading the game memory, so let's think about
     // it when we will be able to find the stable path for 
     // eve classes and data structures(I HATE PYTHON).
    float tracking_modifier_;
};

class Weapon {
  public:
    enum class Type {
      TurretWeapon,
      MissileWeapon,

      Total
    };

    Weapon(float rof, float reload_time, float weapon_amount, 
           const DamageProfile* dmg_profle);

    virtual ~Weapon() = default;

    virtual void LoadAmmo(const shared_ptr<Ammo>& ammo) = 0;

    virtual void UnloadAmmo() = 0;

    virtual float Dps(const ShipResistances* res) const = 0;
    virtual float Dps(const ResistanceProfile* res) const = 0;
    virtual float Dps(const shared_ptr<abyss::Bot>& target) const = 0;

    virtual Weapon::Type GetType() const = 0;

    virtual shared_ptr<Weapon> Copy() const = 0;

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
    MissileWeapon(float rof, float reload_time, float weapon_amount,
                  const DamageProfile* dmg_profile);

    float Dps(const ShipResistances* res) const override;
    float Dps(const ResistanceProfile* res) const override;
    float Dps(const shared_ptr<abyss::Bot>& target) const override;

    void LoadAmmo(const shared_ptr<Ammo>& ammo) override;

    void UnloadAmmo() override;

    inline Weapon::Type GetType() const override {
      return Type::MissileWeapon;
    }


    virtual shared_ptr<Weapon> Copy() const override;

  protected:
    inline const shared_ptr<MissileAmmo>& GetAmmo() const {
      return ammo_;
    };

    void ApplyAmmoBonuses();

  private:
    shared_ptr<MissileAmmo> ammo_;
};

class TurretWeapon : public Weapon {
  public:
    TurretWeapon(float rof, float reload_time, float weapon_amount,
                 float dmg_multiplier, float optimal, float falloff,
                 float tracking, const DamageProfile* dmg_profile,
                 float dmg_multiplier_per_cycle,
                 float dmg_max_spool_multiplier);

    inline float Tracking() const {
      return tracking_;  
    }

    inline float Optimal() const {
      return optimal_;
    }

    inline float Falloff() const {
      return falloff_;
    }

    inline Weapon::Type GetType() const override {
      return Type::TurretWeapon;
    }

    virtual shared_ptr<Weapon> Copy() const override;

    float Dps(const ShipResistances* res) const override;
    float Dps(const ResistanceProfile* res) const override;
    float Dps(const shared_ptr<abyss::Bot>& target) const override;

    void LoadAmmo(const shared_ptr<Ammo>& ammo) override;

    void UnloadAmmo() override;

  protected:
    void ApplyAmmoBonuses();

  private:
    shared_ptr<TurretAmmo> ammo_;
    float dmg_multiplier_;
    float dmg_multiplier_per_cycle_;
    float dmg_max_spool_multiplier_;
    float optimal_;
    float falloff_;
    float tracking_;
};

class WeaponContainer {
  public:
    WeaponContainer() = default;

    WeaponContainer(vector<shared_ptr<Weapon>>& weapons);

    vector<shared_ptr<Weapon>>* GetWeaponsByType(Weapon::Type weapon_type);

    void Init(const vector<shared_ptr<Weapon>>& weapons);

    virtual WeaponContainer Copy() const;

    inline int TotalWeapons() const {
      return total_weapons_amount_;
    }

    inline auto begin() const {
      return weapons_map_.begin();
    }

    inline auto end() const {
      return weapons_map_.end();
    }

    vector<shared_ptr<Weapon>> GetAllWeapons() const;

  private:
    unordered_map<Weapon::Type, vector<shared_ptr<Weapon>>> weapons_map_;
    int total_weapons_amount_;
};

} // namespace eve

#endif // SHIP_WEAOPN_H_
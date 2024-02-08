class Weapon {
  public:
    float Dps();
    float LoadAmmo();
    float UnloadAmmo();
    float Range();
};

class MissileLauncher : public Weapon {
  public:

  private:
    float range_;
};

class ProjectileWeapon : public Weapon {
  public:
  
  private:
    float range_;
};
#include "eve_math.h"

namespace eve {
using std::vector;

float CalcStackingPenalty(float effect_amount) {
  float base_strength{100};
  // 𝑆(𝑢)=𝑒^−(𝑢/2.67)^2
  float p = std::powf(exp(1), std::powf((effect_amount / 2.67), 2));
  float ep = 100 / p;
  float result = (base_strength * (ep / 100) / 100);

  return result;
}

float ApplyStackingPenalty(float effect_strength, float effect_amount) {
  return 1 - (effect_strength * CalcStackingPenalty(effect_amount)) / 100;
}

float CalcStackingPenalty(float affected_parameter_value,
                          const vector<float>& attributes) {
  int   applied_times = 0;
  float current_parameter_value = affected_parameter_value;
  float result = 0;

  for (int i = 0; i < attributes.size(); i++) {
    // 𝑆(𝑢)=𝑒^−(𝑢/2.67)^2
    float p = std::powf(std::exp(1), std::powf((i / 2.67), 2));
    float ep = 100 / p;
    float curr_atribute_val = attributes[i] * (ep / 100);
    float diff = (current_parameter_value * (curr_atribute_val / 100));
    current_parameter_value -= diff;

    result += (diff / affected_parameter_value) * 100;
  }
  
  return result;
}

float ApplyStackingPenalty(float affected_parameter_value,
                           const vector<float>& attributes) {
  float percent = CalcStackingPenalty(affected_parameter_value, attributes);

  return (affected_parameter_value / 100) * (100 - percent);
};

float DecreaseByPercent(float value, float percent) {
  return value * (1 - percent);
}

float DivideByPercent(float value, float percent) {
  return value / percent;
}

template<typename T>
T SumOfVector(vector<T> values) {
  T result{0};
  for (auto& value : values) {
    result += value;
  }

  return result;
}

float RoundF(float value) {
  return ((float)((int)(value * 10 + .5)) / 10);
}

// Read about this function: https://wiki.eveuniversity.org/Turret_mechanics
float CalcTurretHitProbability(float angular_velocity,
                               float turret_tracking,
                               float turret_falloff,
                               float turret_optimal,
                               float target_signature_radius,
                               float distance) 
{
  // Angular velocity = velocity / orbit range

  // Formula: ((AngularVel * 40,000m) / (Tracking * Signature))^2 +
  //          ((max(0, Distance - Optimal) / Falloff))^2
  
  // ((AngularVel * 40,000m) / (Tracking * Signature))^2
  float expr1 = ((angular_velocity * 40000) / 
                 (turret_tracking * target_signature_radius));
  expr1 = std::powf(expr1, 2);                

  // ((max(0, Distance - Optimal) / Falloff))^2
  float expr2 = ((std::max(0.0f, distance - turret_optimal) / turret_falloff));
  expr2 = std::powf(expr2, 2);

  // 0.5^(expr1 + expr2)
  float result = std::powf(0.5, (expr1 + expr2));

  return result;
}

// Read about this function: https://wiki.eveuniversity.org/Missile_mechanics
float CalcMissileApplication(float damage,
                             float target_sig_radius, 
                             float missile_expl_radius,
                             float missile_expl_velocity,
                             float target_velocity)
{  
  // S = target_signature_radius
  // Vt = target_velocity
  // D = damage
  // E = missile_explosion_radius
  // Ve = missile_explosion_velocity
  // DRF = damage_reduction_factor (Check the Wiki)

  // Formula: Damage=𝐷⋅min(1,𝑆/𝐸,((𝑆/𝐸)⋅(𝑉e/𝑉t))^drf)

  // S/E
  float expr1 = target_sig_radius / missile_expl_radius;

  float drf = 0.682;

  // ((𝑆/𝐸)⋅(𝑉e/𝑉t))^drf
  float expr2 = std::powf((target_sig_radius / missile_expl_radius) *
                          (missile_expl_velocity / target_velocity), drf);

  float min = std::min({1.0f, expr1, expr2});

  // Damage * min
  return damage * min;
}

} // namespace eve
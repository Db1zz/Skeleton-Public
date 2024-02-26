#ifndef EVE_MATH_H_
#define EVE_MATH_H_

#include <vector>

namespace eve {

float CalcStackingPenalty(float effect_amount);

float ApplyStackingPenalty(float effect_strength, float effect_amount);

float CalcStackingPenalty(float affected_parameter_value,
                          const std::vector<float>& attributes);

float ApplyStackingPenalty(float affected_parameter_value,
                           const std::vector<float>& attributes);

template<typename T>
T MultiplyByValuesInArray(T value_to_multiply, const std::vector<T>& values) {
  if (values.size() > 0) {
    for (auto& value : values) {
      value_to_multiply *= value;
    }
  }

  return value_to_multiply;
}

float DecreaseByPercent(float value, float percent);

float DivideByPercent(float value, float percent);

template<typename T>
T SumOfVector(std::vector<T> values);

float RoundF(float value);

float CalcAngularVelocity(float ship_velocity, float target_orbit_range);

float CalcTurretHitProbability(float angular_velocity,
                               float turret_tracking,
                               float turret_falloff,
                               float turret_optimal,
                               float target_signature_radius,
                               float distance);

float CalcMissileApplication(float target_sig_radius, 
                             float missile_expl_radius,
                             float missile_expl_velocity,
                             float target_velocity);

} // namespace eve

#endif // EVE_MATH_H_
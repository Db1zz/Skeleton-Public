#ifndef EVE_MATH_H_
#define EVE_MATH_H_

#include <vector>

namespace eve {

float CalcStackingPenalty(float affected_parameter_value,
                          const std::vector<float>& attributes) {
  int   applied_times = 0;
  float current_parameter_value = affected_parameter_value;
  float result = 0;

  for (int i = 0; i < attributes.size(); i++)
  {
      // 𝑆(𝑢)=𝑒^−(𝑢/2.67)^2
      float p = std::pow(std::exp(1), std::pow((i / 2.67), 2));
      float ep = 100 / p;
      float curr_atribute_val = attributes[i] * (ep / 100);
      float diff = (current_parameter_value * (curr_atribute_val / 100));
      current_parameter_value -= diff;

      result += (diff / affected_parameter_value) * 100;
  }

  return result;
}

float ApplyStackingPenalty(float affected_parameter_value,
                           const std::vector<float>& attributes) {
  float percent = CalcStackingPenalty(affected_parameter_value, attributes);

  return (affected_parameter_value / 100) * (100 - percent);
};

} // namespace eve

#endif // EVE_MATH_H_
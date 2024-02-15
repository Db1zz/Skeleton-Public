#include "eve_math.h"

namespace eve {

float CalcStackingPenalty(float effect_amount) {
  float base_strength{100};
  // 𝑆(𝑢)=𝑒^−(𝑢/2.67)^2
  float p = std::pow(std::exp(1), std::pow((effect_amount / 2.67), 2));
  float ep = 100 / p;
  float result = (base_strength * (ep / 100) / 100);

  return result;
}

float ApplyStackingPenalty(float effect_strength, float effect_amount) {
  return 1 - (effect_strength * CalcStackingPenalty(effect_amount)) / 100;
}

float CalcStackingPenalty(float affected_parameter_value,
                          const std::vector<float>& attributes) {
  int   applied_times = 0;
  float current_parameter_value = affected_parameter_value;
  float result = 0;

  for (int i = 0; i < attributes.size(); i++) {
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

float DecreaseByPercent(float value, float percent) {
  return value * (1 - percent);
}

float DivideByPercent(float value, float percent) {
  return value / percent;
}

template<typename T>
T SumOfVector(std::vector<T> values) {
  T result{0};
  for (auto& value : values) {
    result += value;
  }

  return result;
}

float RoundF(float value) {
  return ((float)((int)(value * 10 + .5)) / 10);
}

} // namespace eve
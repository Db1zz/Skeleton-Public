#ifndef EVE_MATH_H_
#define EVE_MATH_H_

#include <vector>

namespace eve {

float CalcStackingPenalty(float affected_parameter_value,
                          const std::vector<float>& attributes);

float ApplyStackingPenalty(float affected_parameter_value,
                           const std::vector<float>& attributes);

float DecreaseByPercent(float value, float percent);

float DivideByPercent(float value, float percent);

} // namespace eve

#endif // EVE_MATH_H_
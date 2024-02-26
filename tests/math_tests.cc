// #include "../Skeleton/eve/eve_math.h"
// #include <gtest/gtest.h>

// using namespace eve;

// TEST(DecreaseByPercentTest, HandlesDecimalPercent) {
//   EXPECT_EQ(DecreaseByPercent(100, 0.10), 90);
// }

// TEST(DecreaseByStackingPenalty, HandlesAmountOfEffects) {
//   EXPECT_FLOAT_EQ(RoundF(CalcStackingPenalty(0)), RoundF(float(1)));
//   EXPECT_FLOAT_EQ(RoundF(CalcStackingPenalty(1)), RoundF(float(0.869)));
//   EXPECT_FLOAT_EQ(RoundF(CalcStackingPenalty(2)), RoundF(float(0.57)));
//   EXPECT_FLOAT_EQ(RoundF(CalcStackingPenalty(3)), RoundF(float(0.28)));
//   EXPECT_FLOAT_EQ(RoundF(CalcStackingPenalty(4)), RoundF(float(0.11)));
//   EXPECT_FLOAT_EQ(RoundF(CalcStackingPenalty(5)), RoundF(float(0.029)));
// }

// TEST(MultiplyEffectStrByStackingPenalty, HandlesEffectStrAndEffectAmount) {
//   EXPECT_FLOAT_EQ(RoundF(ApplyStackingPenalty(60, 0)), RoundF(60));
//   EXPECT_FLOAT_EQ(RoundF(ApplyStackingPenalty(60, 1)), RoundF(52.15));
//   EXPECT_FLOAT_EQ(RoundF(ApplyStackingPenalty(60, 2)), RoundF(34.23));
//   EXPECT_FLOAT_EQ(RoundF(ApplyStackingPenalty(60, 3)), RoundF(16.98));
//   EXPECT_FLOAT_EQ(RoundF(ApplyStackingPenalty(60, 4)), RoundF(6.36));
//   EXPECT_FLOAT_EQ(RoundF(ApplyStackingPenalty(60, 5)), RoundF(1.8));
// }


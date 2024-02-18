#include "scores.h"

namespace abyss {

using namespace eve;

Scores::Scores(initializer_list<pair<EwarModule::Type, float>> scores) {
  Insert(scores);
}

pair<EwarModule::Type, float> Scores::GetBiggestScore() const {
  if (scores_.size() > 0) {
    return scores_[0];
  }
  return {EwarModule::Type::None, 0};
}

void Scores::Insert(const pair<eve::EwarModule::Type, float>& score) {
  size_t size = scores_.size();
  for (int i = 0; i < size; i++) {
    if (score.second >= scores_[i].second) {
      scores_.insert(scores_.begin() + i, score);
      break;
    }
  }

  if (size == scores_.size()) {
    scores_.push_back(score);
  }
}

void Scores::Insert(initializer_list<pair<EwarModule::Type, float>> scores) {
  for (const auto& scr : scores) {
    Insert(scr);
  }
}

} // namespace abyss
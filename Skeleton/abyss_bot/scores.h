#ifndef SCORES_H_
#define SCORES_H_

#include "../eve/ewar_modules.h"

#include <vector>
#include <initializer_list>

namespace abyss {

using std::pair;
using std::initializer_list;
using std::vector;

class Scores {
  public:
    Scores(initializer_list<pair<eve::EwarModule::Type, float>> scores);

    pair<eve::EwarModule::Type, float> GetBiggestScore() const;
    
    void Insert(const pair<eve::EwarModule::Type, float>& score);
    void Insert(initializer_list<pair<eve::EwarModule::Type, float>> scores);

  private:
    vector<pair<eve::EwarModule::Type, float>> scores_;
};

} // namespace abyss

#endif // SCORES_H_
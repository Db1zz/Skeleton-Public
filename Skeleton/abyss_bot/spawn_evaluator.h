
#ifndef SPAWN_EVALUATOR_H_
#define SPAWN_EVALUATOR_H_

#include "../eve/npc_container.h"
#include "../eve/ship.h"

#include <memory>
#include <utility>

namespace abyss {

using std::shared_ptr;
using std::pair;

class NpcEvaluator {
  public:
    NpcEvaluator() = default;

    float GetTargetEhp(const shared_ptr<eve::Ship>& target,
                       const shared_ptr<eve::Ship>& shooter);

    float GetEfficiencyForEhpUnit(float ehp, float value_for_evaluation);

    float EvaluateTargetEwarEfficiencyForEhpUnit(
        const shared_ptr<eve::EwarModule>& ewar, float ehp);

    float EvaluateTargetEwarEfficiencyForEhpUnit(
        const shared_ptr<eve::Ship>& ewar_source, 
        const shared_ptr<eve::Ship>& player);

    float EvaulateTargetDpsEfficiencyForEhpUnit(
        const shared_ptr<eve::Npc>& npc_target, 
        const shared_ptr<eve::Ship>& player);

    float EvaulateTargetRrEfficenecyForEhpUnit(
        const shared_ptr<eve::Npc>& npc_target, 
        const shared_ptr<eve::Ship>& player);

    float Evaulate(const shared_ptr<eve::Npc>& target_to_evaulate,
                   const shared_ptr<eve::Ship>& player_ship);

  private:

};

class SpawnEvaluator {
  public:
    SpawnEvaluator() = default;

    shared_ptr<eve::Npc> EvaulateTarget(
        const shared_ptr<eve::NpcContainer>& spawn,
        const shared_ptr<eve::Ship>& player_ship);

    void EvaulateSpawn(const shared_ptr<eve::NpcContainer>& spawn,
                       const shared_ptr<eve::Ship>& player_ship);

  private:
    enum class ScoreType {
      LockRangeScore,
      VelocityScore,
      CapacitorRegScore,
      DpsTankScore,
      DpsScore,

      None
    };

    pair<ScoreType, float> GetLockRangeScore(
        const shared_ptr<eve::Ship>& affected_ship,
        const shared_ptr<eve::Ship>& ship);

    pair<ScoreType, float> GetVelocityScore(
        const shared_ptr<eve::Ship>& affected_ship,
        const shared_ptr<eve::Ship>& ship);

    pair<ScoreType, float> GetCapactiorRegScore(
        const shared_ptr<eve::Ship>& affected_ship,
        const shared_ptr<eve::Ship>& ship);

    pair<ScoreType, float> GetDpsTankScore(
        const shared_ptr<eve::Ship>& affected_ship,
        const shared_ptr<eve::Ship>& ship);

    pair<SpawnEvaluator::ScoreType, float> GetDpsScore(
        const shared_ptr<eve::Ship>& affected_ship,
        const shared_ptr<eve::Ship>& ship);

    shared_ptr<eve::Ship> ApplyEffectsToShip(
        const shared_ptr<eve::NpcContainer>& spawn,
        const shared_ptr<eve::Ship>& ship);

shared_ptr<eve::Npc> GetTarget(
    const shared_ptr<eve::NpcContainer>& spawn,
    const shared_ptr<eve::Ship>& player_ship,
    const shared_ptr<eve::Ship>& affected_player_ship);

    float spawn_remote_rep_total_str_ = 0;
    float spawn_lock_range_score_ = 0;
    vector<shared_ptr<Npc>> dangerous_npcs_;
};

} // namespace abyss

#endif // SPAWN_EVALUATOR_H_
#include "spawn_evaluator.h"
#include "../eve/eve_math.h"

#include <iostream>
#include <initializer_list>

namespace abyss {

using std::initializer_list;

using namespace eve;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// NpcEvaluator ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

float NpcEvaluator::GetTargetEhp(const shared_ptr<Ship>& target, 
                                 const shared_ptr<Ship>& shooter) 
{
  return target->Defense()->Ehp(*shooter->Weapons());
}

float NpcEvaluator::GetEfficiencyForEhpUnit(
    float ehp, float value_for_evaluation) 
{
  return value_for_evaluation / ehp * 1000;
}

float NpcEvaluator::EvaluateTargetEwarEfficiencyForEhpUnit(
    const shared_ptr<EwarModule>& ewar, float ehp)
{
  float score = 0;

  vector<shared_ptr<Effect>>* effects = ewar->GetEffects();
  for (const auto& effect : *effects) {
    score += GetEfficiencyForEhpUnit(ehp, effect->GetStrength());
  }

  return score;
}

float NpcEvaluator::EvaluateTargetEwarEfficiencyForEhpUnit(
    const shared_ptr<Ship>& ewar_source, const shared_ptr<Ship>& player)
{
  float score = 0;

  float ehp = GetTargetEhp(ewar_source, player);

  for (const auto& ewar : *ewar_source->Ewar()) {
    score += EvaluateTargetEwarEfficiencyForEhpUnit(ewar, ehp);
  }

  return score;
}

float NpcEvaluator::EvaulateTargetDpsEfficiencyForEhpUnit(
    const shared_ptr<Npc>& npc_target, const shared_ptr<Ship>& player)
{
  float dps = npc_target->Dps(&player->Defense()->ShipRes());
  float ehp = GetTargetEhp(npc_target, player);

  return GetEfficiencyForEhpUnit(ehp, dps);
}

float NpcEvaluator::EvaulateTargetRrEfficenecyForEhpUnit(
    const shared_ptr<Npc>& npc_target, const shared_ptr<Ship>& player)
{
  float rrps = 0;
  float ehp = GetTargetEhp(npc_target, player);

  vector<EwarModule*> rr_ewar = 
      npc_target->Ewar()->FindEwarByType(EwarModule::RemoteRepair);

  if (rr_ewar.size() > 0) {
    for (const auto& rr : rr_ewar) {
      shared_ptr<Effect>* eff =
          rr->GetSpecifcEffect(EffectType::EwarArmorRestorationEffect);

      if (eff) {
        rrps += (*eff)->GetStrength();
      }
    }
  }
  
  return GetEfficiencyForEhpUnit(ehp, rrps);
}

float NpcEvaluator::Evaulate(const shared_ptr<Npc>& target_to_evaulate,
                             const shared_ptr<Ship>& player_ship)
{
  if (target_to_evaulate->GetName() == "Tangling Damavik") {
    printf("Found\n");
  }

  float dps_efficency_score = 
      EvaulateTargetDpsEfficiencyForEhpUnit(target_to_evaulate, player_ship);
  
  float ewar_efficency_score = 
      EvaluateTargetEwarEfficiencyForEhpUnit(target_to_evaulate, player_ship);
  
  float total_score = dps_efficency_score + ewar_efficency_score;
  
  return total_score;
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// SpawnEvaluator ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define TIME_MULTIPLIER 2.5f
#define SURVIVIAL_MULTIPLIER 1.3f
#define ENERGY_NEUT_MULTIPLIER 40.0f
#define WEBIFIER_MULTIPLIER 0.3f
#define DPS_TANK_MULTIPLIER 2.5f
#define REMOTE_REP_MULTIPLIER 7.5f

// 

pair<SpawnEvaluator::ScoreType, float> SpawnEvaluator::GetCapactiorRegScore(
    const shared_ptr<Ship>& affected_ship,
    const shared_ptr<Ship>& ship)
{ 
  float score = 1.0f;

  if (affected_ship->Capacitor()->RechargeRate() < 0) {
    score += fabs(affected_ship->Capacitor()->RechargeRate() /
                  ship->Capacitor()->RechargeRate());
    
    if (score != 1.0f) {
      score = score * SURVIVIAL_MULTIPLIER * ENERGY_NEUT_MULTIPLIER;
    }
  }

  std::cout << "Capacitor Reg Score: " << score << '\n';
  return {SpawnEvaluator::ScoreType::CapacitorRegScore, score};
}

pair<SpawnEvaluator::ScoreType, float> SpawnEvaluator::GetDpsTankScore(
    const shared_ptr<Ship>& affected_ship,
    const shared_ptr<Ship>& ship)
{
  float score = 1;

  if (affected_ship->Defense()->HPs() < 0)  {
    float temp_score = fabs(affected_ship->Defense()->HPs() /
                            ship->Defense()->HPs());
    if (temp_score != 1.0f) {
      score += temp_score * SURVIVIAL_MULTIPLIER * DPS_TANK_MULTIPLIER;
    }
  }

  std::cout << "Dps Tank Score: " << score << '\n';
  return {SpawnEvaluator::ScoreType::DpsTankScore, score};
}

pair<SpawnEvaluator::ScoreType, float> SpawnEvaluator::GetDpsScore(
    const shared_ptr<Ship>& affected_ship,
    const shared_ptr<Ship>& ship) 
{
  float dps = affected_ship->Dps(affected_ship);
      
  float score = 1;
  if (spawn_remote_rep_total_str_ > 0) {
    float temp = (dps - spawn_remote_rep_total_str_) / dps;
    if (temp != 1.0f) {
      score += temp * TIME_MULTIPLIER * REMOTE_REP_MULTIPLIER;
      // if (temp < 0.7) {
      //   score = score * 4;
      // }
    }
  }

  std::cout << "Dps Score: " << score << '\n';
  return {SpawnEvaluator::ScoreType::DpsScore, score};
}

pair<SpawnEvaluator::ScoreType, float> SpawnEvaluator::GetLockRangeScore(
    const shared_ptr<Ship>& affected_ship,
    const shared_ptr<Ship>& ship) 
{
  float score = affected_ship->Targeting()->Range() / 
                ship->Targeting()->Range();
  
  float velocity_score = GetVelocityScore(affected_ship, ship).second;

  if (score != 1.0f) {
    score = (score * TIME_MULTIPLIER * SURVIVIAL_MULTIPLIER * 2);
    if (velocity_score != 1.0f) {
      score = 1;
    }
  } else {
      score = 0;
  }

  spawn_lock_range_score_ = score;

  std::cout << "Lock Range Score: " << score << '\n';
  return {SpawnEvaluator::ScoreType::LockRangeScore, score};
}

pair<SpawnEvaluator::ScoreType, float> SpawnEvaluator::GetVelocityScore(
    const shared_ptr<Ship>& affected_ship,
    const shared_ptr<Ship>& ship)
{
  float score = 1;

  if (affected_ship->Engine()->Velocity() != 
      ship->Engine()->Velocity()) {
    float temp = affected_ship->Engine()->Velocity() /
                 ship->Engine()->Velocity();
    if (temp != 1.0f) {
      score += (temp * WEBIFIER_MULTIPLIER) + 
               (spawn_lock_range_score_ / 2);
    }
  }

  std::cout << "Velocity Score: " << score << '\n';
  return {SpawnEvaluator::ScoreType::VelocityScore, score};
}


//TODO: I don't like the name of function and it's body.
shared_ptr<Ship> SpawnEvaluator::ApplyEffectsToShip(
    const shared_ptr<NpcContainer>& spawn,
    const shared_ptr<Ship>& ship)
{
  // Create a copy of the ship
  shared_ptr<Ship> s_copy = ship->Copy();

  for (const auto& npc_pair : *spawn.get()) {
    string_view            npc_name   = npc_pair.first;
    const shared_ptr<Npc>& npc        = npc_pair.second.first;
    int                    npc_amount = npc_pair.second.second;

    for (int c = npc_amount; c != 0; c--) {
      for (const auto& ewar : *npc->Ewar()) {    
        if (ewar->GetType() != EwarModule::Type::RemoteRepair) {
          s_copy->ApplyEffect(ewar);
        } else {
          spawn_remote_rep_total_str_ +=(*ewar->GetEffects())[0]->GetStrength();
        }
      }

      s_copy->Defense()->ApplyDps({string(npc_name), npc->Dps(s_copy)});
    }
  }

  return s_copy;
}

shared_ptr<Npc> SpawnEvaluator::GetTarget(
    const shared_ptr<NpcContainer>& spawn,
    const shared_ptr<Ship>& player_ship,
    const shared_ptr<Ship>& affected_player_ship) 
{
  pair<ScoreType, float> capacitor_reg_score = 
      GetCapactiorRegScore(affected_player_ship, player_ship);

  pair<ScoreType, float> dps_tank_score = 
      GetDpsTankScore(affected_player_ship, player_ship);

  pair<ScoreType, float> dps_score = 
      GetDpsScore(affected_player_ship, player_ship);

  pair<ScoreType, float> lock_range_score = 
      GetLockRangeScore(affected_player_ship, player_ship);

  pair<ScoreType, float> velocity_score = 
      GetVelocityScore(affected_player_ship, player_ship);


  shared_ptr<Npc> target = nullptr;
  float score = 0;

  for (const auto& npc_data : *spawn.get()) {
    vector<float> multipliers{};

    const shared_ptr<Npc>& npc = npc_data.second.first;
    if (lock_range_score.second != 1.0f && 
        npc->Ewar()->FindEwarByType(EwarModule::SensorDampener).size() > 0) {
      multipliers.push_back(lock_range_score.second);
    }

    if (velocity_score.second != 1.0f &&
        npc->Ewar()->FindEwarByType(EwarModule::StasisWebifier).size() > 0) {
      multipliers.push_back(velocity_score.second);
    }

    if (capacitor_reg_score.second != 1.0f &&
        npc->Ewar()->FindEwarByType(EwarModule::EnergyNeutralizer).size() > 0) {
      multipliers.push_back(capacitor_reg_score.second);
    }

    if (dps_tank_score.second != 1.0f && npc->Weapons()->TotalWeapons() > 0) {
      multipliers.push_back(dps_tank_score.second);
    }

    if (dps_score.second != 1.0f && 
        npc->Ewar()->FindEwarByType(EwarModule::RemoteRepair).size() > 0) {
      multipliers.push_back(dps_score.second);
    }

    NpcEvaluator npc_evaulator;
    float new_score = npc_evaulator.Evaulate(npc, player_ship);
    new_score = MultiplyByValuesInArray<float>(new_score, multipliers);

    std::cout << npc->GetName()
          << " Total score: " 
          << new_score 
          << '\n';


    if (new_score > score) {
      score = new_score;
      target = npc;
      std::cout << "Choosed new target. Score: " 
                << score 
                << " Target name: "
                << target->GetName() << '\n';
    }
  }

  std::cout << "Calculated Target: " << target->GetName() << '\n';
  return target;
}

shared_ptr<Npc> SpawnEvaluator::EvaulateTarget(
    const shared_ptr<NpcContainer>& spawn, const shared_ptr<Ship>& player_ship)
{
  shared_ptr<Ship> affected_player_ship = 
      ApplyEffectsToShip(spawn, player_ship);

  shared_ptr<Npc> target = GetTarget(spawn, player_ship, affected_player_ship);

  spawn_remote_rep_total_str_ = 0;

  return target;
}

void SpawnEvaluator::EvaulateSpawn(const shared_ptr<NpcContainer>& spawn,
                                   const shared_ptr<Ship>& player_ship)
{
  shared_ptr<NpcContainer> copy_spawn = spawn->Copy();

  vector<shared_ptr<Npc>> npc_target_sequence_vector;
  npc_target_sequence_vector.reserve(copy_spawn->TotalAmountNpc());

  while(copy_spawn->TotalAmountNpc() != 0) 
  {
    shared_ptr<Npc> npc = EvaulateTarget(copy_spawn, player_ship);
    npc_target_sequence_vector.push_back(npc);
    copy_spawn->DecreaseAmount(npc->GetName());
    printf("\033[1;31mTarget: %s\033[0m\n", npc->GetName().c_str());
  }
}

} // namespace abyss
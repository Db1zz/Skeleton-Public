#include "bot.h"
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

float GetTargetEhp(const shared_ptr<Ship>& target, 
                   const shared_ptr<Ship>& shooter) 
{
  return target->Defense()->Ehp(*shooter->Weapons());
}

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
#define ENERGY_NEUT_MULTIPLIER 6.0f
#define WEBIFIER_MULTIPLIER 0.3f
#define DPS_TANK_MULTIPLIER 2.5f
#define REMOTE_REP_MULTIPLIER 1.2f

float SpawnEvaluator::GetCapactiorRegScore(
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
  return score;
}

float SpawnEvaluator::GetDpsTankScore(const shared_ptr<Ship>& affected_ship,
                                      const shared_ptr<Ship>& ship)
{
  float score = 1;
  float affected_hps = affected_ship->Defense()->HPs();
  float base_hps = ship->Defense()->HPs();

  score += fabs(std::min(0.0f, affected_hps / base_hps))
          * SURVIVIAL_MULTIPLIER * DPS_TANK_MULTIPLIER;

  std::cout << "Dps Tank Score: " << score << '\n';
  return score;
}

float SpawnEvaluator::GetDpsScore(const shared_ptr<Ship>& affected_ship,
                                  const shared_ptr<Ship>& ship)
{
  float dps = affected_ship->Dps(affected_ship);
  printf("Dps: %f\n", dps);

  float new_dps = dps - spawn_remote_rep_total_str_;
  printf("NewDps: %f \n", new_dps);

  float score = dps / new_dps;
  if (spawn_remote_rep_total_str_ > 0) {
    if (score != 1.0f) {
      if (score >= 2) {
        score *= TIME_MULTIPLIER * REMOTE_REP_MULTIPLIER * 20;
      }
    }
  }

  std::cout << "Dps Score: " << score << '\n';
  return score;
}

float SpawnEvaluator::GetLockRangeScore(const shared_ptr<Ship>& affected_ship,
                                        const shared_ptr<Ship>& ship) 
{
  float score = affected_ship->Targeting()->Range() / 
                ship->Targeting()->Range();
  
  float velocity_score = GetVelocityScore(affected_ship, ship);

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
  return score;
}

float SpawnEvaluator::GetVelocityScore(const shared_ptr<Ship>& affected_ship,
                                       const shared_ptr<Ship>& ship)
{
  float score = 1;

  if (affected_ship->Engine()->Velocity() != 
      ship->Engine()->Velocity()) {
    float temp = affected_ship->Engine()->Velocity() /
                 ship->Engine()->Velocity();
    if (temp != 1.0f) {
      score += (temp * WEBIFIER_MULTIPLIER) + 
               (spawn_lock_range_score_);
    }
  }

  std::cout << "Velocity Score: " << score << '\n';
  return score;
}


// TODO: I don't like the name of function and it's body.
shared_ptr<Bot> SpawnEvaluator::ApplyEffectsToShip(
    const shared_ptr<NpcContainer>& spawn,
    const shared_ptr<Bot>& bot_ship)
{
  // Create the ship copy.
  shared_ptr<Bot> s_copy = bot_ship->Copy();

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

// shared_ptr<Npc> SpawnEvaluator::GetTarget(
//     const shared_ptr<NpcContainer>& spawn,
//     const shared_ptr<Ship>& player_ship,
//     const shared_ptr<Ship>& affected_player_ship) 
// {
//   float capacitor_reg_score = 
//       GetCapactiorRegScore(affected_player_ship, player_ship);

//   float dps_tank_score = GetDpsTankScore(affected_player_ship, player_ship);

//   float dps_score = GetDpsScore(affected_player_ship, player_ship);

//   float lock_range_score = GetLockRangeScore(affected_player_ship, player_ship);

//   float velocity_score = GetVelocityScore(affected_player_ship, player_ship);

//   shared_ptr<Npc> target = nullptr;
//   float score = 0;

//   for (const auto& npc_data : *spawn.get()) {
//     vector<float> multipliers{};

//     const shared_ptr<Npc>& npc = npc_data.second.first;
//     if (lock_range_score != 1.0f && 
//         npc->Ewar()->FindEwarByType(EwarModule::SensorDampener).size() > 0) {
//       multipliers.push_back(lock_range_score);
//     }

//     if (velocity_score != 1.0f &&
//         npc->Ewar()->FindEwarByType(EwarModule::StasisWebifier).size() > 0) {
//       multipliers.push_back(velocity_score);
//     }

//     if (capacitor_reg_score != 1.0f &&
//         npc->Ewar()->FindEwarByType(EwarModule::EnergyNeutralizer).size() > 0) {
//       multipliers.push_back(capacitor_reg_score);
//     }

//     if (dps_tank_score != 1.0f && npc->Weapons()->TotalWeapons() > 0) {
//       multipliers.push_back(dps_tank_score);
//     }

//     if (dps_score != 1.0f && 
//         npc->Ewar()->FindEwarByType(EwarModule::RemoteRepair).size() > 0) {
//       multipliers.push_back(dps_score);
//     }

//     NpcEvaluator npc_evaulator;
//     float new_score = npc_evaulator.Evaulate(npc, player_ship);
//     new_score = MultiplyByValuesInArray<float>(new_score, multipliers);

//     // TODO: Implement Application algorithms to get more precise score;

//     std::cout << npc->GetName()
//           << " Total score: " 
//           << new_score 
//           << '\n';


//     if (new_score > score) {
//       score = new_score;
//       target = npc;
//       std::cout << "Choosed new target. Score: " 
//                 << score 
//                 << " Target name: "
//                 << target->GetName() << '\n';
//     }
//   }

//   std::cout << "Calculated Target: " << target->GetName() << '\n';
//   return target;
// }

shared_ptr<Npc> SpawnEvaluator::GetTarget(
    const shared_ptr<NpcContainer>& spawn,
    const shared_ptr<Bot>& bot_ship,
    const shared_ptr<Bot>& affected_bot_ship) 
{
  float capacitor_reg_score = 
      GetCapactiorRegScore(affected_bot_ship, bot_ship);

  float dps_tank_score = GetDpsTankScore(affected_bot_ship, bot_ship);

  float dps_score = GetDpsScore(affected_bot_ship, bot_ship);

  float lock_range_score = GetLockRangeScore(affected_bot_ship, bot_ship);

  float velocity_score = GetVelocityScore(affected_bot_ship, bot_ship);

  shared_ptr<Npc> target = nullptr;
  float score = 0;

  for (const auto& npc_data : *spawn.get()) {
    const shared_ptr<Npc>& npc = npc_data.second.first;
    float curr_score = 0;
    float target_ehp = GetTargetEhp(npc, affected_bot_ship);

    vector<EwarModule*> dampeners = 
        npc->Ewar()->FindEwarByType(EwarModule::SensorDampener);
    if (lock_range_score != 1.0f && dampeners.size() > 0) {
      float damp_str = (*dampeners[0]->GetEffects())[0]->GetStrength();

      curr_score += lock_range_score * damp_str / target_ehp;
    }

    vector<EwarModule*> webifiers = 
        npc->Ewar()->FindEwarByType(EwarModule::StasisWebifier);
    if (velocity_score != 1.0f && webifiers.size() > 0) {
      float web_str = (*webifiers[0]->GetEffects())[0]->GetStrength();
      
      curr_score += velocity_score * web_str / target_ehp;
    }

    vector<EwarModule*> neuts =
        npc->Ewar()->FindEwarByType(EwarModule::EnergyNeutralizer);
    if (capacitor_reg_score != 1.0f && neuts.size() > 0) {
      float neut_str = (*neuts[0]->GetEffects())[0]->GetStrength();
      
      curr_score += capacitor_reg_score * neut_str / target_ehp;
    }

    vector<EwarModule*> remote_repairs = 
        npc->Ewar()->FindEwarByType(EwarModule::RemoteRepair);
    if (dps_score != 1.0f && remote_repairs.size() > 0) {
      float rr_str = (*remote_repairs[0]->GetEffects())[0]->GetStrength();

      curr_score += dps_score * rr_str / target_ehp;
    }

    float npc_weapon_dps = npc->Dps(affected_bot_ship);
    if (dps_tank_score != 1.0f && npc_weapon_dps > 0) {
      float npc_weapon_application = 0;
      curr_score += dps_tank_score * (npc_weapon_dps * npc_weapon_application) / target_ehp;
    }

    curr_score *= 100000;
    // NpcEvaluator npc_evaulator;
    // float new_score = npc_evaulator.Evaulate(npc, player_ship);

    // TODO: Implement Application algorithms to get more precise score;

    std::cout << npc->GetName()
          << " Total score: " 
          << curr_score 
          << '\n';


    if (curr_score > score) {
      score = curr_score;
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
    const shared_ptr<NpcContainer>& spawn, const shared_ptr<Bot>& bot_ship)
{
  shared_ptr<Ship> affected_bot_ship = 
      ApplyEffectsToShip(spawn, bot_ship);

  shared_ptr<Npc> target = GetTarget(spawn, bot_ship, affected_bot_ship);

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
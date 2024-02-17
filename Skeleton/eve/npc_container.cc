#include "npc_container.h"

#include <iostream>

namespace eve {

using std::string;
using std::cout;

NpcContainer::~NpcContainer() {}

shared_ptr<Npc> NpcContainer::GetNpcDetails(const string& npc_name)  {
  auto npc_it = npc_container_.find(npc_name);
  if (npc_it == npc_container_.end()) {
    return nullptr;
  } 
    
  return npc_it->second.first;
}

int* NpcContainer::GetNpcAmount(const string& npc_name) {
  auto npc_it = npc_container_.find(npc_name);
  if (npc_it == npc_container_.end()) {
    return nullptr;
  }
  return &npc_it->second.second;
}

bool NpcContainer::AddNpc(const string& npc_name,
                          NpcContainer& npc_container) {
  if (!npc_container.IsExist(npc_name)) {
    cout << "Error: NPC is not exist in NPC dictionary \n";
    return false;
  }

  const shared_ptr<Npc>& npc = npc_container.GetNpcDetails(npc_name);
  
  return AddNpc(npc);
}

bool NpcContainer::AddNpc(const shared_ptr<Npc>& npc_data) {
  const string& npc_name = npc_data->GetName();
  int amount = 1;

  if (IsExist(npc_name)) {
    IncreaseAmount(npc_name, amount);
    return true;
  }

  npc_container_.insert({npc_name, {npc_data, amount}});
  total_npc_amount_ += amount;
  return true;
}

bool NpcContainer::RemoveNpc(const string& npc_name) {
  // Check if NPC exist in npc_container_
  if (!IsExist(npc_name)) {
    cout << "Cannot delete NPC that not exist \n";
    return false;
  }

  // Decrease total_npc_amount_ by amount of npc_name
  total_npc_amount_ -= *GetNpcAmount(npc_name);

  // Erase NPC from npc_container_
  npc_container_.erase(npc_name);
  return true;
}

bool NpcContainer::IncreaseAmount(const string& npc_name, int amount) {
  if (!IsExist(npc_name)) {
    cout << "Error: Cannot increase amount of NPC that not exist \n";
    return false;
  }

  int* npc_amount = GetNpcAmount(npc_name);
  *npc_amount += amount;
  total_npc_amount_ += amount;

  return true;
}

bool NpcContainer::DecreaseAmount(const string& npc_name, int amount) {

  if (!IsExist(npc_name)) {
    cout << "Error: Cannot decrease amount of NPC that not exist \n";
    return false;
  }
  
  int* npc_amount = GetNpcAmount(npc_name);
  npc_amount -= amount;
  total_npc_amount_ -= amount;

  if (*npc_amount <= 0) {
    RemoveNpc(npc_name);
  }

  return true;
}

bool NpcContainer::IsExist(const string& npc_name)  {
  return GetNpcAmount(npc_name) != nullptr;
}

} // namespace eve
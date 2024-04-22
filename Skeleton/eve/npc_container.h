#ifndef NPC_CONTAINER_H_
#define NPC_CONTAINER_H_

#include "npc.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace eve {

using std::pair;
using std::shared_ptr;
using std::string_view;

class NpcContainer {
public:
  NpcContainer() = default;

  virtual ~NpcContainer();

  // Returns pointer to Npc in npc_container_
  // if NPC not exist in npc_container function will
  // return nullptr.
  virtual shared_ptr<Npc> GetNpcDetails(const string &npc_name);

  // Returns pointer to the variable that holds amount of NPC.
  // Will return nullptr if NPC doesn't exist in container_.
  virtual int *GetNpcAmount(const string &npc_name);

  // Returns true if NPC exist in npc_container_ or
  // false if not.
  virtual bool IsExist(const string &npc_name);

  // Returns total_npc_amount_.
  virtual inline int TotalAmountNpc() { return total_npc_amount_; }

  // Returns true if total_npc_amount_ > 0
  // or false if total_npc_amount_ == 0.
  virtual inline bool Empty() { return total_npc_amount_ == 0; }

  // Returns true if NPC was successfully added
  // in npc_container_. Will return false if NPC does
  // not exist in npc_dictionary or NPC already exist
  // in npc_container_.
  virtual bool AddNpc(const string &npc_name, NpcContainer &npc_dictionary);

  // Returns true if NPC was successfully added in npc_container_,
  // will NOT return false if NPC already exist in npc_container_
  // but adds to the amount of NPC by 1
  virtual bool AddNpc(const shared_ptr<Npc> &npc_data);

  // Returns true if NPC was successfully added in npc_container_,
  // will NOT return false if NPC already exist in npc_container_
  // but adds to the amount of NPC by 1
  virtual bool AddNpc(const shared_ptr<Npc> &npc_data, int npc_amount);

  // RemoveNpc completely removes specifc NPC
  // from npc_container_. If NPC does not exist in
  // npc_container_ function will return false.
  virtual bool RemoveNpc(const string &npc_name);

  // IncreaseAmount simply increases the amount of specific NPC.
  virtual bool IncreaseAmount(const string &npc_name, int amount = 1);

  // DecreaseAmount decreases amount of specific NPC in
  // Npc and if amount of NPC's == 0 this function
  // will automaticaly delete the ship from npc_container_.
  virtual bool DecreaseAmount(const string &npc_name, int amount = 1);

  // Allocates new memory space
  // and Copies all Npc's from Container A to Container B.
  virtual shared_ptr<NpcContainer> Copy() const;

  // Returns iterator of the beginning npc_container_.
  inline auto begin() { return npc_container_.begin(); }
  // Returns iterator of the end npc_container_.
  inline auto end() { return npc_container_.end(); }

protected:
  // total_npc_amount_ can be increased and decreased by
  // IncreaseAmount and DecreaseAmount functions.
  int total_npc_amount_{0};

  // This is the main variable of class, we can add and remove
  // NPC' by using AddNpc and RemoveNpc functions.
  // Note: DO NOT modify this array without using class methods.
  std::unordered_map<string_view, pair<shared_ptr<Npc>, int>> npc_container_;
};

} // namespace eve

#endif // NPC_CONTAINER_H_

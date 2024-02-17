#ifndef SPAWNS_H_
#define SPAWNS_H_

#include "../Skeleton/abyss_bot/npc_builder.h"
#include "../Skeleton/eve/npc_container.h"

#include <memory>
#include <utility>
#include <initializer_list>

namespace spawns {

using namespace abyss;

using std::initializer_list;
using std::string;
using std::make_unique;
using std::dynamic_pointer_cast;
using std::static_pointer_cast;

static shared_ptr<NpcContainer> NPC_DICTIONARY = AbyssNpcBuilder::Build();

// This function accepts a list of names and creates new NpcContainer
shared_ptr<NpcContainer> BuildSpawn(initializer_list<string> npc_list) {
  shared_ptr<NpcContainer> container = make_unique<NpcContainer>();

  for (const auto& npc : npc_list) {
    shared_ptr<Npc> npc_ship 
        = static_pointer_cast<Npc>(NPC_DICTIONARY->GetNpcDetails(npc)->Copy());

    if (!npc_ship) continue; 
    else container->AddNpc(npc_ship);
  }
  return container;
}

} // namespace spawns

#endif // SPAWNS_H_
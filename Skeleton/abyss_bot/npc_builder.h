#ifndef NPC_BUILDER_H_
#define NPC_BUILDER_H_

#include "../db/db_loader.h"
#include "../eve/npc.h"
#include "../eve/type_converter.h"
#include "../eve/npc_container.h"
#include "npc_ship.h"

#include <iostream>
#include <utility>
namespace abyss {

using std::vector;
using std::string;
using namespace eve;

class AbyssNpcBuilder {
  public:
    static unique_ptr<ShipEngine> BuildEngine(const vector<string>& s);

    static unique_ptr<ShipCapacitor> BuildCapacitor(const vector<string>& s);

    static unique_ptr<ShipTargeting> BuildTargeting(const vector<string>& s);

    static unique_ptr<ShipDefense> BuildDefense(const vector<string>& s);

    static shared_ptr<EwarModule> BuildEwarModule(
        const vector<string>& s, int type_index);

    static shared_ptr<EwarModule> BuildRemoteRepairModule(
      const vector<string>& s);

    static shared_ptr<NpcContainer> Build();
};
} // namespace abyss

#endif // NPC_BUILDER_H_
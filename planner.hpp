#include "actions.hpp"
#include <utility>
#include <queue>
#include <map>
#include <unordered_map>

using ActionCondition = std::pair<Conditions, ActionPtr>;
using ActionConditions = std::set<ActionCondition>;//,  ActionConditionLessThan>;

class Planner {
  Actions actions;
  Conditions postConds;
  Conditions preConds;
  std::vector<Conditions> mutexes;
public:
  void AddAction(ActionPtr);
  void SetPostConditions(Conditions);
  void SetPreConditions(Conditions);
  void AddMutex(Conditions);

  ActionConditions findActions(Actions actions, Conditions preConditions);
  
  ActionPlan Plan();
  ActionPlan Plan2();
};

struct ActionNode{
  ActionPtr action;
  Conditions conds;
  int cost_so_far;
  std::shared_ptr<ActionNode> parent;
};

using ActionNodePtr = std::shared_ptr<ActionNode>;

inline bool operator==(const ActionNodePtr& l, const ActionNodePtr& r ){
  return l->action->getName() == r->action->getName()
    && l->conds == r->conds;   
  
}


template<typename T, typename Number=int>
struct PriorityQueue {
  typedef std::pair<Number, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>,
                 std::greater<PQElement>> elements;
  
  inline bool empty() { return elements.empty(); }
  
  inline void put(T item, Number priority) {
    elements.emplace(priority, item);
  }

  inline T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

// struct ActionGraph{
//   std::unordered_map<Conditions, Actions, ConditionsHash> edges;
//   inline const  Actions neighbours(Conditions a){
//     // Actions acs;
//     // for (auto& e : edges)
//     //   if(std::includes(a.begin(), a.end(), e.first.begin(), e.first.end()))
//     // 	acs.insert(e.second.begin(), e.second.end());
//     // return acs;
//     return edges[a];
//   }
// };

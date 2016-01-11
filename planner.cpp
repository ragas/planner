#include "planner.hpp"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <utility>

void Planner::AddAction(ActionPtr action) {
  actions.insert(action);
}

void Planner::SetPostConditions(Conditions conds){
  postConds = conds;
}

void Planner::SetPreConditions(Conditions conds){
  preConds = conds;
}

void Planner::AddMutex(Conditions mutex)
{
  mutexes.push_back(mutex);
}


ActionConditions Planner::findActions(Actions actions, Conditions preConditions){
  ActionConditions result;
  for(auto& a: actions){
    Conditions postA = a->getPreConditions();
    if(satisfies(postA, preConditions)){
      
      Conditions yetToSatisfy;
    
      std::set_difference(preConditions.begin(),preConditions.end(),
			  postA.begin(), postA.end(),
			  std::inserter(yetToSatisfy, yetToSatisfy.begin())
			  );
      Conditions combined;
      Conditions preA = a->getPostConditions();

      std::set_union(preA.begin(), preA.end(),
		     yetToSatisfy.begin(), yetToSatisfy.end(),
		     std::inserter(combined, combined.begin()));
      bool c = false;
      for(auto& m : mutexes)
	if(satisfies(combined, m)){
	  c=true;break;
	}
      if(!c)
	result.insert({combined,a});
    }      
    
  }
  return result;
}


ActionPlan Planner::Plan2(){
  ActionPlan result;
  auto curr = preConds;
  auto goal = postConds;

  ActionPtr nullAction = Action::ActionFactory("NULL",{},{},0,0);
  ActionCondition start = {curr, nullAction};

  PriorityQueue<ActionNodePtr> acs;
  auto curr_ac  = std::make_shared<ActionNode>();
  curr_ac->action = nullAction;
  curr_ac->conds = curr;
  curr_ac->cost_so_far = 0;
  curr_ac->parent = nullptr;
  
  acs.put(curr_ac,0);
  bool success = false;
  while(!acs.empty()){
    curr_ac  = acs.get();

    if(!satisfies(curr_ac->conds, goal)){
      for(auto& ac : findActions(actions, curr_ac->conds)){
	bool skip = false;
	auto temp_ac = curr_ac;
	while(temp_ac->action->getName() != nullAction->getName()){
	  temp_ac = temp_ac->parent;
	  if(ac.first == temp_ac->conds){
	    skip=true;
	    break;
	  }
	}
	if(skip)
	  continue;
	auto node = std::make_shared<ActionNode>();
	node->action = ac.second;
	node->conds = ac.first;
	node->cost_so_far = ac.second->getCost()+curr_ac->cost_so_far;
	node->parent = curr_ac;
	acs.put(node,node->cost_so_far);
      }
    }
    else{
      success=true;
      break;
    }
  }
  std::cout <<"PLANNED " << std::boolalpha << success << "\n";
  if(!success)
    return result;
  while(curr_ac != nullptr){
    result.push_back(curr_ac->action);
    //std::cout << curr_ac->action << " ";
    curr_ac = curr_ac->parent;
  }
     
  std::reverse(result.begin(), result.end());
  return result;
}

ActionPlan Planner::Plan(){
  ActionPlan result;

  return result;
}

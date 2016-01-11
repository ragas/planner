#include "actions.hpp"
#include "planner.hpp"
#include <cassert>
#include <iostream>

void Complex();
void farmerPuzzle(){
  Condition BoatOnLeft		= Condition("BoatOnLeft", true);
  Condition BoatOnRight		= Condition("BoatOnLeft", false);
  Condition GoatOK		= Condition("GoatOK", true);
  Condition GoatDead		= Condition("GoatOK", false);
  Condition CabbageOK		= Condition("CabbageOK", true);
  Condition CabbageDead		= Condition("CabbageOK", false);
      
  Condition GoatOnLeft		= Condition("GoatOnLeft", true);
  Condition CabbageOnLeft	= Condition("CabbageOnLeft", true);
  Condition WolfOnLeft		= Condition("WolfOnLeft", true);
  
  Condition GoatOnRight		= Condition("GoatOnLeft", false);
  Condition CabbageOnRight	= Condition("CabbageOnLeft", false);
  Condition WolfOnRight		= Condition("WolfOnLeft", false);

  auto ActionTravelRight1	= Action::ActionFactory("TravelRight1", {BoatOnLeft}, {BoatOnRight}, 1,1);
  auto ActionTravelLeft1	= Action::ActionFactory("TravelLeft1", {BoatOnRight}, {BoatOnLeft}, 1,1);

  auto ActionTravelGoatRight	= Action::ActionFactory("TravelGoatRight", {BoatOnLeft, GoatOnLeft}, {BoatOnRight,GoatOnRight}, 1,1);
  auto ActionTravelCabbageRight = Action::ActionFactory("TravelCabbageRight", {BoatOnLeft, CabbageOnLeft}, {BoatOnRight, CabbageOnRight}, 1,1);
  auto ActionTravelWolfRight	= Action::ActionFactory("TravelWolfRight", {BoatOnLeft, WolfOnLeft}, {BoatOnRight, WolfOnRight}, 1,1);

  auto ActionTravelGoatLeft	= Action::ActionFactory("TravelGoatLeft", {BoatOnRight, GoatOnRight}, {BoatOnLeft, GoatOnLeft}, 1,1);  
  auto ActionTravelCabbageLeft	= Action::ActionFactory("TravelCabbageLeft", {BoatOnRight, CabbageOnRight}, {BoatOnLeft, CabbageOnLeft}, 1,1);
  auto ActionTravelWolfLeft	= Action::ActionFactory("TravelWolfLeft", {BoatOnRight, WolfOnRight}, {BoatOnLeft, WolfOnLeft}, 1,1);


  Conditions init	= {BoatOnLeft, GoatOnLeft, CabbageOnLeft, WolfOnLeft, GoatOK, CabbageOK};
  Conditions goal	= {BoatOnRight, GoatOnRight, CabbageOnRight, WolfOnRight, GoatOK, CabbageOK};

  Conditions mutex1	= {BoatOnRight, GoatOnLeft, CabbageOnLeft};
  Conditions mutex2	= {BoatOnRight, GoatOnLeft, WolfOnLeft};
  Conditions mutex3	= {BoatOnLeft, GoatOnRight, CabbageOnRight};
  Conditions mutex4	= {BoatOnLeft, GoatOnRight, WolfOnRight};
  
  Planner p;
  p.SetPostConditions(goal);
  p.SetPreConditions(init);
  p.AddMutex(mutex1);
  p.AddMutex(mutex2);
  p.AddMutex(mutex3);
  p.AddMutex(mutex4);
  
  p.AddAction(ActionTravelRight1);
  p.AddAction(ActionTravelLeft1);

  p.AddAction(ActionTravelGoatRight);
  p.AddAction(ActionTravelWolfRight);
  p.AddAction(ActionTravelCabbageRight);

  p.AddAction(ActionTravelGoatLeft);
  p.AddAction(ActionTravelWolfLeft);
  p.AddAction(ActionTravelCabbageLeft);
  
  auto plan = p.Plan2();
  std::cout << "\n";
  for(auto& p : plan){
    if(p!= nullptr)
      std::cout << p->getName() << "\t";
  }

  std::cout << "\n";

  
}


int main(){
  farmerPuzzle();
  return 0;
}

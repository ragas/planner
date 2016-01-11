#include "actions.hpp"
#include <algorithm>
#include <utility>

Condition::Condition(std::string n, bool s):name(n), status(s), priority(0){
}

bool Condition::operator<(const Condition& c) const{
  return name < c.name;
}

bool Condition::operator!() const{
  return !status;
}

Condition Condition::operator~() const{
  Condition n = *this;
  n.status = !status;
  return n;
}

std::string Condition::getName() const{
  return name;
}

bool Condition::operator==(const Condition& o) const{
  return (name == o.name) && (status == o.status);
}

std::size_t IAction::id = 0;
IAction::IAction(std::string n):name(n),myId(id++){
}

std::size_t IAction::getID() const{
  return myId;
}

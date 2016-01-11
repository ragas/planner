#pragma once
#include <numeric>
#include <string>
#include <memory>
#include <unordered_set>
#include <set>
#include <functional>
#include <vector>
#include <iterator>
#include <iostream>

class Condition {
  friend class ConditionHash;
  friend class ConditionEqual;
  std::string name;
  bool status;
public:
  int priority;
  explicit Condition(std::string n, bool s);
  // Condition(const Condition&);
  // Condition& operator=(const Condition&);
  std::string getName() const;
  bool operator!() const;
  bool operator==(const Condition&) const;
  Condition operator~() const;
  bool operator<(const Condition&) const;
};

struct ConditionHash {
  inline std::size_t operator() (const Condition& c) const{
    return std::hash<std::string>()(c.name);
  }
};

// struct ConditionEqual{
//   bool operator()(const Condition& lhs, const Condition& rhs ) const{
//     return lhs.name == rhs.name;
//   }
// };

// struct ConditionLessThan{
//   bool operator()(const Condition& l, const Condition& r) const{
//     return l.getName() < r.getName();
//   }  
// };

//using Conditions = std::unordered_set<Condition, ConditionHash, ConditionEqual>;
using Conditions = std::set<Condition>;

inline std::ostream& operator<<(std::ostream& os, const Conditions& conds){
  for(auto& c : conds)
    os << c.getName() << " " << std::boolalpha << !!c << " ";
  return os;
}

inline bool satisfies(const Conditions& first, const Conditions& second){
  Conditions common;
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(common, common.begin()));
  if(common.empty())
    return false;
  for(auto& c : common)
    if(! (*(first.find(c)) == *(second.find(c))) )
      return false;
  return true;
}

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template<>
inline void hash_combine<Condition>(std::size_t& seed, const Condition& v)
{
  std::hash<std::string> hasher;
  seed ^= hasher(v.getName()) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct ConditionsHash {
  std::size_t operator() (const Conditions& conds) const{
    std::size_t seed = 0;
    for(auto& c : conds)
      hash_combine(seed, c);
    return seed;
  }
};

class IAction{
protected:
  std::string name;
  static std::size_t id;
  const std::size_t myId;
  
public:
  explicit IAction(std::string n);
  std::string getName() const {return name;}
  std::size_t getID() const;
  virtual Conditions getPreConditions() const	= 0;
  virtual Conditions getPostConditions() const	= 0;
  virtual int getCost() const			= 0;  
  virtual ~IAction(){
  }
};

inline std::ostream& operator<<(std::ostream& os, const IAction& ia){
  os << ia.getName();
  return os;
}

using ActionPtr = std::shared_ptr<IAction>;

inline std::ostream& operator<<(std::ostream& os, const ActionPtr& ia){
  os << ia->getName();
  return os;
}

struct ActionLessThan{
  bool operator()(const ActionPtr& l, const ActionPtr& r){
    return l->getName() < r->getName();
  }  
};

using Actions =  std::set<ActionPtr, ActionLessThan>;
using ActionPlan = std::vector<ActionPtr>;
// template <typename T>
// class Condition: public Condition{
// };

// template <>
// class Condition<bool> : public Condition {
//   bool state;
// public:
//   Condition(std::string name, bool s):Condition(name),state(s){}
//   virtual explicit operator bool() override{
//     return state;
//   }  
// };


class Action : public IAction{
  int cost;
  Conditions pre,post;
  Action(std::string name):IAction(name){};

public:
  virtual Conditions getPreConditions() const override {
    return pre;
  }
  virtual Conditions getPostConditions() const override {
    return post;
  }
  virtual int getCost() const override {
    return cost;
  }  
  
  static ActionPtr  ActionFactory(std::string name,
				  Conditions pre, 
				  Conditions post,
				  int cost, int duration){
    auto u = std::unique_ptr<Action>(new Action(name));
    u->pre = pre;
    u->post = post;
    u->cost = cost;
    
    return std::move(u);
  }
};

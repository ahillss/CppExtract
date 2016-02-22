#ifndef VISITOR
#define VISITOR

#include <llvm/Support/Casting.h>

//visitor
template<typename Ret, typename Base, typename... Rest>
class Visitor;

template<typename Ret, typename Base, typename First, typename... Rest>
class Visitor<Ret,Base,First, Rest...>
  : public Visitor<Ret,Base,Rest ...>
{
public:
  virtual ~Visitor() {}
  using Visitor<Ret,Base,Rest...>::visit;

  Ret acceptMe(Base *t) {
    if(First *tt=
       // dynamic_cast<First*>
       clang::dyn_cast<First>
       (t)) {
      return visit(tt);
    } else {
      return Visitor<Ret,Base,Rest...>::acceptMe(t);
    }
  }

  virtual Ret visit(First *t)=0;
};

template<typename Ret, typename Base, typename First>
class Visitor<Ret,Base,First>
{
public:
  virtual ~Visitor() {}

  Ret acceptMe(Base *t) {
    if(First *tt=
       // dynamic_cast<First*>
       clang::dyn_cast<First>
       (t)) {
      return visit(tt);
    } else {
      return visit(t);
    }
  }

  virtual Ret visit(First *t)=0;
  virtual Ret visit(Base *t)=0;
};

template<typename Ret, typename Base>
class Visitor<Ret,Base>
{
public:
  virtual ~Visitor() {}

  Ret acceptMe(Base *t) {
    return visit(t);
  }

  virtual Ret visit(Base *t)=0;
};

//visitors

template<typename... Rest>
class Visitors;

template<typename First, typename... Rest>
  class Visitors<First, Rest...>
  : public Visitors<Rest ...>, public First
{
public:
  virtual ~Visitors() {}
  using Visitors<Rest ...>::acceptMe;
  using First::acceptMe;
};

template<typename First>
class Visitors<First> : public First {
public:
  virtual ~Visitors() {}
  using First::acceptMe;
};

#endif

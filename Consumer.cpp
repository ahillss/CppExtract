#include "Consumer.h"

Consumer::Consumer() {
}

bool Consumer::HandleTopLevelDecl(clang::DeclGroupRef dg) {
  for(clang::DeclGroupRef::iterator i=dg.begin();i!=dg.end();i++) {
    decls.push_back(*i);

  }

  return true;
}

std::list<clang::Decl*>::iterator Consumer::declsBegin() {
  return decls.begin();
}

std::list<clang::Decl*>::iterator Consumer::declsEnd() {
  return decls.end();
}

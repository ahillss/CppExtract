
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclGroup.h>

#include <list>

class Consumer : public clang::ASTConsumer {
private:
  std::list<clang::Decl*> decls;
public:
  Consumer();
  bool HandleTopLevelDecl(clang::DeclGroupRef dg);
  std::list<clang::Decl*>::iterator declsBegin();
  std::list<clang::Decl*>::iterator declsEnd();
};

#include "XmlVisitor.h"

XmlVisitor::XmlVisitor(clang::PrintingPolicy &printPolicy,
                       clang::ASTContext &astContext)
  : printPolicy(printPolicy),astContext(astContext)
{
}

XmlNode *XmlVisitor::visit(clang::Decl *d) {
  XmlNode *node=new XmlNode("todo_decl");
  node->setAttrib("kind",d->getDeclKindName());

  if(clang::NamedDecl *nd=clang::dyn_cast<clang::NamedDecl>(d)) {
    node->setAttrib("name",nd->getNameAsString());
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::AccessSpecDecl *d) {
  clang::AccessSpecifier access=d->getAccess();
  std::string type;

  if(access==clang::AS_public) {
    type="public";
  } else if(access==clang::AS_protected) {
    type="protected";
  } else if(access==clang::AS_private) {
    type="private";
  }

  XmlNode *node=new XmlNode(type);
  return node;
}

XmlNode *XmlVisitor::visit(clang::FriendDecl *d) {
  clang::QualType q=d->getFriendType()->getType();
  XmlNode *node=new XmlNode("friend");
  node->addNode(acceptMe(q.getTypePtr()));
  return node;
}

// XmlNode *XmlVisitor::visit(clang::FriendTemplateDecl *d) {
//   XmlNode *node=new XmlNode("friend_template");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(clang::LinkageSpecDecl *d) {
  XmlNode *node=new XmlNode("extern");

  if(d->getLanguage()== clang::LinkageSpecDecl::LanguageIDs::lang_c) {
    node->setAttrib("lang","c");
  } else if(d->getLanguage()==
            clang::LinkageSpecDecl::LanguageIDs::lang_cxx) {
    node->setAttrib("lang","cxx");
  }

  for(clang::DeclContext::decl_iterator i=d->decls_begin();
      i!=d->decls_end();i++) {
    XmlNode *childNode=acceptMe(*i);

    if(childNode) {
      node->addNode(childNode);
    }
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::LabelDecl *d) {
  XmlNode *node=new XmlNode("label");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::NamespaceAliasDecl *d) {
  XmlNode *node=new XmlNode("namespace_alias");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::NamespaceDecl *d) {
  XmlNode *node=new XmlNode("namespace");
  node->setAttrib("name", d->getNameAsString());

  for(clang::DeclContext::decl_iterator i=d->decls_begin();
      i!=d->decls_end();i++) {
    XmlNode *childNode=acceptMe(*i);

    if(childNode) {
      node->addNode(childNode);
    }
  }

  return node;
}

// XmlNode *XmlVisitor::visit(clang::ClassTemplateDecl *d) {
//   XmlNode *node=new XmlNode("template");
//   node->addNode(acceptMe(d->getTemplatedDecl()));
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(clang::FunctionTemplateDecl *d) {
//   XmlNode *node=new XmlNode("template");
//   node->addNode(acceptMe(d->getTemplatedDecl()));
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(clang::TypeAliasTemplateDecl *d) {
//   XmlNode *node=new XmlNode("template");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(clang::TemplateTemplateParmDecl *d) {
//   XmlNode *node=new XmlNode("template_template_parm");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(clang::EnumDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  std::string n = d->getNameAsString();

  XmlNode *node=new XmlNode("enum");
  node->setAttrib("name",n);

  for(clang::EnumDecl::enumerator_iterator i=d->enumerator_begin();
      i!=d->enumerator_end();i++) {
    node->addNode(acceptMe(*i));
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::CXXRecordDecl *d) {
  if(d->getPreviousDecl() && d->decls_begin() == d->decls_end()) {
    return 0;
  }

  //
  std::string type;

  if(d->isStruct()) {
    type="struct";
  } else if(d->isUnion()) {
    type="union";
  } else if(d->isClass()) {
    type="class";
  }

  std::string name=d->getNameAsString();

  if(name.empty()) {
    name=std::to_string(uintptr_t(d));
  }

  XmlNode *node=new XmlNode(type);
  node->setAttrib("name",name);

  if(d->decls_begin() != d->decls_end()) {
    const clang::ASTRecordLayout &layout=astContext.getASTRecordLayout(d);
    std::string size=std::to_string(layout.getSize().getQuantity());
    node->setAttrib("size",size);

    if(d->isAbstract()) {
      node->setAttrib("abstract","true");
    }

    if(d->isPolymorphic()) {
      node->setAttrib("polymorphic","true");
    }

    for(clang::CXXRecordDecl::base_class_iterator i=d->bases_begin();
        i!=d->bases_end();i++) {
      XmlNode *baseNode=node->addNode(new XmlNode("base"));

      clang::CXXBaseSpecifier b=*i;
      clang::QualType q=b.getType();
      clang::AccessSpecifier access=b.getAccessSpecifier();

      if(access==clang::AS_protected) {
        baseNode->setAttrib("protected","true");
      } else if(access==clang::AS_private) {
        baseNode->setAttrib("private","true");
      }

      if(b.isVirtual()) {
        baseNode->setAttrib("virtual","true");
      }

      baseNode->addNode(acceptMe(q.getTypePtr()));
    }

    for(clang::DeclContext::decl_iterator i=++d->decls_begin();i!=d->decls_end();i++) {
      clang::Decl *dd=*i;
      if(dd) {
        node->addNode(acceptMe(dd));
      }
    }
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::RecordDecl *d) {
  if(d->getPreviousDecl() && d->decls_begin() == d->decls_end()) {
    return 0;
  }

  std::string type;

  if(d->isStruct()) {
    type="struct";
  } else if(d->isUnion()) {
    type="union";
  }

  std::string name=d->getNameAsString();

  if(name.empty()) {
    name=std::to_string(uintptr_t(d));
  }

  XmlNode *node=new XmlNode(type);
  node->setAttrib("name",name);

  if(d->decls_begin() != d->decls_end()) {
    const clang::ASTRecordLayout &layout=astContext.getASTRecordLayout(d);
    std::string size=std::to_string(layout.getSize().getQuantity());
    node->setAttrib("size",size);
  }

  for(clang::DeclContext::decl_iterator i=d->decls_begin();
      i!=d->decls_end();i++) {
    if(*i) {
      node->addNode(acceptMe(*i));
    }
  }

  return node;
}

// XmlNode *XmlVisitor::visit(clang::ClassTemplateSpecializationDecl *d) {
//   XmlNode *node=new XmlNode("template_special");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(clang::ClassTemplatePartialSpecializationDecl *d) {
//   XmlNode *node=new XmlNode("partial_template_special");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(clang::TemplateTypeParmDecl *d) {
//   XmlNode *node=new XmlNode("template_parm");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(clang::TypeAliasDecl *d) {
  XmlNode *node=new XmlNode("type_alias");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::TypedefDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  std::string name = d->getNameAsString();
  const clang::QualType q=d->getUnderlyingType();

  XmlNode *node=new XmlNode("typedef");
  node->setAttrib("name",name);
  node->addNode(acceptMe(q.getTypePtr()));
  return node;
}

XmlNode *XmlVisitor::visit(clang::UnresolvedUsingTypenameDecl *d) {
  XmlNode *node=new XmlNode("unresolved_using_typename");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::FieldDecl *d) {
  const clang::ASTRecordLayout &layout=
    astContext.getASTRecordLayout(d->getParent());
  std::string offset=
    std::to_string(layout.getFieldOffset(d->getFieldIndex())/8);
  clang::QualType q=d->getType();
  clang::AccessSpecifier access=d->getAccess();

  XmlNode *node=new XmlNode("field");
  node->setAttrib("name",d->getNameAsString());
  node->setAttrib("offset",offset);
  node->addNode(acceptMe(q.getTypePtr()));

  if(access==clang::AS_protected) {
    node->setAttrib("protected","true");
  } else if(access==clang::AS_private) {
    node->setAttrib("private","true");
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::CXXConstructorDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  //
  XmlNode *node=new XmlNode("constructor");

  if(d->isVariadic()) {
    node->setAttrib("variadic","true");
  }

  for(clang::FunctionDecl::param_iterator i=d->param_begin();
      i!=d->param_end();i++) {
    clang::ParmVarDecl *p = *i;
    node->addNode(acceptMe(p));
  }

  clang::AccessSpecifier access=d->getAccess();

  if(access==clang::AS_protected) {
    node->setAttrib("protected","true");
  } else if(access==clang::AS_private) {
    node->setAttrib("private","true");
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::CXXConversionDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  //
  std::string name = d->getNameAsString();
  // const clang::QualType r=d->getResultType();
  const clang::QualType r=d->getConversionType();
  clang::AccessSpecifier access=d->getAccess();

  XmlNode *node=new XmlNode("conversion");

  node->setAttrib("name",name);
  node->addNode(acceptMe(r.getTypePtr()));

  if(d->isPure()) {
    node->setAttrib("pure","true");
  }

  if(d->isConst()) {
    node->setAttrib("const","true");
  }

  if(d->isVirtual()) {
    node->setAttrib("virtual","true");
  }

  if(access==clang::AS_protected) {
    node->setAttrib("protected","true");
  } else if(access==clang::AS_private) {
    node->setAttrib("private","true");
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::CXXDestructorDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  //
  XmlNode *node=new XmlNode("destructor");

  if(d->isPure()) {
    node->setAttrib("pure","true");
  }

  if(d->isVirtual()) {
    node->setAttrib("virtual","true");
  }

  clang::AccessSpecifier access=d->getAccess();

  if(access==clang::AS_protected) {
    node->setAttrib("protected","true");
  } else if(access==clang::AS_private) {
    node->setAttrib("private","true");
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::CXXMethodDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  XmlNode *node=new XmlNode("method");

  std::string name = d->getNameAsString();
  // const clang::QualType r=d->getResultType();
  const clang::QualType r=d->getCallResultType();
  clang::AccessSpecifier access=d->getAccess();

  XmlNode *retNode=node->addNode(new XmlNode("return"));
  node->setAttrib("name",name);
  retNode->addNode(acceptMe(r.getTypePtr()));

  if(d->isVariadic()) {
    node->setAttrib("variadic","true");
  }

  if(d->isPure()) {
    node->setAttrib("pure","true");
  }

  if(d->isStatic()) {
    node->setAttrib("static","true");
  }

  if(d->isConst()) {
    node->setAttrib("const","true");
  }

  if(d->isVirtual()) {
    node->setAttrib("virtual","true");
  }

  for(clang::FunctionDecl::param_iterator i=d->param_begin();
      i!=d->param_end();i++) {
    clang::ParmVarDecl *p = *i;
    node->addNode(acceptMe(p));
  }

  if(access==clang::AS_protected) {
    node->setAttrib("protected","true");
  } else if(access==clang::AS_private) {
    node->setAttrib("private","true");
  }

  return node;
}

XmlNode *XmlVisitor::visit(clang::FunctionDecl *d) {
  if(d->getPreviousDecl()) {
    return 0;
  }

  std::string name = d->getNameAsString();
  // const clang::QualType r=d->getResultType();
  const clang::QualType r=d->getCallResultType();

  XmlNode *node=new XmlNode("function");
  XmlNode *retNode=node->addNode(new XmlNode("return"));
  node->setAttrib("name",name);
  retNode->addNode(acceptMe(r.getTypePtr()));

  if(d->isVariadic()) {
    node->setAttrib("variadic","true");
  }

  for(clang::FunctionDecl::param_iterator i=d->param_begin();
      i!=d->param_end();i++) {
    clang::ParmVarDecl *p = *i;
    node->addNode(acceptMe(p));
  }

  return node;
}

// XmlNode *XmlVisitor::visit(clang::NonTypeTemplateParmDecl *d) {
//   XmlNode *node=new XmlNode("nontype_template_parm");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(clang::ImplicitParamDecl *d) {
  XmlNode *node=new XmlNode("implicit_param");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::ParmVarDecl *d) {
  const clang::QualType q=d->getOriginalType();

  XmlNode *node=new XmlNode("param");
  node->setAttrib("name", d->getNameAsString());
  node->addNode(acceptMe(q.getTypePtr()));
  return node;
}

XmlNode *XmlVisitor::visit(clang::VarDecl *d) {
  XmlNode *node=new XmlNode("var");
  node->setAttrib("name",d->getNameAsString());
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::EnumConstantDecl *d) {
  XmlNode *node=new XmlNode("item");
  node->setAttrib("name",d->getNameAsString());
  node->setAttrib("value",d->getInitVal().toString(10));
  return node;
}

XmlNode *XmlVisitor::visit(clang::IndirectFieldDecl *d) {
  XmlNode *node=new XmlNode("indirect_field");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::UsingDirectiveDecl *d) {
  clang::NamespaceDecl *ns=d->getNominatedNamespace();
  XmlNode *node=new XmlNode("using");
  node->setAttrib("namespace",ns->getNameAsString());
  return node;
}

XmlNode *XmlVisitor::visit(clang::UsingShadowDecl *d) {
  XmlNode *node=new XmlNode("using_shadow");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(clang::StaticAssertDecl *d) {
  XmlNode *node=new XmlNode("static_assert");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::Type *t) {
  XmlNode *node=new XmlNode("todo_type");

  return node;
}

XmlNode *XmlVisitor::visit(const clang::ConstantArrayType *t) {
  //[N]
  XmlNode *node=new XmlNode("array");

  const clang::QualType q = t->getElementType();
  node->addNode(acceptMe(q.getTypePtr()));
  node->setAttrib("size",t->getSize().toString(10,false));

  return node;
}

XmlNode *XmlVisitor::visit(const clang::DependentSizedArrayType *t) {
  //[var]
  XmlNode *node=new XmlNode("array");

  const clang::QualType q = t->getElementType();
  node->addNode(acceptMe(q.getTypePtr()));

  node->addNode(new XmlNode("todo")); //size

  return node;
}

XmlNode *XmlVisitor::visit(const clang::IncompleteArrayType *t) {
  //[]
  XmlNode *node=new XmlNode("array");

  const clang::QualType q = t->getElementType();
  node->addNode(acceptMe(q.getTypePtr()));

  return node;
}

XmlNode *XmlVisitor::visit(const clang::AtomicType *t) {
  XmlNode *node=new XmlNode("atomic");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::AttributedType *t) {
  XmlNode *node=new XmlNode("attributed");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::AutoType *t) {
  XmlNode *node=new XmlNode("auto");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::BuiltinType *t) {
  XmlNode *node=new XmlNode("builtin");
  node->setAttrib("name",t->getName(printPolicy).data());
  return node;
}

XmlNode *XmlVisitor::visit(const clang::ComplexType *t) {
  XmlNode *node=new XmlNode("complex");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::DependentSizedExtVectorType *t) {
  XmlNode *node=new XmlNode("dependent_sized_ext_vector");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::FunctionNoProtoType *t) {
  // const clang::QualType r=t->getResultType();
  const clang::QualType r=t->getCallResultType(astContext);
  XmlNode *node=new XmlNode("function");
  XmlNode *retNode=node->addNode(new XmlNode("return"));
  retNode->addNode(acceptMe(r.getTypePtr()));

  return node;
}

XmlNode *XmlVisitor::visit(const clang::FunctionProtoType *t) {
  // const clang::QualType r=t->getResultType();
  const clang::QualType r=t->getCallResultType(astContext);

  XmlNode *node=new XmlNode("function");
  XmlNode *retNode=node->addNode(new XmlNode("return"));
  retNode->addNode(acceptMe(r.getTypePtr()));

  // for(auto i=t->arg_type_begin();i!=t->arg_type_end();i++) {
  for(auto i=t->param_type_begin();i!=t->param_type_end();i++) {
    const clang::QualType &p=*i;

    XmlNode *paramNode=node->addNode(new XmlNode("param"));
    paramNode->addNode(acceptMe(p.getTypePtr()));
  }

  return node;
}

XmlNode *XmlVisitor::visit(const clang::InjectedClassNameType *t) {
  XmlNode *node=new XmlNode("injected_class");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::MemberPointerType *t) {
  XmlNode *node=new XmlNode("member_pointer");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::PackExpansionType *t) {
  XmlNode *node=new XmlNode("pack_expansion");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::ParenType *t) {
  const clang::QualType q = t->getInnerType();
  return acceptMe(q.getTypePtr());
}

XmlNode *XmlVisitor::visit(const clang::PointerType *t) {
  const clang::QualType q = t->getPointeeType();

  XmlNode *node=new XmlNode("pointer");
  node->addNode(acceptMe(q.getTypePtr()));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::LValueReferenceType *t) {
  const clang::QualType q = t->getPointeeType();

  XmlNode *node=new XmlNode("reference");
  node->addNode(acceptMe(q.getTypePtr()));
  return node;
}


// XmlNode *XmlVisitor::visit(const clang::SubstTemplateTypeParmPackType *t) {
//   XmlNode *node=new XmlNode("subst_template_parm_pack");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(const clang::SubstTemplateTypeParmType *t) {
//   XmlNode *node=new XmlNode("subst_template_parm");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(const clang::EnumType *t) {
  clang::EnumDecl *d =t->getDecl();
  std::string name=d->getNameAsString();

  if(name.empty()) {
    name=std::to_string(uintptr_t(d));
  }

  XmlNode *node=new XmlNode("enum");
  node->setAttrib("name",name);
  return node;
}

XmlNode *XmlVisitor::visit(const clang::RecordType *t) {
  clang::RecordDecl *d =t->getDecl();
  std::string name=d->getNameAsString();

  if(name.empty()) {
    name=std::to_string(uintptr_t(d));
  }

  std::string type;

  if(d->isStruct()) {
    type="struct";
  } else if(d->isUnion()) {
    type="union";
  } else if(d->isClass()) {
    type="class";
  }

  XmlNode *node=new XmlNode(type);
  node->setAttrib("name",name);
  return node;
}

// XmlNode *XmlVisitor::visit(const clang::TemplateSpecializationType *t) {
//   XmlNode *node=new XmlNode("template_specialization");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

// XmlNode *XmlVisitor::visit(const clang::TemplateTypeParmType *t) {
//   XmlNode *node=new XmlNode("template_parm");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(const clang::TypedefType *t) {
  clang::TypedefNameDecl *d =t->getDecl();
  std::string name=d->getNameAsString();

  if(name.empty()) {
    name=std::to_string(uintptr_t(d));
  }

  XmlNode *node=new XmlNode("typedef");
  node->setAttrib("name",name);
  return node;
}

XmlNode *XmlVisitor::visit(const clang::DependentNameType *t) {
  XmlNode *node=new XmlNode("dependent_name");
  node->addNode(new XmlNode("todo"));
  return node;
}

// XmlNode *XmlVisitor::visit(const clang::DependentTemplateSpecializationType *t) {
//   XmlNode *node=new XmlNode("dependent_template_specialization");
//   node->addNode(new XmlNode("todo"));
//   return node;
// }

XmlNode *XmlVisitor::visit(const clang::ElaboratedType *t) {
  const clang::QualType q = t->getNamedType();
  return acceptMe(q.getTypePtr());
}

XmlNode *XmlVisitor::visit(const clang::UnaryTransformType *t) {
  XmlNode *node=new XmlNode("unary_transform");
  node->addNode(new XmlNode("todo"));
  return node;
}

XmlNode *XmlVisitor::visit(const clang::UnresolvedUsingType *t) {
  XmlNode *node=new XmlNode("unresolved_using");
  node->addNode(new XmlNode("todo"));
  return node;
}

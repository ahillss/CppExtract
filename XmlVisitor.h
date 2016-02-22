
#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclFriend.h>
#include <clang/AST/RecordLayout.h>
#include <clang/AST/Type.h>
#include <string>
#include <sstream>

#include "Visitor.h"
#include "XmlNode.h"

class XmlVisitor
  : public Visitors<
  Visitor<
    XmlNode*,clang::Decl
    ,clang::AccessSpecDecl
    ,clang::FriendDecl
    // ,clang::FriendTemplateDecl
    ,clang::LinkageSpecDecl
    ,clang::LabelDecl
    ,clang::NamespaceAliasDecl
    ,clang::NamespaceDecl
    // ,clang::ClassTemplateDecl
    // ,clang::FunctionTemplateDecl
    // ,clang::TypeAliasTemplateDecl
    // ,clang::TemplateTemplateParmDecl
    //,clang::VarTemplateDecl
    ,clang::EnumDecl
    ,clang::CXXRecordDecl
    ,clang::RecordDecl
    // ,clang::ClassTemplateSpecializationDecl
    // ,clang::ClassTemplatePartialSpecializationDecl
    // ,clang::TemplateTypeParmDecl
    ,clang::TypeAliasDecl
    ,clang::TypedefDecl
    ,clang::UnresolvedUsingTypenameDecl
    ,clang::FieldDecl
    ,clang::CXXConstructorDecl
    ,clang::CXXConversionDecl
    ,clang::CXXDestructorDecl
    ,clang::CXXMethodDecl
    ,clang::FunctionDecl
    // ,clang::NonTypeTemplateParmDecl
    ,clang::ImplicitParamDecl
    ,clang::ParmVarDecl
    // ,clang::VarTemplatePartialSpecializationDecl
    // ,clang::VarTemplateSpecializationDecl
    ,clang::VarDecl
    ,clang::EnumConstantDecl
    ,clang::IndirectFieldDecl
    ,clang::UsingDirectiveDecl
    ,clang::UsingShadowDecl
    ,clang::StaticAssertDecl
    >,
  Visitor<
    XmlNode*,const clang::Type
    ,const clang::ConstantArrayType
  ,const clang::DependentSizedArrayType
  ,const clang::IncompleteArrayType
  ,const clang::AtomicType
  ,const clang::AttributedType
  ,const clang::AutoType
  ,const clang::BuiltinType
  ,const clang::ComplexType
  ,const clang::DependentSizedExtVectorType
  ,const clang::FunctionNoProtoType
  ,const clang::FunctionProtoType
  ,const clang::InjectedClassNameType
  ,const clang::MemberPointerType
  ,const clang::PackExpansionType
  ,const clang::ParenType
  ,const clang::PointerType
  ,const clang::LValueReferenceType
  // ,const clang::SubstTemplateTypeParmPackType
  // ,const clang::SubstTemplateTypeParmType
  ,const clang::EnumType
  ,const clang::RecordType
  // ,const clang::TemplateSpecializationType
  // ,const clang::TemplateTypeParmType
  ,const clang::TypedefType
  ,const clang::DependentNameType
  // ,const clang::DependentTemplateSpecializationType
  ,const clang::ElaboratedType
  ,const clang::UnaryTransformType
  ,const clang::UnresolvedUsingType>>
  {
  private:
    clang::PrintingPolicy &printPolicy;
    clang::ASTContext &astContext;
  public:
    XmlVisitor(clang::PrintingPolicy &printPolicy,
               clang::ASTContext &astContext);

    XmlNode *visit(clang::Decl *d);
    XmlNode *visit(clang::AccessSpecDecl *d);
    XmlNode *visit(clang::FriendDecl *d);
    // XmlNode *visit(clang::FriendTemplateDecl *d);
    XmlNode *visit(clang::LinkageSpecDecl *d);
    XmlNode *visit(clang::LabelDecl *d);
    XmlNode *visit(clang::NamespaceAliasDecl *d);
    XmlNode *visit(clang::NamespaceDecl *d);
    // XmlNode *visit(clang::ClassTemplateDecl *d);
    // XmlNode *visit(clang::FunctionTemplateDecl *d);
    // XmlNode *visit(clang::TypeAliasTemplateDecl *d);
    // XmlNode *visit(clang::TemplateTemplateParmDecl *d);
    XmlNode *visit(clang::EnumDecl *d);
    XmlNode *visit(clang::CXXRecordDecl *d);
    XmlNode *visit(clang::RecordDecl *d);
    // XmlNode *visit(clang::ClassTemplateSpecializationDecl *d);
    // XmlNode *visit(clang::ClassTemplatePartialSpecializationDecl *d);
    // XmlNode *visit(clang::TemplateTypeParmDecl *d);
    XmlNode *visit(clang::TypeAliasDecl *d);
    XmlNode *visit(clang::TypedefDecl *d);
    XmlNode *visit(clang::UnresolvedUsingTypenameDecl *d);
    XmlNode *visit(clang::FieldDecl *d);
    XmlNode *visit(clang::CXXConstructorDecl *d);
    XmlNode *visit(clang::CXXConversionDecl *d);
    XmlNode *visit(clang::CXXDestructorDecl *d);
    XmlNode *visit(clang::CXXMethodDecl *d);
    XmlNode *visit(clang::FunctionDecl *d);
    // XmlNode *visit(clang::NonTypeTemplateParmDecl *d);
    XmlNode *visit(clang::ImplicitParamDecl *d);
    XmlNode *visit(clang::ParmVarDecl *d);
    XmlNode *visit(clang::VarDecl *d);
    XmlNode *visit(clang::EnumConstantDecl *d);
    XmlNode *visit(clang::IndirectFieldDecl *d);
    XmlNode *visit(clang::UsingDirectiveDecl *d);
    XmlNode *visit(clang::UsingShadowDecl *d);
    XmlNode *visit(clang::StaticAssertDecl *d);

    XmlNode *visit(const clang::Type *t);
    XmlNode *visit(const clang::ConstantArrayType *t);
    XmlNode *visit(const clang::DependentSizedArrayType *t);
    XmlNode *visit(const clang::IncompleteArrayType *t);
    XmlNode *visit(const clang::AtomicType *t);
    XmlNode *visit(const clang::AttributedType *t);
    XmlNode *visit(const clang::AutoType *t);
    XmlNode *visit(const clang::BuiltinType *t);
    XmlNode *visit(const clang::ComplexType *t);
    XmlNode *visit(const clang::DependentSizedExtVectorType *t);
    XmlNode *visit(const clang::FunctionNoProtoType *t);
    XmlNode *visit(const clang::FunctionProtoType *t);
    XmlNode *visit(const clang::InjectedClassNameType *t);
    XmlNode *visit(const clang::MemberPointerType *t);
    XmlNode *visit(const clang::PackExpansionType *t);
    XmlNode *visit(const clang::ParenType *t);
    XmlNode *visit(const clang::PointerType *t);
    XmlNode *visit(const clang::LValueReferenceType *t);
    // XmlNode *visit(const clang::SubstTemplateTypeParmPackType *t);
    // XmlNode *visit(const clang::SubstTemplateTypeParmType *t);
    XmlNode *visit(const clang::EnumType *t);
    XmlNode *visit(const clang::RecordType *t);
    // XmlNode *visit(const clang::TemplateSpecializationType *t);
    // XmlNode *visit(const clang::TemplateTypeParmType *t);
    XmlNode *visit(const clang::TypedefType *t);
    XmlNode *visit(const clang::DependentNameType *t);
    // XmlNode *visit(const clang::DependentTemplateSpecializationType *t);
    XmlNode *visit(const clang::ElaboratedType *t);
    XmlNode *visit(const clang::UnaryTransformType *t);
    XmlNode *visit(const clang::UnresolvedUsingType *t);
  };

#include "Main.h"

typedef std::pair<clang::FullSourceLoc,XmlNode*> LocXmlNodePair;

bool locXmlNodePairPredicate(LocXmlNodePair &lhs,
                         LocXmlNodePair &rhs) {
  return clang::FullSourceLoc::BeforeThanCompare()(lhs.first, rhs.first);
}

XmlNode *macroXmlNode(clang::SourceManager &srcManager,
                      clang::Preprocessor &preprocessor,
                      const clang::IdentifierInfo *id,
                      clang::MacroDirective *m) {
  if(!id->hasMacroDefinition()) {
    return 0;
  }

  clang::DefMacroDirective *md=clang::dyn_cast<clang::DefMacroDirective>(m);

  if(!md) {
    return 0;
  }

  const clang::MacroInfo* mi=md->getInfo();
  clang::FileID fid= srcManager.getFileID(mi->getDefinitionLoc());

  if(mi->isVariadic() || mi->isFunctionLike() || !mi->isObjectLike()) {
    //skip function macros
    return 0;
  }

  if(mi->isBuiltinMacro()) {
    return 0;
  }

  if(!srcManager.getFileEntryForID(fid)) {
    return 0;
  }

  //
  std::string macroName= id->getName().data();

  if(mi->getNumTokens()==1) {
    clang::Token t=*mi->tokens_begin();

    if(t.isLiteral()) {
      std::string spelling=preprocessor.getSpelling(t);

      XmlNode *node=new XmlNode("define");
      node->setAttrib("name",macroName);

      if(spelling.length() > 2 &&
         spelling[0] == '0' &&
         (spelling[1] == 'x' ||
          spelling[1] == 'X')) { //hex

        std::stringstream ss;
        ss << std::hex << spelling;

        unsigned int x;
        ss >> x;

        std::stringstream ss2;
        ss2 << x;

        node->setAttrib("type","number");
        node->setAttrib("value", ss2.str());
      } else if(spelling.length()>2 &&
                spelling[0]=='"' &&
                spelling[spelling.length()-1]=='"') { //strings
        node->setAttrib("type","string");
        node->setAttrib("value",spelling.substr(1,spelling.length()-2));
      } else if(spelling.length()>2 &&
                spelling[0]=='\'' &&
                spelling[spelling.length()-1]=='\'') { //character
        node->setAttrib("type","character");
        node->setAttrib("value",spelling.substr(1,spelling.length()-2));
      } else { //number
        node->setAttrib("type","number");
        node->setAttrib("value", spelling);
      }

      return node;
    }
  }

  return 0;
}

int main(int argc, char* argv[]) {
  llvm::cl::list<std::string>
    inputFiles(llvm::cl::Positional,llvm::cl::desc("<headers>"),llvm::cl::OneOrMore);

  llvm::cl::list<std::string>
    inclFolders("I", llvm::cl::Prefix,llvm::cl::ZeroOrMore,llvm::cl::desc("Include"));

  llvm::cl::list<std::string>
    macroDefs("D", llvm::cl::Prefix,llvm::cl::ZeroOrMore,llvm::cl::desc("Define macro"));

  llvm::cl::list<std::string>
    macroUndefs("U", llvm::cl::Prefix,llvm::cl::ZeroOrMore,llvm::cl::desc("Undefine macro"));

  llvm::cl::opt<bool> use_sxml("sxml", llvm::cl::desc("Output as SXML"));

  enum Lang {cpp_lang,c_lang};

  llvm::cl::opt<Lang>
    lang("lang", llvm::cl::desc(""),
         llvm::cl::values
         (clEnumValN(cpp_lang, "c++", "C++ language"),
          clEnumValN(c_lang,"c", "C language"),
          clEnumValEnd));

  llvm::cl::ParseCommandLineOptions(argc, argv);

  //
  clang::CompilerInstance ci;
  ci.createDiagnostics();
  ci.createFileManager();
  ci.createSourceManager(ci.getFileManager());
  ci.getPreprocessorOpts().UsePredefines = true;

  //
  std::string src="\n";

  //headers
  for(llvm::cl::list<std::string>::iterator i=inputFiles.begin();
      i!=inputFiles.end();i++) {
    src+= "#include <"+*i+">\n";
  }

  //defines
  for(llvm::cl::list<std::string>::iterator i=macroDefs.begin();
      i!=macroDefs.end();i++) {
    ci.getPreprocessorOpts().addMacroDef(*i);
  }

  //undefines
  for(llvm::cl::list<std::string>::iterator i=macroUndefs.begin();
      i!=macroUndefs.end();i++) {
    ci.getPreprocessorOpts().addMacroUndef(*i);
  }

  //
  clang::HeaderSearchOptions &headSearchOpts=ci.getHeaderSearchOpts();

  headSearchOpts.UseBuiltinIncludes=1;
  headSearchOpts.UseStandardSystemIncludes=1;
  headSearchOpts.UseStandardCXXIncludes=1;
  headSearchOpts.ResourceDir = LLVM_PREFIX "/lib/clang/" CLANG_VERSION_STRING;
  headSearchOpts.AddPath(".", clang::frontend::Angled, false, false);
  headSearchOpts.AddPath(headSearchOpts.ResourceDir+"/include",
                         clang::frontend::System, false, false);

  //includes
  for(llvm::cl::list<std::string>::iterator i=inclFolders.begin();
      i!=inclFolders.end();i++) {
    headSearchOpts.AddPath(*i,clang::frontend::Angled, false, false);
  }

  //
  clang::LangOptions &langOpts=ci.getLangOpts();

  //
  if(lang==c_lang) {
    clang::CompilerInvocation::setLangDefaults(langOpts, clang::IK_C);
  } else if(lang==cpp_lang) {
    clang::CompilerInvocation::setLangDefaults(langOpts, clang::IK_CXX);
  }

  //
  std::shared_ptr<clang::TargetOptions> targetOpts(new  clang::TargetOptions);
  // clang::TargetOptions *targetOpts=new  clang::TargetOptions;
  targetOpts->Triple = llvm::sys::getDefaultTargetTriple();
  clang::TargetInfo *targetInfo =
    clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(),targetOpts);
  ci.setTarget(targetInfo);
  ci.createPreprocessor(clang::TranslationUnitKind::TU_Complete);

  //
  // llvm::MemoryBuffer *buf=llvm::MemoryBuffer::getMemBufferCopy(src, "<my-custom>");
  clang::SourceManager &srcManager=ci.getSourceManager();
  // srcManager.createMainFileIDForMemBuffer(buf);
  clang::FileID mainFileId;//=srcManager.createFileID(buf);
  mainFileId=srcManager.createFileID(llvm::MemoryBuffer::getMemBufferCopy(src,"<my-custom>"));
  srcManager.setMainFileID(mainFileId);
  ci.createASTContext();

  //
  Consumer consumer;
  clang::PrintingPolicy printPolicy(langOpts);
  clang::Preprocessor &preprocessor=ci.getPreprocessor();
  clang::ASTContext &astContext=ci.getASTContext();

  //
  ci.getDiagnosticClient().BeginSourceFile(langOpts,&preprocessor);
  clang::ParseAST(preprocessor,&consumer,astContext,
                  false,clang::TU_Complete,0,true);
  ci.getDiagnosticClient().EndSourceFile();

  //
  if(ci.getDiagnostics().hasFatalErrorOccurred()
     || ci.getDiagnostics().hasUnrecoverableErrorOccurred()
     || ci.getDiagnostics().hasErrorOccurred()) {
    return 1;
  }

  //
  std::list<LocXmlNodePair> locsNodes;

  //
  XmlVisitor xmlVisitor(printPolicy,astContext);

  for(std::list<clang::Decl*>::iterator i=consumer.declsBegin();
      i!=consumer.declsEnd();i++) {
    clang::Decl *d=*i;

    if(XmlNode *node=xmlVisitor.acceptMe(d)) {
      clang::FullSourceLoc loc(d->getLocation(),srcManager);
      locsNodes.push_back(std::make_pair(loc,node));
    }
  }

  //
  for(clang::Preprocessor::macro_iterator i=preprocessor.macro_begin(false);
      i!=preprocessor.macro_end(false);i++) {

    if(XmlNode *node=macroXmlNode(srcManager,preprocessor,i->first,i->second)) {
      clang::FullSourceLoc loc(i->second->getLocation(),srcManager);
      locsNodes.push_back(std::make_pair(loc,node));
    }
  }

  //
  locsNodes.sort(locXmlNodePairPredicate);

  //
  XmlNode topNode("root");
  XmlNode *lastBufNode=0;

  for(std::list<LocXmlNodePair>::iterator i=locsNodes.begin();
      i!=locsNodes.end();i++) {
    std::string bufName=srcManager.getBufferName(i->first);

    if(!lastBufNode || bufName != lastBufNode->getAttrib("file")) {
      lastBufNode = topNode.addNode(new XmlNode("buffer"));
      lastBufNode->setAttrib("file", bufName);
    }

    lastBufNode->addNode(i->second);
  }

  //
  if(use_sxml) {
    std::cout << topNode.toSxml();
  } else {
    std::cout << topNode.toXml(0,true);
  }

  return 0;
}

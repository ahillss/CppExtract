#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Host.h>

#include <clang/AST/ASTContext.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/Version.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Parse/ParseAST.h>

#include <iostream>
#include <sstream>
#include <string>
#include <list>

#include "Consumer.h"
#include "XmlVisitor.h"

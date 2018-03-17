//===--- DetectCFunctionsCheck.cpp - clang-tidy----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "DetectCFunctionsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

DetectCFunctionsCheck::DetectCFunctionsCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      stdNamespaceFunctions(Options.get("stdNamespaceFunctions", "exp,floor")),
      functionsToChange(Options.get("functionsToChange", "fabs>std::abs")){}
      
void DetectCFunctionsCheck::registerMatchers(MatchFinder *Finder) {
}

void DetectCFunctionsCheck::check(const MatchFinder::MatchResult &Result) {
}

void DetectCFunctionsCheck::storeOptions(ClangTidyOptions::OptionMap &Opts)
{
  Options.store(Opts, "stdNamespaceFunctions", stdNamespaceFunctions);
  Options.store(Opts, "functionsToChange", functionsToChange);
}

void DetectCFunctionsCheck::parseStdFunctions()
{
}
  
void DetectCFunctionsCheck::parseFunctionToChange()
{
}

} // namespace misc
} // namespace tidy
} // namespace clang

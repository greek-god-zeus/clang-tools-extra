//===--- CatchByConstRefCheck.cpp - clang-tidy-----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "CatchByConstRefCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void CatchByConstRefCheck::registerMatchers(MatchFinder *Finder) {
  // This is a C++ only check thus we register the matchers only for C++
  if (!getLangOpts().CPlusPlus)
    return;

  Finder->addMatcher(varDecl(isExceptionVariable(),hasType(references(qualType(unless(isConstQualified()))))).bind("catch"), this);
}

void CatchByConstRefCheck::check(const MatchFinder::MatchResult &Result) {

  const VarDecl* varCatch = Result.Nodes.getNodeAs<VarDecl>("catch");

  const char *diagMsgCatchReference = "catch handler catches by non const reference; "
                                        "catching by const-reference may be more efficient";

  // Emit error message if the type is not const (ref)s
  diag(varCatch->getLocStart(), diagMsgCatchReference)
    << FixItHint::CreateInsertion(varCatch->getLocStart(), "const ");
}

} // namespace misc
} // namespace tidy
} // namespace clang

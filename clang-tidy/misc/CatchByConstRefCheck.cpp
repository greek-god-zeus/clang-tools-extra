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

  Finder->addMatcher(cxxCatchStmt().bind("catch"), this);
}

void CatchByConstRefCheck::check(const MatchFinder::MatchResult &Result) {

  const CXXCatchStmt* catchStmt = Result.Nodes.getNodeAs<CXXCatchStmt>("catch");
  auto& context = *Result.Context;
  
  if (!catchStmt)
    return;
  auto caughtType = catchStmt->getCaughtType();
  if (caughtType.isNull())
    return;
  auto *varDecl = catchStmt->getExceptionDecl();
  if (const auto *PT = caughtType.getCanonicalType()->getAs<PointerType>()) {
    const char *diagMsgCatchReference = "catch handler catches a pointer value; "
                                        "should throw a non-pointer value and "
                                        "catch by reference instead";
    // We forbid raising sring exceptions as well, contrary to the other check.
    diag(varDecl->getLocStart(), diagMsgCatchReference);
  } else if (!caughtType->isReferenceType()) {
    const char *diagMsgCatchReference = "catch handler catches by value; "
                                        "should catch by reference instead";
    // If it's not a pointer and not a reference then it must be caught "by
    // value".
    diag(varDecl->getLocStart(), diagMsgCatchReference);
  } else if (!caughtType.isConstant(context)) {
    const char *diagMsgCatchReference = "catch handler catches by non const reference; "
                                        "catching by const-reference may be more efficient";
    // Emit error message if the type is not const (ref)s
    diag(varDecl->getLocStart(), diagMsgCatchReference);
  }

}

} // namespace misc
} // namespace tidy
} // namespace clang

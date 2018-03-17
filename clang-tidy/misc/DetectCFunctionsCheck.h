//===--- DetectCFunctionsCheck.h - clang-tidy--------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_DETECTCFUNCTIONSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_DETECTCFUNCTIONSCHECK_H

#include "../ClangTidy.h"

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace clang {
namespace tidy {
namespace misc {

/// Checks that some functions are prefixed with 
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc-detect-c-functions.html
class DetectCFunctionsCheck : public ClangTidyCheck {
public:
  DetectCFunctionsCheck(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  void parseStdFunctions();
  void parseFunctionToChange();

  const std::string stdNamespaceFunctions;
  const std::string functionsToChange;

  std::unordered_set<std::string> stdNamespaceFunctionsSet;
  std::unordered_map<std::string, std::string> functionsToChangeMap;
};

} // namespace misc
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_DETECTCFUNCTIONSCHECK_H

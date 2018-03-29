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

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

DetectCFunctionsCheck::DetectCFunctionsCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      stdNamespaceFunctions(Options.get("stdNamespaceFunctions", "floor,exp")),
      functionsToChange(Options.get("functionsToChange", "fabs>std::abs"))
{
    parseStdFunctions();
    parseFunctionToChange();
}

void DetectCFunctionsCheck::registerMatchers(MatchFinder *Finder) {
    if (!getLangOpts().CPlusPlus)
        return;
    // Should check if there are duplicates.
    for(auto fun: stdNamespaceFunctionsSet)
    {
      Finder->addMatcher(callExpr(callee(allOf(functionDecl(hasName(fun), isExternC())))).bind(fun), this);
    }
    for(auto fun: functionsToChangeMap)
    {
      Finder->addMatcher(callExpr(callee(allOf(functionDecl(hasName(fun.first), isExternC())))).bind(fun.first), this);
    }
}

void DetectCFunctionsCheck::check(const MatchFinder::MatchResult &Result) {

    for(const auto& fun: stdNamespaceFunctionsSet)
    {
        const CallExpr* call = Result.Nodes.getNodeAs<CallExpr>(fun);
        if(call)
        {
            diag(call->getLocStart(), "this function has a corresponding std version. Consider using it (std::" + fun + ")")
                << FixItHint::CreateInsertion(call->getLocStart(), "std::");
        }
    }
    for(const auto& fun: functionsToChangeMap)
    {
        const CallExpr* call = Result.Nodes.getNodeAs<CallExpr>(fun.first);
        if(call)
        {
            auto start = call->getLocStart();
            diag(start, "this function has a better version. Consider using it (" + fun.second + ")")
                << FixItHint::CreateReplacement(SourceRange(start, start.getLocWithOffset(fun.first.size() - 1)), fun.second);
        }
    }
}

void DetectCFunctionsCheck::storeOptions(ClangTidyOptions::OptionMap &Opts)
{
  Options.store(Opts, "stdNamespaceFunctions", stdNamespaceFunctions);
  Options.store(Opts, "functionsToChange", functionsToChange);
}

void DetectCFunctionsCheck::parseStdFunctions()
{
    auto it = stdNamespaceFunctions.begin();
    while(it != stdNamespaceFunctions.end())
    {
        auto end = std::find(it, stdNamespaceFunctions.end(), ',');
        stdNamespaceFunctionsSet.insert(std::string(it, end));
        it = end;
        if(end != stdNamespaceFunctions.end())
        {
            ++it;
        }
    }
}

void DetectCFunctionsCheck::parseFunctionToChange()
{
    auto it = functionsToChange.begin();
    while(it != functionsToChange.end())
    {
        auto end = std::find(it, functionsToChange.end(), ',');
        std::string equivalent(it, end);
        it = end;
        if(end != functionsToChange.end())
        {
            ++it;
        }

        auto chevron = std::find(equivalent.begin(), equivalent.end(), '>');
        if(chevron == equivalent.end())
        {
            std::cerr << "Option functionsToChange is not formed properly, use ',' to separate functions and '>' for the function to look for and the function to be replaced with." << std::endl;
        }
        functionsToChangeMap[std::string(equivalent.begin(), chevron)] = std::string(chevron + 1, equivalent.end());
    }
}

} // namespace misc
} // namespace tidy
} // namespace clang

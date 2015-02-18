//=====-- ModularizeUtilities.h - Utilities for modularize -*- C++ -*-======//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------===//
///
/// \file
/// \brief ModularizeUtilities class definition.
///
//===--------------------------------------------------------------------===//

#ifndef MODULARIZEUTILITIES_H
#define MODULARIZEUTILITIES_H

#include "Modularize.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/HeaderSearchOptions.h"
#include "clang/Lex/ModuleMap.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringSet.h"
#include <string>
#include <vector>

namespace Modularize {

/// Modularize utilities class.
/// Support functions and data for modularize.
class ModularizeUtilities {
public:
  // Input arguments.

  /// The input file paths.
  std::vector<std::string> InputFilePaths;
  /// The header prefix.
  llvm::StringRef HeaderPrefix;

  // Output data.

  /// List of top-level header files.
  llvm::SmallVector<std::string, 32> HeaderFileNames;
  /// Map of top-level header file dependencies.
  DependencyMap Dependencies;
  /// Set of all the headers found in the module map.
  llvm::StringSet<llvm::MallocAllocator> ModuleMapHeadersSet;

  // Functions.

  /// Constructor.
  /// You can use the static createModularizeUtilities to create an instance
  /// of this object.
  /// \param InputPaths The input file paths.
  /// \param Prefix The headear path prefix.
  ModularizeUtilities(std::vector<std::string> &InputPaths,
                      llvm::StringRef Prefix);

  /// Create instance of ModularizeUtilities.
  /// \param InputPaths The input file paths.
  /// \param Prefix The headear path prefix.
  /// \returns Initialized ModularizeUtilities object.
  static ModularizeUtilities *createModularizeUtilities(
      std::vector<std::string> &InputPaths,
                  llvm::StringRef Prefix);

  /// Load header list and dependencies.
  /// \returns std::error_code.
  std::error_code loadAllHeaderListsAndDependencies();

  // Internal.

protected:

  /// Load single header list and dependencies.
  /// \param InputPath The input file path.
  /// \returns std::error_code.
  std::error_code loadSingleHeaderListsAndDependencies(
      llvm::StringRef InputPath);

  /// Load single module map and extract header file list.
  /// \param InputPath The input file path.
  /// \returns std::error_code.
  std::error_code loadModuleMap(
    llvm::StringRef InputPath);

  /// Collect module Map headers.
  /// Walks the modules and collects referenced headers into
  /// ModuleMapHeadersSet.
  /// \param ModMap A loaded module map object.
  /// \return True if no errors.
  bool collectModuleMapHeaders(clang::ModuleMap *ModMap);

  /// Collect referenced headers from one module.
  /// Collects the headers referenced in the given module into
  /// HeaderFileNames and ModuleMapHeadersSet.
  /// \param Mod The module reference.
  /// \return True if no errors.
  bool collectModuleHeaders(const clang::Module &Mod);

  /// Collect headers from an umbrella directory.
  /// \param UmbrellaDirName The umbrella directory name.
  /// \return True if no errors.
  bool collectUmbrellaHeaders(llvm::StringRef UmbrellaDirName,
    DependentsVector &Dependents);

public:

  // Utility functions.

  /// Convert header path to canonical form.
  /// The canonical form is basically just use forward slashes,
  /// and remove "./".
  /// \param FilePath The file path.
  /// \returns The file path in canonical form.
  static std::string getCanonicalPath(llvm::StringRef FilePath);

  /// Check for header file extension.
  /// If the file extension is .h, .inc, or missing, it's
  /// assumed to be a header.
  /// \param FileName The file name.  Must not be a directory.
  /// \returns true if it has a header extension or no extension.
  static bool isHeader(llvm::StringRef FileName);

  // Internal data.

  /// Options controlling the language variant.
  std::shared_ptr<clang::LangOptions> LangOpts;
  /// Diagnostic IDs.
  const llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagIDs;
  /// Options controlling the diagnostic engine.
  llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> DiagnosticOpts;
  /// Diagnostic consumer.
  clang::TextDiagnosticPrinter DC;
  /// Diagnostic engine.
  llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> Diagnostics;
  /// Options controlling the target.
  std::shared_ptr<clang::TargetOptions> TargetOpts;
  /// Target information.
  llvm::IntrusiveRefCntPtr<clang::TargetInfo> Target;
  /// Options controlling the file system manager.
  clang::FileSystemOptions FileSystemOpts;
  /// File system manager.
  llvm::IntrusiveRefCntPtr<clang::FileManager> FileMgr;
  /// Source manager.
  llvm::IntrusiveRefCntPtr<clang::SourceManager> SourceMgr;
  /// Options controlling the \#include directive.
  llvm::IntrusiveRefCntPtr<clang::HeaderSearchOptions> HeaderSearchOpts;
  /// Header search manager.
  std::unique_ptr<clang::HeaderSearch> HeaderInfo;
  // The loaded module map objects.
  std::vector<std::unique_ptr<clang::ModuleMap>> ModuleMaps;
};

} // end namespace Modularize

#endif // MODULARIZEUTILITIES_H
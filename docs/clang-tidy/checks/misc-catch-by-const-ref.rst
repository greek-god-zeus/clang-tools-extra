.. title:: clang-tidy - misc-catch-by-const-ref

misc-catch-by-const-ref
=======================

Modification of the existing throw/catch rule. Never allows catching by value, even for strings, and advises to catch by const ref (see http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Re-exception-ref).

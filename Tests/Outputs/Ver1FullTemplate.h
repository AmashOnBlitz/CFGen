// ===================================================
// CFGen Test Header
// ===================================================
//
// File Name          : FullTemplates.h
// File Stem          : FullTemplates
// File Extension     : .h
// Absolute Path      : C:\Amash\Programs\CFGen\Tests\Outputs\FullTemplates.h
// Directory          : .\Tests\Outputs
// Current Work Dir   : C:\Amash\Programs\CFGen
//
// ---- Time Metadata ----
// Date               : 2026-02-17
// Time               : 17:37:15
// Year               : 2026
// Weekday            : Tuesday
// Month              : February
// ISO Timestamp      : 2026-02-17T17:37:15
// UNIX Timestamp     : 1771330035
//
// ---- User Metadata ----
// Company (raw)      : Undefined Behavior Inc.
// Company (title)    : Undefined Behavior Inc.
// Company (slug)     : undefined-behavior-inc
//
// Author (raw)       : Amash Shafi Jami
// Author (upper)     : AMASH SHAFI JAMI
// Author (lower)     : amash shafi jami
// Author (reverse)   : imaJ ifahS hsamA
//
// Project Name (raw) : Code File Generator
// Project (snake)   : code__file__generator
// Project (camel)   : codeFileGenerator
// Project (pascal)  : CodeFileGenerator
//
// Class Name (raw)  : PressureTest
// Class (upper)     : PRESSURETEST
// Class (snake)     : pressure_test
// Class (pascal)    : Pressuretest
//
// Trim Test          : 'padded value'
// ===================================================

#ifndef PRESSURE_TEST_H
#define PRESSURE_TEST_H

namespace code__file__generator
{

class Pressuretest
{
public:
    Pressuretest();

    void setAuthor(const char* name);
    const char* getAuthor() const;

private:
    const char* m_author;
};

} // namespace code__file__generator

#endif // PRESSURE_TEST_H

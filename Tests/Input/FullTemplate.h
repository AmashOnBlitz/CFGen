// ===================================================
// CFGen Test Header
// ===================================================
//
// File Name          : ${filename}
// File Stem          : ${stem}
// File Extension     : ${ext}
// Absolute Path      : ${abspath}
// Directory          : ${dir}
// Current Work Dir   : ${cwd}
//
// ---- Time Metadata ----
// Date               : ${date}
// Time               : ${time}
// Year               : ${year}
// Weekday            : ${weekday}
// Month              : ${month}
// ISO Timestamp      : ${iso_date}
// UNIX Timestamp     : ${unix_time}
//
// ---- User Metadata ----
// Company (raw)      : ${company}
// Company (title)    : ${title:${company}}
// Company (slug)     : ${slug:${company}}
//
// Author (raw)       : ${author}
// Author (upper)     : ${upper:${author}}
// Author (lower)     : ${lower:${author}}
// Author (reverse)   : ${reverse:${author}}
//
// Project Name (raw) : ${project}
// Project (snake)   : ${snake:${project}}
// Project (camel)   : ${camel:${project}}
// Project (pascal)  : ${pascal:${project}}
//
// Class Name (raw)  : ${classname}
// Class (upper)     : ${upper:${classname}}
// Class (snake)     : ${snake:${classname}}
// Class (pascal)    : ${pascal:${classname}}
//
// Trim Test          : '${trim:${whitespace}}'
// ===================================================

#ifndef ${upper:${snake:${classname}}}_H
#define ${upper:${snake:${classname}}}_H

namespace ${snake:${project}}
{

class ${pascal:${classname}}
{
public:
    ${pascal:${classname}}();

    void setAuthor(const char* name);
    const char* getAuthor() const;

private:
    const char* m_author;
};

} // namespace ${snake:${project}}

#endif // ${upper:${snake:${classname}}}_H

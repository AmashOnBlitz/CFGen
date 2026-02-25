# CFGen — Code & File Generator

> **Stop copy-pasting boilerplate. Register a template once, generate it anywhere.**

CFGen is a lightweight CLI tool that generates files from templates you define. Write a template once, register it, then call `cfgen -gen` to stamp out a fresh copy — with macros, timestamps, filenames, and naming-convention transforms automatically filled in.

It was built out of one simple frustration: every new C++ header, Python module, or config file starts with the same block of metadata that you always forget to update, paste wrong, or forget to change from the last file. CFGen fixes that at the root.

---

## Table of Contents

- [Why This Tool Exists](#why-this-tool-exists)
- [Key Features](#key-features)
- [How It Works](#how-it-works)
- [Installation](#installation)
- [Quick Start: Real-World Tutorial](#quick-start-real-world-tutorial)
- [Command Reference](#command-reference)
- [Template System](#template-system)
- [Built-in Default Macros](#built-in-default-macros)
- [Runtime Variables](#runtime-variables)
- [Template Transforms](#template-transforms)
- [Advanced Tips](#advanced-tips)
- [Common Pitfalls & Debugging](#common-pitfalls--debugging)
- [Comparison / Alternatives](#comparison--alternatives)

---

## Why This Tool Exists

File templates already exist in IDEs, snippets managers, and shell scripts. None of them work cleanly across tools, terminals, and projects.

**IDE snippets** are locked to one editor. Switch tools, lose your templates. **Shell scripts** work, but writing a new one for each template is tedious and they don't compose well. **Cookiecutter / Yeoman** are overkill — they require Python environments, config files, prompts, and learning a framework just to stamp out a header.

CFGen is a single static binary. Register a template. Optionally define named macros. Generate. No runtime, no config, no YAML soup.

---

## Key Features

- **Template registry** — Register any file as a named template. CFGen copies and stores it internally so you can call it from anywhere.
- **Macro system** — Define named key-value pairs (`author`, `company`, etc.) that get substituted into any template on generation.
- **Runtime variables** — Declare `@register` variables inside a template and pass values with `-p<...>` at generation time. No need to update the macro registry between files.
- **Built-in default macros** — `${filename}`, `${date}`, `${year}`, `${iso_date}`, `${cwd}`, and more resolve automatically without any setup.
- **Transform functions** — Apply `upper`, `lower`, `snake`, `camel`, `pascal`, `title`, `slug`, `trim`, and `reverse` directly in template syntax.
- **Chainable / nested transforms** — `${upper:${snake:${classname}}}` works exactly as you'd expect.
- **Version checker** — `cfgen -version` shows your installed version and checks GitHub for updates.
- **Zero runtime dependencies** — The compiled binary is standalone. No interpreters, no package managers at runtime.
- **Cross-platform** — Works on Windows, Linux, and macOS.

---

## How It Works

CFGen is a two-registry tool with a template engine.

```
 [ Template Registry ]          [ Macro Registry ]
  name → script file             name → value
       |                              |
       └──────────┬───────────────────┘
                  ▼
           [ Generator ]
       reads template content
       processes @register directives
       scans for ${...} tokens
       resolves macros + runtime vars + transforms
       writes output file
```

When you run `cfgen -gen output.h MyTemplate`:

1. CFGen looks up `MyTemplate` in the template registry.
2. It reads the stored template file.
3. It strips and processes any `@register` directives, binding runtime values from `-p<...>`.
4. It scans the content for `${...}` expressions.
5. Each expression resolves — first against built-in macros, then runtime variables, then registered macros.
6. Any transform (`upper:`, `snake:`, etc.) is applied to the resolved value.
7. The processed content is written to the output file.

Templates and macros are stored in your system's standard app data directory:

| Platform | Location |
|---|---|
| Windows | `%APPDATA%\CFGen\` |
| Linux | `~/.config/CFGen/` |
| macOS | `~/Library/Application Support/CFGen/` |

---

## Installation

### Pre-built Binaries (Recommended)

Grab the latest binary from the [Releases page](https://github.com/AmashOnBlitz/cfgen/releases).

| Platform | File |
|---|---|
| Windows | `cfgen-windows` |
| Linux | `cfgen-linux` |
| macOS | `cfgen-macos` |

Download, rename to `cfgen` (or `cfgen.exe` on Windows), and put it on your `PATH`. No compiler needed.

On Linux/macOS, mark it executable first:

```bash
chmod +x cfgen
```

**macOS — Gatekeeper quarantine:** macOS flags binaries downloaded from the internet. If you see `zsh: operation not permitted: ./cfgen`, remove the quarantine flag:

```bash
xattr -d com.apple.quarantine ./cfgen
# If that doesn't work (some Apple Silicon Macs are stricter):
xattr -c ./cfgen
```

You only need to do this once.

### Building from Source

You need a C++20-capable compiler: GCC 10+ or Clang 12+. Verify with `g++ --version`.

**Windows:**
```bat
git clone https://github.com/AmashOnBlitz/cfgen.git
cd cfgen/scripts/
build.bat
```
Executable: `build\cfgen.exe`

**Linux:**
```bash
git clone https://github.com/AmashOnBlitz/cfgen.git
cd cfgen/
chmod +x scripts/build_linux.sh
./scripts/build_linux.sh
```
Executable: `build/cfgen`

> **Windows line endings issue:** If you cloned this repo on Windows and are now building on Linux, or if your editor saved the build script with Windows-style line endings (`\r\n`), you'll get an error like `env: bash\r: No such file or directory` when running the script. The `\r` is invisible but breaks the shebang line. Fix it with:
> ```bash
> sed -i 's/\r$//' scripts/build_linux.sh
> ```
> Then run `./scripts/build_linux.sh` again. You only need to do this once per clone.

### macOS
```bash
cd cfgen/
chmod +x scripts/build_mac.sh
./scripts/build_mac.sh
```
The script auto-detects GCC or Clang.  
The executable will be at `build/cfgen`.

> **Windows line endings issue:** If the build script shows `env: bash\r: No such file or directory`, the file has Windows-style line endings. Fix it with `sed -i 's/\r$//' scripts/build_linux.sh` (Linux) or `sed -i '' $'s/\r$//' scripts/build_mac.sh` (macOS). Run once per clone.

### Environment Variables Setup

To run `cfgen` from any terminal without the full path, add its directory to your `PATH`.

**Windows (permanent, PowerShell):**
```powershell
[System.Environment]::SetEnvironmentVariable(
    "PATH",
    $env:PATH + ";C:\path\to\cfgen\build",
    [System.EnvironmentVariableTarget]::User
)
```

**Linux / macOS (permanent):**
```bash
# bash
echo 'export PATH="$PATH:/path/to/cfgen/build"' >> ~/.bashrc && source ~/.bashrc
# zsh
echo 'export PATH="$PATH:/path/to/cfgen/build"' >> ~/.zshrc && source ~/.zshrc
```

---

## Quick Start: Real-World Tutorial

This walkthrough covers the two main use cases: **static macros** for things that never change (your name, your company), and **runtime variables** for things that change every file (class name, project name).

### Step 1 — Register your identity macros once

These are things you type in every file header. Register them once and never think about them again.

```bash
cfgen -reg -m author "Ada Lovelace"
cfgen -reg -m company "Babbage & Co."
cfgen -reg -m license "MIT"
cfgen -reg -m email   "ada@babbage.co"
```

`author` and `company` are perfect candidates for registered macros because they're the same across every project you ever work on.

### Step 2 — Create a template

Save this as `cpp_header.h`:

```cpp
// =======================================================
// File    : ${filename}
// Author  : ${author}
// Company : ${company}
// License : ${license}
// Created : ${date}
// =======================================================

@register ClassName
@register ProjectName

#ifndef ${upper:${snake:${ClassName}}}_H
#define ${upper:${snake:${ClassName}}}_H

namespace ${snake:${ProjectName}}
{

class ${pascal:${ClassName}}
{
public:
    ${pascal:${ClassName}}();
    ~${pascal:${ClassName}}();
};

} // namespace ${snake:${ProjectName}}

#endif // ${upper:${snake:${ClassName}}}_H
```

Notice the split:

- `${author}` and `${company}` come from your registered macros — you set them once and they appear in every generated file automatically.
- `ClassName` and `ProjectName` are **runtime variables** declared with `@register`. You pass them fresh each time you generate, right on the command line.

### Step 3 — Register the template

```bash
cfgen -reg -t CppHeader cpp_header.h
```

### Step 4 — Generate files

```bash
# Generate a file for a networking class in your "SocketLib" project
cfgen -gen src/TcpSocket.h CppHeader "-p<TcpSocket,SocketLib>"

# Generate another for a different class, no macro juggling needed
cfgen -gen src/UdpSocket.h CppHeader "-p<UdpSocket,SocketLib>"
```

The `-p<ClassName,ProjectName>` values bind to `@register` declarations in order. The output for `TcpSocket.h`:

```cpp
// =======================================================
// File    : TcpSocket.h
// Author  : Ada Lovelace
// Company : Babbage & Co.
// License : MIT
// Created : 2026-02-25
// =======================================================

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

namespace socket_lib
{

class Tcpsocket
{
public:
    Tcpsocket();
    ~Tcpsocket();
};

} // namespace socket_lib

#endif // TCP_SOCKET_H
```

`author` and `company` filled in from your stored macros. `ClassName` and `ProjectName` came from the command line — no registry updates required between files.

---

## Command Reference

All commands follow:
```
cfgen <command> [sub-command] [arguments]
```

### Help

```bash
cfgen -h
```

### Version

```bash
cfgen -version
```

Shows your installed version, fetches the latest version from GitHub, and tells you whether you're up to date, behind, or running a dev build ahead of the latest release. Requires an internet connection for the remote check.

```
CFGen - Code/File Generator
---------------------------
Installed Version : 2.0.0
Checking for updates...
Latest Version    : 2.0.0
Status            : You are using the latest version !
```

### Register a Template

```bash
cfgen -reg -t <TemplateName> <path/to/file>
```

CFGen copies the file into its own internal storage under that name. The original file is no longer referenced after registration — you can move or delete it.

```bash
cfgen -reg -t CppHeader   templates/cpp_header.h
cfgen -reg -t PythonMod   ~/boilerplate/py_module.py
cfgen -reg -t GitIgnore   .gitignore
cfgen -reg -t CMakeLists  cmake/CMakeLists.txt
```

### Register a Macro

```bash
cfgen -reg -m <name> <value>
```

Values with spaces must be quoted. Names are case-sensitive.

```bash
cfgen -reg -m author   "Ada Lovelace"
cfgen -reg -m company  "Babbage & Co."
cfgen -reg -m license  "MIT"
cfgen -reg -m email    "ada@babbage.co"
```

### Generate a File

```bash
# Standard generation
cfgen -gen <output-path> <TemplateName>

# With runtime variables
cfgen -gen <output-path> <TemplateName> "-p<val1,val2,...>"
```

```bash
# No runtime variables needed
cfgen -gen src/config.json JsonConfig

# With runtime variables
cfgen -gen src/Engine.h CppHeader "-p<Engine,GameLib>"
```

The output directory is created automatically if it doesn't exist (one level deep).

### List Templates

```bash
cfgen -show -t
```

### List Macros

```bash
cfgen -show -m
```

### Delete a Template

```bash
cfgen -del -t <TemplateName>
```

### Delete a Macro

```bash
cfgen -del -m <name>
```

### Show Storage Locations

```bash
cfgen -show -trdir   # where templates.map is stored
cfgen -show -mrdir   # where macros.map is stored
```

---

## Template System

### What a Template Is

A template is any plain text file — `.h`, `.cpp`, `.py`, `.json`, `.md`, anything. Inside it, you write `${macroname}` wherever you want a value substituted. Everything else is kept exactly as-is.

### Macro Syntax

```
${macroname}
```

### `@register` — Runtime Variable Declarations

Place `@register <varname>` directives at the top of your template to declare variables that are provided at generation time:

```
@register ClassName
@register ModuleName
```

These lines are stripped from the output entirely. The variables they declare are populated from the `-p<...>` flag in order of appearance.

### Naming Conventions

- Template names: `PascalCase` or `snake_case` — e.g., `CppHeader`, `python_module`
- Macro names: `lowercase` — e.g., `author`, `company`
- Runtime variable names: `PascalCase` recommended — e.g., `ClassName`, `ProjectName`

---

## Built-in Default Macros

These resolve automatically at generation time — no registration needed.

### File Information

| Macro | Example Value | Description |
|---|---|---|
| `${filename}` | `MyClass.h` | Full filename with extension |
| `${stem}` | `MyClass` | Filename without extension |
| `${ext}` | `.h` | Extension only |
| `${abspath}` | `/home/user/project/MyClass.h` | Absolute path of the output file |
| `${dir}` | `./src/include` | Directory of the output file |
| `${cwd}` | `/home/user/project` | Current working directory when cfgen is called |

### Date & Time

| Macro | Example Value | Description |
|---|---|---|
| `${date}` | `2026-02-17` | Today's date (YYYY-MM-DD) |
| `${time}` | `17:37:15` | Current local time (HH:MM:SS) |
| `${year}` | `2026` | Current year |
| `${month}` | `February` | Full month name |
| `${weekday}` | `Tuesday` | Full weekday name |
| `${iso_date}` | `2026-02-17T17:37:15` | ISO 8601 timestamp |
| `${unix_time}` | `1771330035` | UNIX epoch timestamp |

---

## Runtime Variables

Runtime variables let you pass per-generation values directly on the command line, without touching the macro registry. They're ideal for things that change with every file: class names, module names, feature flags, version strings.

### How They Work

**1. Declare in the template with `@register`:**

```
@register ClassName
@register ModuleName
@register FeatureFlag
```

**2. Pass values at generation time with `-p<...>`:**

```bash
cfgen -gen src/Parser.h CppHeader "-p<Parser,Lexer,true>"
```

Values bind to `@register` declarations **in order**. So `ClassName=Parser`, `ModuleName=Lexer`, `FeatureFlag=true`.

**3. Use in the template like any other macro:**

```cpp
class ${pascal:${ClassName}} : public ${ModuleName}Base
{
    bool m_enabled { ${lower:${FeatureFlag}} };
};
```

### Runtime Variables vs. Registered Macros

| | Registered Macros | Runtime Variables |
|---|---|---|
| **Set with** | `cfgen -reg -m name value` | `-p<val1,val2>` at generation time |
| **Persist** | Yes — until deleted | No — per-generation only |
| **Best for** | Author, company, license, email | Class name, project name, module name |
| **Template syntax** | `${author}` | `${ClassName}` (after `@register ClassName`) |

### Example: A Reusable C++ Class Template

Template file `cpp_class.h`:
```cpp
// Author  : ${author}          ← from macro registry, always the same
// Company : ${company}         ← from macro registry, always the same
// File    : ${filename}        ← built-in, resolved automatically
// Created : ${date}            ← built-in, resolved automatically

@register ClassName
@register BaseClass

#ifndef ${upper:${snake:${ClassName}}}_H
#define ${upper:${snake:${ClassName}}}_H

class ${pascal:${ClassName}} : public ${pascal:${BaseClass}}
{
public:
    ${pascal:${ClassName}}();
    virtual ~${pascal:${ClassName}}() = default;
};

#endif
```

Usage:
```bash
# Generate three different classes without touching the registry once
cfgen -gen src/Button.h    CppClass "-p<Button,Widget>"
cfgen -gen src/TextInput.h CppClass "-p<TextInput,Widget>"
cfgen -gen src/Checkbox.h  CppClass "-p<Checkbox,Widget>"
```

### Error Handling

If you provide **fewer** values than `@register` declarations, CFGen prints a diagnostic and aborts:

```
Runtime Variables:
------------------
ClassName   ---> Parser
ModuleName  --->  ?

[Error] Template requires 2 runtime variables, but only 1 were provided.
```

If you provide **more** values than declared, the extras are ignored with a warning.

---

## Template Transforms

### Applying a Transform

```
${transform:${macroname}}
```

```
${upper:${author}}          → ADA LOVELACE
${lower:${author}}          → ada lovelace
${snake:${project}}         → analytical_engine
${camel:${project}}         → analyticalEngine
${pascal:${project}}        → AnalyticalEngine
${title:${project}}         → Analytical Engine
${slug:${project}}          → analytical-engine
${reverse:${author}}        → ecalevoL adA
${trim:${whitespace_macro}} → trimmed value
```

### Available Transforms

| Transform | Input Example | Output |
|---|---|---|
| `upper` | `hello world` | `HELLO WORLD` |
| `lower` | `HELLO WORLD` | `hello world` |
| `title` | `hello world` | `Hello World` |
| `snake` | `HelloWorld` or `Hello World` | `hello_world` |
| `camel` | `hello_world` or `hello world` | `helloWorld` |
| `pascal` | `hello_world` or `hello world` | `HelloWorld` |
| `slug` | `Hello World!` | `hello-world` |
| `reverse` | `abc` | `cba` |
| `trim` | `  padded  ` | `padded` |

### Chaining / Nesting Transforms

Nest transforms to apply multiple operations. The inner expression resolves first:

```
${upper:${snake:${classname}}}
```

For `classname = PressureTest` → `snake` → `pressure_test` → `upper` → `PRESSURE_TEST`.

This is valid syntax:
```
${upper:${snake:${classname}}}    ✓
```

This is not:
```
${upper:snake:${classname}}       ✗  (ambiguous parse — will error)
```

### Practical Example: C++ Header Guard

```cpp
#ifndef ${upper:${snake:${ClassName}}}_H
#define ${upper:${snake:${ClassName}}}_H
// ...
#endif // ${upper:${snake:${ClassName}}}_H
```

With `ClassName = PressureTest`:
```cpp
#ifndef PRESSURE_TEST_H
#define PRESSURE_TEST_H
// ...
#endif // PRESSURE_TEST_H
```

---

## Advanced Tips

### Workflow: Setting Up a New Project

```bash
# 1. Register identity macros once (do this when you first install CFGen)
cfgen -reg -m author   "Ada Lovelace"
cfgen -reg -m company  "Babbage & Co."
cfgen -reg -m license  "MIT"

# 2. Register your templates
cfgen -reg -t CppHeader  templates/cpp_header.h
cfgen -reg -t CppClass   templates/cpp_class.cpp
cfgen -reg -t CppTest    templates/cpp_test.cpp
cfgen -reg -t PythonMod  templates/py_module.py

# 3. Generate files as you work — runtime vars for everything per-file
cfgen -gen src/Millwork.h    CppHeader "-p<Millwork,MachineLib>"
cfgen -gen src/Millwork.cpp  CppClass  "-p<Millwork,MachineLib>"
cfgen -gen tests/Millwork_test.cpp CppTest "-p<Millwork>"
```

No registry updates between files. Everything that's unique to each file goes through `-p<...>`.

### When to Use Macros vs. Runtime Variables

Use **registered macros** for values that are the same across all your work:
- Your full name, email, company name, default license

Use **runtime variables** for values that change with each file you generate:
- Class name, module name, base class, feature flag, version string, project name

If a value is the same 95% of the time but sometimes differs, register it as a macro and override it via a runtime variable in templates that need the flexibility.

### Checking What's Registered

Before generating, confirm macros are what you expect:

```bash
cfgen -show -m
cfgen -show -t
```

### Using `${cwd}` vs `${dir}`

`${cwd}` is where you're running `cfgen` from — your project root, usually. `${dir}` is the directory of the output file you specified.

Running `cfgen -gen src/net/Socket.h MyHeader` from `/home/user/project`:
- `${cwd}` → `/home/user/project`
- `${dir}` → `./src/net`
- `${filename}` → `Socket.h`
- `${stem}` → `Socket`

### Generating Into Subdirectories

If the output path's parent directory doesn't exist, CFGen creates it automatically:

```bash
cfgen -gen output/new_dir/file.h MyTemplate   # creates output/new_dir/ if needed
```

---

## Common Pitfalls & Debugging

### "Macro Not Registered" Warning

If `${something}` is in a template and `something` isn't a built-in macro, runtime variable, or registered macro, CFGen warns and substitutes an empty string:

```
[Warning] Macro Not Registered : something | Ignoring Macro
```

Fix it by registering the macro or passing the value as a runtime variable.

### Missing Runtime Parameter

If a template has more `@register` declarations than values provided with `-p<...>`:

```
[Error] Template requires 3 runtime variables, but only 1 were provided.
```

Check the count of `@register` lines in your template and match them in `-p<val1,val2,val3>`.

### Extra Runtime Parameters

If you pass more values than `@register` declarations:

```
[Error] Too many runtime parameters provided.
Some values passed with -p<...> were not used.
```

CFGen warns but still generates the file using only the values it could bind.

### Mismatched Braces

```
[Error] Macro braces are not balanced. Missing '${' or '}'.
```

Check the template for any unclosed `${`.

### Invalid Transform Syntax

The inner expression must be a full `${}` token:

```
${upper:stem}       ← wrong
${upper:${stem}}    ← correct
```

### Template Already Exists

```
ERROR : Template With This Name Already Exists!
```

Delete first or pick a different name: `cfgen -del -t Name`

### "snake" Produces Double Underscores

`snake` inserts an underscore before each uppercase letter and replaces spaces with underscores. Input `Code File Generator` → `code__file__generator` — one underscore from the capital, one from the space. Store space-separated values in all-lowercase if you plan to `snake` them.

### Checking Where Data Is Stored

```bash
cfgen -show -trdir   # template record file path
cfgen -show -mrdir   # macro record file path
```

The `.map` files are plain text with a `name.key<=@=>val.value` format — readable in any editor.

---

## Comparison / Alternatives

| Tool | Use Case | When to Pick It |
|---|---|---|
| **CFGen** | Static file generation from registered templates | You want speed, simplicity, and a binary you can drop anywhere |
| **Cookiecutter** | Full project scaffolding with prompts | You're generating an entire directory structure, not individual files |
| **IDE snippets** | In-editor template insertion | You never leave your editor and don't need cross-tool consistency |
| **Shell scripts** | Custom one-off generation | You need conditional logic, loops, or external tool integration |
| **Jinja2 / Tera** | Template rendering inside a build pipeline | You need a full template language as part of a larger system |

**CFGen is the right choice when:**
- You're generating individual files (headers, modules, configs) repeatedly across projects.
- You want templates that are editor-agnostic and terminal-native.
- You don't want to install a Python environment or configure a framework just to stamp out a header.

**CFGen may not be enough when:**
- You need conditional blocks inside templates (`if debug ... else ...`).
- You're generating entire project trees with nested structure.
- You need interactive prompts or validation during generation.

---

## Contributing

Contributions are welcome. A few expectations:

- Keep it C++20. No third-party libraries without a strong reason.
- Match the existing code style.
- If you add a transform, add a test case in `Tests/Input/` and verify output matches `Tests/Outputs/`.
- Error handling should be explicit. Don't silently swallow failures.
- If you're fixing a bug, a minimal repro case in the issue is appreciated before a PR.

To build and test locally:

```bash
./build.sh          # or build.bat on Windows
./build/cfgen -h    # sanity check
```

---

## Final Notes

CFGen was written by a student who was tired of typing the same header comment block for the fifth time this week. If it saves you that same annoyance, that's exactly what it's for.

<<<<<<< HEAD
And yes — **I took help from AI to write this README, but not with the code otherwise I could have have got someting TRASH!**
=======
And yes if you are wondering, **I took help from AI to build this Readme but not with the code** 
>>>>>>> f4507c3 (Chore : Revised Readme)

If something's broken, missing, or confusing — open an issue. Feedback from actual use is more useful than anything else.

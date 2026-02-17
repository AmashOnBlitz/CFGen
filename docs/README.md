# CFGen — Code & File Generator

> **Stop copy-pasting boilerplate. Register a template once, generate it anywhere.**

CFGen is a lightweight CLI tool that generates files from templates you define. You write the template once, register it, then call `cfgen -gen` to stamp out a fresh copy with all your macros, timestamps, filenames, and naming-convention transforms automatically filled in.

It was built out of one simple frustration: every new C++ header, Python module, or config file starts with the same block of metadata that you always forget to update, or paste wrong, or forget to change from the last file. CFGen fixes that at the root.

---

## Why This Tool Exists

File templates already exist in IDEs, snippets managers, and shell scripts. None of them work cleanly across tools, terminals, and projects.

- **IDE snippets** are locked to one editor. Switch tools, lose your templates.
- **Shell scripts** work, but writing a new one for each template is tedious and they don't compose well.
- **Cookiecutter / Yeoman** are overkill. They require Python environments, config files, prompts, and learning a framework. You just want to generate `main.cpp`.

CFGen is a single static binary. You register a template file. You optionally define named macros (like `author` or `company`). Then you generate. No runtime, no config, no YAML soup.

The template system resolves macros at generation time, applies transforms (uppercase, snake_case, slug, etc.), and handles nested transforms correctly — all without a scripting language.

---

## Key Features

- **Template registry** — Register any file as a named template. CFGen copies and stores it internally so you can call it from anywhere.
- **Macro system** — Define named key-value pairs (`author`, `company`, `project`, etc.) that get substituted into any template on generation.
- **Built-in default macros** — `${filename}`, `${date}`, `${year}`, `${iso_date}`, `${cwd}`, and more are resolved automatically without you defining anything.
- **Transform functions** — Apply `upper`, `lower`, `snake`, `camel`, `pascal`, `title`, `slug`, `trim`, and `reverse` directly inside the template syntax.
- **Chainable / nested transforms** — You can nest one transform inside another: `${upper:${snake:${classname}}}` works exactly as you'd expect.
- **Zero runtime dependencies** — The compiled binary is standalone. No interpreters, no package managers at runtime.
- **Cross-platform** — Works on Windows, Linux, and macOS. Platform-specific paths (`%APPDATA%`, `~/.config`) are handled automatically.

---

## How It Works

The mental model is simple: CFGen is a two-registry tool with a template engine.

```
 [ Template Registry ]          [ Macro Registry ]
  name → script file             name → value
       |                              |
       └──────────┬───────────────────┘
                  ▼
           [ Generator ]
       reads template content
       scans for ${...} tokens
       resolves macros + transforms
       writes output file
```

When you run `cfgen -gen output.h MyTemplate`:

1. CFGen looks up `MyTemplate` in the template registry.
2. It reads the stored template file.
3. It scans the content for `${...}` expressions.
4. Each expression is resolved — first against built-in macros, then your registered macros.
5. Any transform (`upper:`, `snake:`, etc.) is applied to the resolved value.
6. The processed content is written to `output.h`.

Templates and macros are stored in your system's standard app data directory:
- **Windows:** `%APPDATA%\CFGen\`
- **Linux:** `~/.config/CFGen/`
- **macOS:** `~/Library/Application Support/CFGen/`

You never have to touch these directories manually.

---

## Installation

### Pre-built Binaries (Recommended)

If you just want to use CFGen without building from source, grab the latest binary from the [Releases page](https://github.com/AmashOnBlitz/cfgen/releases).

| Platform | File |
|---|---|
| Windows | `cfgen-windows` |
| Linux | `cfgen-linux` |
| macOS | `cfgen-macos` |

Download the binary for your platform, rename it to `cfgen` (or `cfgen.exe` on Windows), and put it somewhere on your `PATH`. That's it — no compiler needed.

On Linux/macOS you'll need to mark it executable first:

```bash
chmod +x cfgen
```

**macOS only — Gatekeeper quarantine:** macOS flags binaries downloaded from the internet and blocks them from running, even after `chmod +x`. You'll know you've hit this if you see:

```
zsh: operation not permitted: ./cfgen
```

Remove the quarantine flag with:

```bash
xattr -d com.apple.quarantine ./cfgen
```

If that doesn't fully work (some Apple Silicon Macs are stricter), clear all extended attributes:

```bash
xattr -c ./cfgen
```

After either command, `./cfgen` will run normally. You only need to do this once — the flag doesn't come back.

> **Note:** `cfgen` (without `./`) will still say `command not found` until you add the binary's directory to your `PATH`. That's a separate step — see Environment Variables below.

---

### Building from Source

If you'd rather build it yourself, you need a C++20-capable compiler:
- **Windows:** GCC via [MSYS2](https://www.msys2.org/) or MinGW-w64 (recommended)
- **Linux:** GCC (`g++`) — usually pre-installed or available via your package manager
- **macOS:** GCC via Homebrew (`brew install gcc`) or Apple Clang (both work)

Verify with:
```bash
g++ --version   # should say GCC 10+ or Clang 12+
```

### Windows

```bat
git clone https://github.com/AmashOnBlitz/cfgen.git
cd cfgen/scripts/
build.bat
```

The executable will be at `build\cfgen.exe`.

To use `cfgen` from anywhere, add the `build\` directory to your `PATH` (see Environment Variables section below).

### Linux

```bash
git clone https://github.com/AmashOnBlitz/cfgen.git
cd cfgen/scripts/
chmod +x build.sh
./build.sh
```

The executable will be at `build/cfgen`.

> **Windows line endings issue:** If you cloned this repo on Windows and are now building on Linux, or if your editor saved the build script with Windows-style line endings (`\r\n`), you'll get an error like `env: bash\r: No such file or directory` when running the script. The `\r` is invisible but breaks the shebang line. Fix it with:
> ```bash
> sed -i 's/\r$//' build.sh
> ```
> Then run `./build.sh` again. You only need to do this once per clone.

### macOS

```bash
cd cfgen/
chmod +x scripts/build_mac.sh
./scripts/build_mac.sh
```

The build script auto-detects whether you have GCC or Clang and adjusts accordingly. If you only have Apple's Clang (no Homebrew GCC), it falls back to `clang++` automatically. C++20 support on Apple Clang requires Xcode 13+ / macOS 12+.

> **Windows line endings issue:** Same situation as Linux — if the script was ever touched on Windows, you may hit `env: bash\r: No such file or directory`. Fix it with:
> ```bash
> sed -i '' $'s/\r$//' scripts/build_mac.sh
> ```
> The `''` after `-i` is required on macOS (BSD sed syntax). Linux uses `sed -i` without it.

### Build Script Features

Both build scripts share the same behavior:
- Compile source files **in parallel** for speed.
- Use **incremental builds** — only recompile files that changed (based on timestamps).
- Output colored status (`[BUILD]` / `[CACHE]`) per file.
- Fail fast and show the compiler error if any file fails.

---

## Environment Variables Setup

To run `cfgen` from any terminal without specifying the full path, add its directory to your system `PATH`.

**Why this matters:** Without this, you'd have to type `C:\path\to\cfgen\build\cfgen.exe` every time. With it, you just type `cfgen`.

### Windows — PowerShell (temporary, current session only)

```powershell
$env:PATH += ";C:\path\to\cfgen\build"
```

### Windows — Permanent via GUI

1. Open **Start**, search for **"Edit the system environment variables"**.
2. Click **Environment Variables**.
3. Under **User variables**, find `Path` and click **Edit**.
4. Click **New** and paste the full path to `cfgen\build\`.
5. Click OK on all dialogs.
6. Open a **new** terminal — the change takes effect immediately.

### Windows — Permanent via PowerShell

```powershell
[System.Environment]::SetEnvironmentVariable(
    "PATH",
    $env:PATH + ";C:\path\to\cfgen\build",
    [System.EnvironmentVariableTarget]::User
)
```

### Linux / macOS — Temporary

```bash
export PATH="$PATH:/path/to/cfgen/build"
```

### Linux / macOS — Permanent

Add the export line to your shell's config file:

```bash
# For bash:
echo 'export PATH="$PATH:/path/to/cfgen/build"' >> ~/.bashrc
source ~/.bashrc

# For zsh:
echo 'export PATH="$PATH:/path/to/cfgen/build"' >> ~/.zshrc
source ~/.zshrc
```

---

## Basic Usage

All commands follow this structure:

```
cfgen <command> [sub-command] [arguments]
```

### Help

```bash
cfgen -h
```

### Register a Template

```bash
cfgen -reg -t MyHeader header_template.h
```

The `-reg -t` command takes two things: a **name** you're assigning to the template, and a **path to the file** that will serve as the template. The name is what you'll use to refer to this template later — in `-gen`, `-del`, or `-show`. Pick something short and descriptive.

What happens internally: CFGen copies your file into its own storage directory under that name. After registration, the original file is no longer referenced — CFGen has its own copy. You can safely move or delete the source file and the template will still work.

```
cfgen -reg -t CppHeader  templates/cpp_header.h   ← name, then file path
cfgen -reg -t PythonMod  ~/boilerplate/py_mod.py
cfgen -reg -t GitIgnore  .gitignore
```

Any plain text file can be a template. The file extension doesn't matter to CFGen — it copies the content verbatim and substitutes macros when you generate. A `.gitignore`, a `CMakeLists.txt`, a Markdown file — all valid.

### Register a Macro

```bash
cfgen -reg -m author "Ada Lovelace"
cfgen -reg -m company "Babbage & Co."
cfgen -reg -m project "AnalyticalEngine"
```

The `-reg -m` command takes a **name** and a **value**. The name is what you write inside `${...}` in your templates. The value is what gets substituted in its place at generation time.

Macros are persistent — you register them once and they're available to every template, every time. Think of them as your personal variables: `author`, `company`, `license`, `email`, whatever you put in every file header. Register them once when you set up CFGen and forget about them.

A few things worth knowing:
- **Values with spaces must be quoted** on the command line, otherwise the shell splits them as separate arguments.
- **Names are case-sensitive.** `Author` and `author` are two different macros.
- **There's no concept of scope.** All macros are global. If you need a different value for a specific generation (like `classname`), you update the macro before generating — either by deleting and re-registering, or by simply using a different name per use case.

```bash
# Project-level macros — register once
cfgen -reg -m author   "Ada Lovelace"
cfgen -reg -m company  "Babbage & Co."
cfgen -reg -m license  "MIT"

# Per-file macros — update as you work
cfgen -reg -m classname "Socket"
cfgen -gen src/Socket.h CppHeader

cfgen -del -m classname
cfgen -reg -m classname "EventLoop"
cfgen -gen src/EventLoop.h CppHeader
```

### Generate a File

```bash
cfgen -gen output/MyClass.h MyHeader
```

This reads the `MyHeader` template, resolves all macros and transforms, and writes the result to `output/MyClass.h`. The directory will be created if it doesn't exist.

### List Templates

```bash
cfgen -show -t
```

Output:
```
Templates:
----------------------------------------------------
| #  | Template Name         | File Path           |
----------------------------------------------------
| 1  | MyHeader              | header_template.h   |
| 2  | CppClass              | class_template.cpp  |
----------------------------------------------------
```

### List Macros

```bash
cfgen -show -m
```

### Delete a Template

```bash
cfgen -del -t MyHeader
```

### Delete a Macro

```bash
cfgen -del -m author
```

### Show Storage Locations

```bash
cfgen -show -trdir   # Where templates.map is stored
cfgen -show -mrdir   # Where macros.map is stored
```

---

## Template System

### What a Template Is

A template is any plain text file — a `.h`, `.cpp`, `.py`, `.json`, `.md`, whatever. Inside it, you write `${macroname}` wherever you want a value substituted. Everything else in the file is kept exactly as-is.

### Where Templates Are Stored

When you register a template with `-reg -t`, CFGen does two things: it copies your file into its own internal directory, and it adds an entry to its record file so it knows the name-to-file mapping.

The internal storage location is:

```
Windows:  %APPDATA%\CFGen\<TemplateName>\<original_filename>
Linux:    ~/.config/CFGen/<TemplateName>/<original_filename>
macOS:    ~/Library/Application Support/CFGen/<TemplateName>/<original_filename>
```

Each template gets its own subdirectory named after the template. So if you register `MyHeader` from `header_template.h`, the stored copy lives at something like `%APPDATA%\CFGen\MyHeader\header_template.h`. The original filename is preserved inside that folder.

The full registry of template names is kept in `templates.map` in the same base directory. It's a plain text file — you can open it and read it, though you shouldn't need to edit it manually. Use `-show -trdir` to find out exactly where it is on your machine.

When you delete a template with `-del -t`, CFGen removes both the entry from the record file and the stored script file. It cleans up after itself.

### Macro Syntax

Macros use `${...}` syntax:

```
${macroname}
```

Example template:
```cpp
// File    : ${filename}
// Author  : ${author}
// Created : ${date}

#ifndef ${upper:${stem}}_H
#define ${upper:${stem}}_H

class ${pascal:${classname}}
{
public:
    ${pascal:${classname}}();
    ~${pascal:${classname}}();
};

#endif // ${upper:${stem}}_H
```

When you run `cfgen -gen MyClass.h MyHeader`, and you have `classname` registered as `my_class`, this generates:

```cpp
// File    : MyClass.h
// Author  : Ada Lovelace
// Created : 2026-02-17

#ifndef MYCLASS_H
#define MYCLASS_H

class MyClass
{
public:
    MyClass();
    ~MyClass();
};

#endif // MYCLASS_H
```

### Naming Conventions

Template names and macro names are case-sensitive strings. Recommended conventions:
- Template names: `PascalCase` or `snake_case` (e.g., `CppHeader`, `python_module`)
- Macro names: `lowercase` (e.g., `author`, `company`, `classname`)

---

## Built-in Default Macros

These are resolved automatically — no registration needed. They reflect the current state at the moment of generation.

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

## Template Transforms & Logic

### Applying a Transform

Transforms follow this syntax:

```
${transform:${macroname}}
```

The transform name comes before the colon. The macro to transform comes after, wrapped in `${}`.

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

**`snake` on space-separated input:** Spaces become underscores, uppercase letters get an underscore prepended. `Code File Generator` → `code__file__generator`. If you want clean snake_case, use a value without capital letters or spaces.

**`slug`:** Strips everything except alphanumerics, spaces, underscores, and dashes. Replaces them all with hyphens. Good for URLs or file slugs.

### Chaining / Nesting Transforms

You can nest transforms to apply multiple operations:

```
${upper:${snake:${classname}}}
```

This first resolves `${classname}`, applies `snake` to it, then applies `upper` to that result. Nesting is supported up to 32 levels deep (you'll never realistically hit this).

**The syntax rule:** The inner expression must always be a full `${...}` expression, not a raw string. This is valid:

```
${upper:${snake:${classname}}}    ✓
```

This is not:

```
${upper:snake:${classname}}       ✗  (invalid — ambiguous parse)
```

### Transforms Applied to Built-in Macros

Built-in macros can also be transformed:

```
${upper:${stem}}        → MYCLASS   (from output filename)
${slug:${month}}        → february
${lower:${weekday}}     → tuesday
```

### Practical Example: Header Guard

A common pattern for C++ include guards:

```cpp
#ifndef ${upper:${snake:${classname}}}_H
#define ${upper:${snake:${classname}}}_H
// ...
#endif // ${upper:${snake:${classname}}}_H
```

If `classname` is `PressureTest`, this produces:

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
# Register your project-level macros once
cfgen -reg -m author "Ada Lovelace"
cfgen -reg -m company "Babbage & Co."
cfgen -reg -m project "AnalyticalEngine"
cfgen -reg -m license "MIT"

# Register templates
cfgen -reg -t CppHeader templates/cpp_header.h
cfgen -reg -t CppClass  templates/cpp_class.cpp
cfgen -reg -t CppTest   templates/cpp_test.cpp

# Generate files as you work
cfgen -reg -m classname "Millwork"
cfgen -gen src/Millwork.h  CppHeader
cfgen -gen src/Millwork.cpp CppClass
cfgen -gen tests/Millwork_test.cpp CppTest
```

### Swapping Macros for Context

Macros are global, so you update them before generating a context-specific file:

```bash
cfgen -del -m classname
cfgen -reg -m classname "Gearbox"
cfgen -gen src/Gearbox.h CppHeader
```

This is intentional. CFGen doesn't have per-file variable scoping — it's a simple tool. For complex templating with prompts and branching, you'd reach for something heavier.

### Checking What's Registered

Before generating, confirm your macros are what you expect:

```bash
cfgen -show -m
cfgen -show -t
```

### Using `${cwd}` vs `${dir}`

- `${cwd}` is where you're running `cfgen` from — your project root, usually.
- `${dir}` is the directory of the output file you specified in `-gen`.

If you run `cfgen -gen src/net/Socket.h MyHeader` from `/home/user/project`:
- `${cwd}` → `/home/user/project`
- `${dir}` → `./src/net`
- `${filename}` → `Socket.h`
- `${stem}` → `Socket`

### Generating Into Subdirectories

If the output path's parent directory doesn't exist, CFGen creates one level automatically:

```bash
cfgen -gen output/new_dir/file.h MyTemplate   # creates output/new_dir/ if needed
```

---

## Common Pitfalls & Debugging

### "Macro Not Registered" Warning

If you use `${something}` in a template and `something` isn't a built-in macro and isn't registered, CFGen prints a warning and substitutes an empty string:

```
[Warning] Macro Not Registered : something | Ignoring Macro
```

The file still gets generated — the field just ends up blank. Fix it by registering the macro:

```bash
cfgen -reg -m something "my value"
```

### Mismatched Braces

If you write `${upper:${stem}` (missing closing brace), CFGen will detect it:

```
[Error] Macro braces are not balanced. Missing '${' or '}'.
```

Check the template file for unclosed `${`.

### Invalid Transform Syntax

The transform syntax requires the inner expression to be a full `${}` token. This will error:

```
${upper:stem}     ← wrong, 'stem' is not wrapped in ${}
```

Correct form:

```
${upper:${stem}}
```

Error message: `[Error] Invalid transform format. Use: ${transform:${macro}}`

### Template Already Exists

```
ERROR : Template With This Name Already Exists!
```

You already registered a template with that name. Either delete it first (`cfgen -del -t Name`) or pick a different name.

### Template File Not Found During Generation

```
ERROR : Cannot Read The Template Requested!
```

The template is registered in the record but its stored script file is missing. This can happen if you manually deleted files from the CFGen data directory. Delete the broken registration and re-register:

```bash
cfgen -del -t BrokenTemplate
cfgen -reg -t BrokenTemplate path/to/template/file.h
```

### "snake" Produces Double Underscores

`snake` inserts an underscore before each uppercase letter and replaces spaces with underscores. If your input is `Code File Generator` (space-separated words starting with capitals), you'll get `code__file__generator` — one underscore from the capital, one from the space.

To avoid this, store your macro value in all-lowercase if you plan to snake_case it, or just be aware of the output format.

### Build Script Fails with `bash\r: No such file or directory`

This is a Windows line endings problem. It happens when the build script (`build.sh` or `build_mac.sh`) was saved or edited on Windows, which writes `\r\n` line endings instead of the Unix `\n`. The `\r` character attaches invisibly to the shebang line (`#!/usr/bin/env bash\r`), and the OS tries to find an interpreter literally named `bash\r` — which doesn't exist.

The error looks like:

```
env: bash\r: No such file or directory
```

or sometimes just:

```
./build.sh: line 1: $'\r': command not found
```

Fix on **Linux:**
```bash
sed -i 's/\r$//' build.sh
```

Fix on **macOS** (BSD sed requires the `''` argument and a slightly different syntax):
```bash
sed -i '' $'s/\r$//' scripts/build_mac.sh
```

Run the script again after fixing — you only need to do this once per clone. If you want to prevent it from happening again, configure Git to not convert line endings on checkout:

```bash
git config core.autocrlf false
```

Or add a `.gitattributes` file to the repo root:
```
*.sh text eol=lf
```

### Checking Where Data Is Stored

```bash
cfgen -show -trdir   # template record file path
cfgen -show -mrdir   # macro record file path
```

You can open these `.map` files in any text editor — they're plain text with a simple `name.key<=@=>val.value` format.

---

## Comparison / Alternatives

| Tool | Use Case | When to Pick It |
|---|---|---|
| **CFGen** | Static file generation from registered templates | You want speed, simplicity, and a binary you can drop anywhere |
| **Cookiecutter** | Full project scaffolding with prompts | You're generating an entire directory structure, not individual files |
| **IDE snippets** | In-editor template insertion | You never leave your editor and don't need cross-tool consistency |
| **Shell scripts** | Custom one-off generation | You need conditional logic, loops, or external tool integration |
| **Jinja2 / Tera** | Template rendering as part of a build pipeline | You need full template language features inside a larger system |

**CFGen is the right choice when:**
- You're generating individual files (headers, modules, configs) repeatedly across projects.
- You want templates that are editor-agnostic and terminal-native.
- You don't want to install a Python environment or configure a framework just to stamp out a header.

**CFGen may not be enough when:**
- You need conditional blocks inside templates (`if debug ... else ...`).
- You're generating entire project trees with nested structure.
- You need interactive prompts or validation during generation.

---

## Project Philosophy

CFGen is built on a few convictions:

**Correctness over convenience.** The macro resolver validates brace matching, checks transform syntax, enforces nesting limits, and reports errors with line numbers. It won't silently produce garbage output.

**Transparency.** Templates are plain files you write. Macros are key-value pairs you define. The record files are human-readable text. Nothing is hidden in opaque binary formats or databases.

**Minimal footprint.** CFGen doesn't install services, register itself with the OS, or require a runtime. It's a binary that reads a few text files and writes output. When it's done, it's done.

**One job, done well.** This isn't a build system, a scaffolding tool, or a code generator framework. It generates files from templates with macro substitution. That's the whole thing.

---

## Contributing

If you find a bug, have a feature idea, or want to add a transform — contributions are welcome.

A few expectations:
- Keep it C++20. No third-party libraries unless there's a really good reason.
- Match the existing code style. It's consistent — keep it that way.
- If you add a transform, add a test case in `Tests/Input/` and verify the output matches `Tests/Outputs/`.
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
And yes if you are wondering, **I took help from AI to build this Readme but not with the code** 
If something's broken, missing, or confusing — open an issue. Feedback from actual use is more useful than anything else.

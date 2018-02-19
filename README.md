# `pea`

(I didn't have any good name idea so "`pea`" may not be final)

As of now, the main thing `pea` can do is mimic the built-in EA preprocessor (to an extent, see the "EA Incompatibilities" section).

The main *goal* of `pea` *right now* is to be able to *geneate Makefile dependencies* from events.

If you feel like it, you can also use `pea` in a non-EA context. Since `pea` outputs to stdout by default you can even use it in a pipeline with something else.

## Program options & parameters

```
Usage:
  pea <input> [options]...

Options:
  -o <file>      - sets output (defaults to stdout)
  -E             - disables output
  -D <name>      - adds preprocessor definition
  -I <directory> - adds extra directory to search included files in
  -T <directory> - adds extra directory to seatch tools in
  -M             - enables generation of dependency Makefile (implies -E)
  -MM            - same as -M; doesn't add included files from extra directories (see -I)
  -MD            - enables generation of dependency Makefile as side-effect of regular preprocessing (doesn't imply -E)
  -MMD           - same as -MD; doesn't add included files from extra directories (see -I)
  -MG            - when generating dependency Makefile, adds missing included files to dependency
  -MP            - when generating dependency Makefile, adds phony targets for each file listed (except the main input)
  -MF <file>     - when generating dependency Makefile, sets output (defaults to stdout)
  -MT <name>     - when generating dependency Makefile, sets rule target

In addition to using the above options, you can also directly modify the underlying configuration using the following argument format:
  -.<parameter><operation><value>

Where <parameter> is the name of the parameter (see below for a complete list), <operation> either '=' or '+=', and <value> the value you wish to set/add

Examples:
  -.pp.tool.enableSystemTools=true
  -.pp.include.extraIncludeDirectories+=/usr/include

Default parameter values:
  boolean:     pp.condition.enable                = true
  string-list: pp.eaCompat.binaryToolArguments    = ["%%", "-byte", "--to-stdout"]
  string:      pp.eaCompat.binaryToolName         = "bin2ea"
  boolean:     pp.eaCompat.enableBinary           = true
  boolean:     pp.eaCompat.enableDirectiveAliases = true
  boolean:     pp.eaCompat.force                  = false
  boolean:     pp.emulate                         = false
  boolean:     pp.include.enable                  = true
  boolean:     pp.include.enableRootRelative      = false
  path-list:   pp.include.extraIncludeDirectories = []
  boolean:     pp.include.ignoreMissingFiles      = false
  integer:     pp.include.maxDepth                = 100
  path:        pp.input                           = <no path>
  boolean:     pp.macro.enable                    = true
  string-list: pp.macro.extraDefines              = []
  boolean:     pp.macro.strictMacroFormat         = false
  boolean:     pp.makedep.addPhonyTargetFiles     = false
  boolean:     pp.makedep.enable                  = false
  boolean:     pp.makedep.noExtraIncludes         = true
  boolean:     pp.makedep.noMissingFiles          = true
  boolean:     pp.makedep.noToolArguments         = true
  path:        pp.makedep.output                  = <no path>
  boolean:     pp.makedep.outputIsTarget          = true
  path:        pp.makedep.target                  = <no path>
  path:        pp.output                          = <no path>
  boolean:     pp.tool.addToStdout                = true
  boolean:     pp.tool.enable                     = true
  boolean:     pp.tool.enableLocalTools           = false
  boolean:     pp.tool.enableSystemTools          = false
  path-list:   pp.tool.extraToolDirectories       = []
  boolean:     pp.tool.ignoreMissingTools         = false
```

(You can also get this same help from invoking `pea` without arguments, or with `-h`/`--help`).

## Makefile dependency generation

The options for this feature mostly follow the same names as [the ones from `gcc`](https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html): `-M`, `-MD`, `-MM`, `-MT`, `-MF`, etc (see the option & parameter list from the help).

Here's an example output (`pea -MM -MG -MT Out.gba -MP <other stuff>`):

```makefile
Out.gba: Main.event _FE8EssentialFixes.event _FE8ProcNameFix.event Events/Chapter.event Events/_CommonEvents.event IDoNotExist.bin

_FE8EssentialFixes.event:
_FE8ProcNameFix.event:
Events/Chapter.event:
Events/_CommonEvents.event:
IDoNotExist.bin:
```

(`-MG` here allowed `IDoNotExist.bin` to be included in the list, even if it doesn't exist)

It works for `#include`, and also for `#incbin` and *sometimes* for the tool include directive family (if any argument correspond to an existing file relative to the current directory, it will be added to the list).

## EA Incompatibilities

- `#easteregg` isn't handled by `pea`, sadly.
- `#runext` isn't handled by `pea` (nobody seems to be using it anyway)
- `#inctext` and `#inctevent` are aliases for `#include_tool`, and will be disabled if the `pp.eaCompat.enableDirectiveAliases` parameter is cleared.
- `#incbin` and `#incext` **require** having [`bin2ea`](https://github.com/StanHash/bin2ea/releases) in one of the tool directories (or another similar tool, the `pp.eaCompat.binaryTool` & `pp.eaCompat.binaryToolArguments` parameters allow you to customize the tool call)
- No support for EA built-in macros (`Switch`, `IsDefined`, ...). `#define`, `#undef`, `#if[n]def` and macro expansions are still a thing don't worry.

## Extra features over EA

(None of which really matters if you just use EA after this anyway, but if you're preprocessing and then feeding the output into EA, it might be useful stuff)

- With "`#define Macro(a, b) abracadabra`"; "`Macro(0, 1)`" will expand to `01r0c0d01r0` in regular EA. `pea` allows for stricter Macro argument expansion through setting the `pp.macro.strictMacroFormat` option (macro parameters will only match the format if it isn't surrounded by any letter or digit).
- EA tools usually are all packed into a specific "tool" folder. `pea` allows you to not only specify more of those folders (using the `-T <folder>` option or the `pp.tool.extraToolDirectories` parameter), but also:
  - call tools from the current directory (aka the directory the current file is in) (requires `pp.tool.enableLocalTools` to be set)
  - call tools from the system (reads the `PATH` env variable) (requires `pp.tool.enableSystemTools` to be set)
- EA also *forces* the `--to-stdout` option for tool calls. `pea` does it too by default, but it can be disabled through clearing the `pp.tool.addToStdout` parameter.

## Building

`pea` uses [boost](http://www.boost.org/) (more specifically `Boost.Filesystem` and `Boost.Process`)... so good luck having *that* set up. I'll be working on porting to [C++17 filesystem](http://en.cppreference.com/w/cpp/filesystem) (for replacing `Boost.Filesystem`) and [some smaller process library](https://github.com/eidheim/tiny-process-library) (for replacing `Boost.Process`), because working with boost was kind of a painful experience.

Otherwise just run *CMake* as for any other *CMake* project:

```cmd
mkdir build
cd build
cmake ..
make
```

# Environment and compatibility

> [English](environment.md) | [Español](environment-es.md)

This is the single place for the lab's environment requirements. The legacy archive has its own
platform history; that is a different topic and is covered by `archaeology-16`.

## Minimum environment (Starter route)

If you are following the [Starter route](starter-route.md), you only need:

- Python 3.10 or newer.
- Bash 4 or newer.
- Standard POSIX tools: `grep`, `diff`, `cmp`, `sha256sum`.

The C++20 compiler is required only when you enter Track A or Track B.

## Primary environment

- Linux or macOS with a POSIX shell.
- Bash 4 or newer.
- A C++20 compiler: `g++` or `clang++`.
- Python 3.10 or newer.
- CMake optional; `run.sh` falls back to a direct compiler invocation.
- Standard POSIX tools: `find`, `grep`, `sed`, `awk`, `cmp`, `mktemp`, `timeout`, `sha256sum`.

## What is portable

- The C++20 code in `construction/`, `reference/` and `archaeology/` uses the standard library.
- The Python code uses only the standard library.
- The game logic, the LCG and the transcript contract are language- and OS-independent.
- `construction-21` uses `std::thread` and `std::mutex`; CMake links `Threads::Threads`.

## What is POSIX-bound

The automation is Bash-based:

- `run.sh`, `build_probe.sh`, `capture_trace.sh`, `measure.sh` and the failure-test runner.
- They use `mktemp`, `timeout`, `sed -i`, `cmp`, `sha256sum`, `/dev/null`, `xargs -0` and process
  substitution.
- `construction-19/code/benchmark.cpp` hardcodes `/dev/null`.
- CMake files pass `-Wall -Wextra -Werror`, which are GCC/Clang flags.

## Why Linux for software engineers

Linux is the lab's primary environment because it gives direct access to the tools engineers use:

- **POSIX shell and pipelines.** Automation is composable: `grep`, `sed`, `awk`, `find`, `xargs`
  and redirection work the same way in scripts, CI and remote sessions.
- **Native toolchains.** Compilers, debuggers, profilers, linkers and package managers are available
  without a compatibility layer.
- **Production parity.** Most servers and CI runners are Linux, so local behavior is closer to
  deployment behavior.
- **Containers and isolation.** Docker, Podman and similar tools run natively; the same image works
  locally and in CI.
- **Transparent system model.** Files, permissions, processes, signals, sockets and environment
  variables are visible and scriptable.
- **Remote work.** SSH, tmux and terminal-first tools make remote machines and servers first-class.
- **Open ecosystem.** Documentation, source code and package repositories are directly available.

WSL2 brings this environment to Windows without dual boot. It is the only supported Windows route
because it preserves the POSIX shell, toolchain and path semantics the repository depends on.

## Reading the Markdown files

The lessons are Markdown. A plain text editor shows the source; a Markdown viewer renders headings,
tables, links and code blocks.

### Linux

Okular is the recommended desktop viewer:

```bash
okular README.md
```

Install it with your package manager, for example `sudo apt install okular` on Debian/Ubuntu,
`sudo dnf install okular` on Fedora, or `sudo pacman -S okular` on Arch.

Terminal alternatives: `glow README.md` or `mdcat README.md`. For a browser view:
`pandoc README.md -o README.html`.

### Other environments

- **Windows:** Visual Studio Code with Markdown Preview, Obsidian or Typora. The lab itself still
  runs only inside WSL2; these tools are for reading the documentation.
- **macOS:** Visual Studio Code, Obsidian, MacDown or Typora.
- **Any environment:** GitHub renders the files in the repository; `pandoc` converts Markdown to
  HTML or PDF.

A rendered view is not required to run the code, but it makes tables, links and code blocks much
easier to read.

## Windows

WSL2 is the only supported Windows option. The repository runs as written inside WSL2. Native
Windows shells and toolchains are not supported workflows.

## Known platform exceptions

- `construction-19`: the benchmark redirects to `/dev/null`; it is a POSIX-only bench.
- `construction-16`: the adapter uses ANSI escape sequences; the terminal must support them.
- `construction-21`: threads are portable through `Threads::Threads`, but the manual
  `-pthread` command is POSIX-specific.
- `archaeology-01` and `archaeology-03`: the legacy build probes use `g++`,
  `-fpermissive`, `timeout` and `/dev/null`.

## Legacy archive

The original source targeted a 2007-era Windows compiler and console. The lab does not build it
natively; the compatibility probes compile a scratch copy with a modern compiler and a
small shim.
The archive itself remains the evidence.

## Containers and CI

No Dockerfile, devcontainer or CI workflow is included. WSL2 is the practical Windows route; a
container would be an additional artifact, not part of the current repository.

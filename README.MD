# C-shell

A shell written in C. For educational purposes. [View Source.](https://github.com/kitswas/C-shell)

> [!CAUTION]
> Warranty NOT included. Use at your own risk.

## How it works

Here's what happens when you write something and press enter after the prompt:

```bash
sleep 5 &; echo "Hello, World!" | wc -c
```

1. The above input will be split up at `;` into two jobs (also known as pipelines or process groups).
2. Each job will be executed sequentially in a separate process. An `&` at the end of a job will run the pipeline in the background.
    - *e.g.* The first job will run `sleep 5` in the background.
    - *e.g.* The second job will run `echo "Hello, World!" | wc -c`.
3. Each job will be split up at `|` into a series of commands.
4. Each command will be spawned in a separate process.  
    - *e.g.* The first command in the second job will run `echo "Hello, World!"`.
    - *e.g.* The second command in the second job will run `wc -c`.
5. The output of one command will be piped to the input of the next command.
6. The output of the last command in the job will be printed to the console.

### Pitfalls to avoid
  
- **Some shell built-ins like `cd`, `cls`, `exit` and `jobs` are executed in the top-level shell process due to their nature.**  
  - *e.g.* `cd ..` will change the directory of the shell process.
  - *e.g.* `exit` will exit the shell process.
  - If found at the beginning of a job, the rest of the pipeline will not be executed.  
  - If found in the middle of a pipeline, the shell will complain about the command not being found.

- **The redirection operators `>`, `>>` and `<` must be space separated from the command and the file name.**  
  - *e.g.* `echo "Hello, World!" > file.txt` is valid.
  - *e.g.* `echo "Hello, World!">file.txt` is invalid.

## Setting up your workspace

This project has a [.editorconfig file](https://editorconfig.org/) to enforce project level coding standards.  
CLion has built-in support,
VSCode requires [a plugin](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig).

## How to run

> [!IMPORTANT]
> Requires a POSIX compliant system.

This project requires [CMake](https://cmake.org/) to build.
Your IDE (VSCode or CLion) should automatically detect the CMakeLists.txt file and build the project.
Install extensions for CMake support if prompted.  
If you are using the command line, you can run the following commands:

```bash
cmake -B build
cmake --build build --config Release
./build/cshell
```

## Generating and Viewing Documentation

This project uses [Doxygen](https://www.doxygen.nl/index.html) to generate documentation.  
If Doxygen is available on your system,  
You can generate the documentation by running the following command:

```bash
doxygen Doxyfile
```

This repository also has an automated workflow to generate documentatation via Github Actions.  

The generated documentation can be viewed at [/docs](./docs/index.html).

```bash
open docs/index.html # or open the file from the OS file manager
```

```bash
pushd docs ; python3 -m https.server 9999; popd # if you have python installed and want to use a server
```

A good starting point to explore the codebase is the [file listing page](./docs/files.html).
([files.html](files.html) if you are viewing this in a browser)

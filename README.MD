# C-shell

A shell written in C.

## Setting up your workspace

This project has a [.editorconfig file](https://editorconfig.org/) to enforce project level coding standards.  
CLion has built-in support,
VSCode requires [a plugin](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig).

## How to run

This project requires [CMake](https://cmake.org/) to build.
Your IDE (VSCode or CLion) should automatically detect the CMakeLists.txt file and build the project.
Install extensions for CMake support if prompted.  
If you are using the command line, you can run the following commands:

```bash
cmake -B build
cmake --build build --config Debug
```

Then execute the following commands to run the program:

Windows:

```bash
cd build
.\executable
```

Linux:

```bash
cd build
./executable
```

## Generating Documentation

This project uses [Doxygen](https://www.doxygen.nl/index.html) to generate documentation.  
If Doxygen is available on your system,  
You can generate the documentation by running the following command:

```bash
doxygen Doxyfile
```

This repository has automated workflow via Github Actions.  
Documentation is auto-generated and deployed to [Github Pages](https://kitswas.github.io/CPP_project_template/).  
Disable Actions on your own copy or delete the [workflows](./.github/workflows) folder if you do not want to be billed. This is **not my responsiblity.**

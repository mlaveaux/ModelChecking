# ModelChecking

This tool provides two different algorithms to check whether a given mu-calculus file combined with the labelled transition system evaluates to true or false.

## Installation guide

1. Download [CMake](https://cmake.org/).
1. In gui select source directory \<path_to_repository\>/tool/ and binary directory \<path_to_repository\>/build/
1. Select desired IDE.
1. Open the project file in IDE and build it.

## Using the tool

1. Open a terminal in the repository directory.
1. Type ./build/<path_to_executable> --algo=[naive|improved] \<path_to_lts\> \<path_to_mcf\>

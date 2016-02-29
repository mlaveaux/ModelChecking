/**
 * Copyright 2016 Algorithms for Model Checking.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "LabelledTransitionSystem.h"

#include <iostream>

/**
 * The main entry point for the program.
 */
int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: [--algo=naive|improved] <aldebaran filename> <mu-calculus filename>" << std::endl;
        std::cin.get(); return -1;
    }

    int argumentIndex = 1; // The zeroed argument is the filename itself.
    bool naiveAlgorithm = false; // Using the naive algorithm.
    
    if (argc == 4) {
        // Check which algorithm was specified, otherwise default to improved.g
        if (strcmp(argv[argumentIndex], "--algo=naive") == 0) {
            naiveAlgorithm = true;
        }
        ++argumentIndex; // The first filename is actually the third argument.
    }

    // Parse the LTS file directly.
    LabelledTransitionSystem system = LabelledTransitionSystem::parseAldebaranFormat(argv[argumentIndex++]);

    // Parse the mu-calculus file.

    // Evaluate the linear transition system with the given mu-calculus.
    if (naiveAlgorithm) {
        std::cout << "Using the naive algorithm." << std::endl;
    }
    else {
        std::cout << "Using the improved algorithm." << std::endl;
    }

    // It holds when it holds in all states and other it was false.
    std::cout << "The given mu-calculus is the false prophet.";
    std::cin.get();

    return 0;
}
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
#include "MuCalculus.h"

#include <iostream>
#include <string.h>

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
        // Check which algorithm was specified, otherwise default to improved.
        if (strcmp(argv[argumentIndex], "--algo=naive") == 0) {
            naiveAlgorithm = true;
        }
        ++argumentIndex; // The first filename is actually the third argument.
    }

    // Parse the LTS file directly.
	std::cout << "Parsing the LTS...\n";
    LabelledTransitionSystem system;
    if (!LabelledTransitionSystem::parseAldebaranFormat(argv[argumentIndex++], system)) {
        std::cin.get(); return -1;
    }
	std::cout << "Parsed the LTS!\n";

    // Parse the mu-calculus file.
	std::cout << "Parsing the mu-calculus formula...\n";
    MuFormula* formula = MuFormula::parseMuFormula(argv[argumentIndex++]);
    if (formula != nullptr) {
        std::cout << "Parsed the formula: " << formula->toString() << "\n";

        std::set<int> states; // The set of states in which the formula holds.

        // Evaluate the linear transition system with the given mu-calculus.
    	std::map<std::string, std::set<int>> variables;
        if (naiveAlgorithm) {
            states = formula->solve(system, variables);
        }
        else {

	    }

        if (states == system.getSetOfStates()) {
            std::cout << " holds" << std::endl;
        }
        else {
            std::cout << " doesn't hold" << std::endl;
        }
    }

    return 0;
}

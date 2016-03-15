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
    
    // Parse the LTS file.
    const char* ltsFilename = argv[argumentIndex++];

    LabelledTransitionSystem system;
    if (!LabelledTransitionSystem::parseAldebaranFormat(ltsFilename, system)) {
        std::cin.get(); return -1;
    }
        
    // Parse the mu-calculus file.
    MuFormula* formula = MuFormula::parseMuFormula(argv[argumentIndex++]);
    if (formula != nullptr) {
        std::cout << "Solving " << formula->toString();

        std::set<int> states; // The set of states in which the formula holds.

        // Evaluate the linear transition system with the given mu-calculus.
        std::map<std::string, std::set<int>> variables;
        if (!naiveAlgorithm) {
            std::cout << " using emerson-lei." << std::endl;

            // recursively set whether (sub-)formuli are closed
            formula->setFormulaClosedness();

            // initialize the variables map
            formula->initVarMaps(system, variables);
        }
        else {
            std::cout << " using the naive algorithm." << std::endl;
        }

        states = formula->solve(system, variables, naiveAlgorithm);
        
        if (states.find(system.getInitialState()) == states.end()) {
            std::cout << "formula doesn't hold" << std::endl;
        }
        else {
            std::cout << "formula holds" << std::endl;
        }
    }

    return 0;
}

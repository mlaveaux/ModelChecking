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

#include <assert.h>
#include <fstream>
#include <string>

LabelledTransitionSystem LabelledTransitionSystem::parseAldebaranFormat(const char* strFilename)
{
    assert(strFilename != nullptr); // nullptr is not allowed.

    std::ifstream file(strFilename);
    std::string line;

    // Verify that the first line consists of "des (<first_state>,<nr_of_transitions>,<nr_of_states>)".
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        // Read all the transitions and put them into the LTS.

    }

    return LabelledTransitionSystem();
}
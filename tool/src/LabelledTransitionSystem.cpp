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
#include <regex>
#include <assert.h>
#include <fstream>
#include <string>

bool LabelledTransitionSystem::parseAldebaranFormat(const char* strFilename, LabelledTransitionSystem& system)
{
    assert(strFilename != nullptr); // nullptr is not allowed.

    std::ifstream file(strFilename);
    if (file.fail()) {
        std::cerr << "Failed to open file " << strFilename << std::endl; return false;
    }

    bool firstLine = true;
    int numOfStates = -1;

    std::string line;
    while (std::getline(file, line)) {

        if (firstLine) {
            // des(<first_state> : int, <number_of_transitions> : int, <number_of_states> : int)
            size_t firstBracket = line.find_first_of("(");
            size_t firstComma = line.find_first_of(",");

            // Convert the first substring to a number.
            system.m_firstState = std::atoi(line.substr(firstBracket + 1, firstComma - firstBracket).c_str());

            // Read the number of transitions.
            std::string rest = line.substr(firstComma + 1);
            size_t secondComma = rest.find_first_of(",");
            int numberOfTransitions = std::atoi(rest.substr(0, secondComma).c_str());

            // Read the number of states.
            rest = rest.substr(secondComma + 1);
            size_t secondBracket = rest.find_first_of(")");
            int numStates = std::atoi(rest.substr(0, secondBracket).c_str());
            system.m_stateTransitions = std::vector<std::map<std::string, std::set<int>>>(numStates);
            numOfStates = numStates;

            firstLine = false;
        }
        else {
            // (start_state : int, transition_label : string, end_state : int)
            size_t firstBracket = line.find_first_of("(");
            size_t firstComma = line.find_first_of(",");

            // Read the first integer to start state.
            int fromState = std::atoi(line.substr(firstBracket + 1, firstComma - firstBracket).c_str());

            // Convert to transition label.
            std::string rest = line.substr(firstComma + 1);
            size_t secondComma = rest.find_first_of(",");
            std::string transitionLabel = rest.substr(1, secondComma - 2);

            // Read the last integer as going state.
            rest = rest.substr(secondComma + 1);
            size_t secondBracket = rest.find_first_of(")");
            int toState = std::atoi(rest.substr(0, secondBracket).c_str());

            // Check that state numbers do not exceed the state numbers (if they do change vector to map).
            if (fromState >= system.m_stateTransitions.size() || fromState < 0) {
                std::cerr << "State " << fromState << " can't be added as index to vector."; return false;
            }
            if (toState >= system.m_stateTransitions.size() || toState < 0) {
                std::cerr << "State " << toState << " can't be added as index to vector."; return false;
            }

            system.m_stateTransitions[fromState][transitionLabel].insert(toState);
        }
    }

    // Generate a set of a states that is accessed later on.
    for (int i = 0; i < numOfStates; i++) {
        system.m_setOfStates.insert(i);
    }

    return true;
}

int LabelledTransitionSystem::getNumStates() {
    return (int)m_setOfStates.size();
}

int LabelledTransitionSystem::getInitialState()
{
    return m_firstState;
}

std::set<int>& LabelledTransitionSystem::getSetOfStates()
{
    return m_setOfStates;
}

std::set<int>& LabelledTransitionSystem::getToStates(int state, const std::string& varLabel)
{
    return m_stateTransitions[state][varLabel];
}
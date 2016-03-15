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

#ifndef TOOL_LABELLEDTRANSITIONSYSTEM_H_
#define TOOL_LABELLEDTRANSITIONSYSTEM_H_

#include <set>
#include <string>
#include <vector>
#include <map>

 /**
  * Contains the description of a linear transition system, consisting of initial state,
  * the set of states and transition relations.
  */
class LabelledTransitionSystem
{
public:

    /**
     * Read the file specified by strFilename and return the LinearTransitionSystem described in it.
     */
    static bool parseAldebaranFormat(const char* strFilename, LabelledTransitionSystem& system);

    /**
     * Returns the number of states
     */
    int getNumStates();

    /**
     * Returns the initial state.
     */
    int getInitialState();

    /**
     * Returns a set of all states
     */
    std::set<int>& getSetOfStates();

    /**
     * Returns all states reachable from state by a transition labelled varLabel
     */
    std::set<int>& getToStates(int state, const std::string& varLabel);

private:
    int m_firstState;
    std::set<int> m_setOfStates;
    std::vector<std::map<std::string, std::set<int>>> m_stateTransitions;

};


#endif // TOOL_LABELLEDTRANSITIONSYSTEM_H_

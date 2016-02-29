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

#ifndef TOOL_LINEARTRANSITIONSYSTEM_H_
#define TOOL_LINEARTRANSITIONSYSTEM_H_

/**
 * Represents a single state.
 */
class State
{

};

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
    static LabelledTransitionSystem parseAldebaranFormat(const char* strFilename);

private:
    State m_initialState; // The initial state of the program.

};


#endif // TOOL_LINEARTRANSITIONSYSTEM_H_

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

#ifndef PGParser_h
#define PGParser_h

#include <stdio.h>  
#include <iostream>
#include <fstream>  
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "ParityGame.h"

/**
 * Parses a parity game file in .gm format and returns a ParityGame object.
 */
ParityGame parseParityGame(const char* pgFilename);

/**
 * Parses a single line from a .gm format parity game file.
 */
void parseLine(std::string& line, std::map<Vertex, std::set<Vertex>>& successors,
               std::map<Vertex, std::set<Vertex>>& predecessors, std::map<Vertex, int>& owner,
               std::map<Vertex, int>& priorities, std::map<int, int>& priorityCount);

/**
 * Splits a string based on a delimiter.
 */
std::vector<std::string> split(const std::string &s, char delim);

/**
 * Actual splitting happens here.
 */
std::vector<std::string> &split(const std::string &s, char delim,
	std::vector<std::string> &elems);

#endif /* PGParser_h */

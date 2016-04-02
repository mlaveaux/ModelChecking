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

#include "PGParser.h"

ParityGame parseParityGame(const char * pgFilename) {

	std::ifstream ifs(pgFilename);
	std::string line;
	std::map<Vertex, std::set<Vertex>> successors;
    std::map<Vertex, std::set<Vertex>> predecessors;
	std::map<Vertex, int> owner;
	std::map<Vertex, int> priorities;
	std::map<int, int> priorityCount;
    
    if (!ifs.good()) {
        throw std::runtime_error(std::string("Failed to open file ") + pgFilename);
    }

	std::getline(ifs, line); // Optimization: Read first line to initialize the vectors to size 
                             // (only one memory allocation required instead of multiple).
    
    while (std::getline(ifs, line)) {
		line.pop_back();
		parseLine(line, successors, predecessors, owner, priorities, priorityCount);
	}

	return ParityGame(successors, predecessors, owner, priorities, priorityCount);
}

void parseLine(std::string& line, std::map<Vertex, std::set<Vertex>>& successors,
               std::map<Vertex, std::set<Vertex>>& predecessors, std::map<Vertex, int>& owner,
               std::map<Vertex, int>& priorities, std::map<int, int>& priorityCount) {

	std::vector<std::string> lineSplit = split(line, ' ');
	int identifier = std::stoi(lineSplit.at(0));
	int priority = std::stoi(lineSplit.at(1));
	int ownedBy = std::stoi(lineSplit.at(2));
	std::vector<std::string> succSplit = split(lineSplit.at(3), ',');

	priorities[identifier] = priority;
	owner[identifier] = ownedBy;
	successors[identifier] = std::set<Vertex>();

	for (auto &succ : succSplit) {
        Vertex successor = std::stoi(succ);
		successors[identifier].insert(successor);
        
        // Add to predecessor set
        if (predecessors.find(successor) == predecessors.end()) {
            predecessors[successor] = std::set<Vertex>();
        }
        predecessors[successor].insert(identifier);
	}

	if (priorityCount.count(priority)) {
		priorityCount[priority] = ++priorityCount[priority];
	} else {
		priorityCount[priority] = 1;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::vector<std::string> &split(const std::string &s, char delim, 
	std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

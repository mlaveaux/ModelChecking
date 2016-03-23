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

#include "ParityGame.h"

ParityGame::ParityGame(std::map<Vertex, std::set<Vertex>>& successors,
	std::map<Vertex, int>& owner, std::map<Vertex, int>& priority,
	std::map<int, int>& priorityCount) {
	this->owner = owner;
	this->successors = successors;
	this->priority = priority;
	this->priorityCount = priorityCount;
}

bool ParityGame::isEven(Vertex vertex) {
	return owner[vertex];
}

std::set<Vertex> ParityGame::getOutgoingVertices(Vertex vertex) {
	return successors[vertex];
}

void ParityGame::print() {
	for (auto &v : successors) {
		std::cout << v.first << " " << priority[v.first] << " " << owner[v.first] << " ";
		for (auto &s : v.second) {
			std::cout << s << " ";
		}
		std::cout << "\n";
	}
}



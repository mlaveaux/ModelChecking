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
	std::map<Vertex, int>& owner,
    std::map<Vertex, int>& priority,
	std::map<int, int>& priorityCount)
{
	this->owner = owner;
	this->successors = successors;
	this->priority = priority;
	this->priorityCount = priorityCount;
}

bool ParityGame::isEven(Vertex vertex) const 
{
	return owner.at(vertex);
}

std::set<Vertex> ParityGame::getOutgoingVertices(Vertex vertex) const 
{
	return successors.at(vertex);
}

size_t ParityGame::getNumberOfVertices() const
{
    return successors.size();
}

int ParityGame::getPriority(Vertex vertex) const
{	
	if (priority.count(vertex)) {
		return priority.at(vertex);
	}
	return -1;
}

int ParityGame::getPriorityCount(int priority) const 
{
	if (priorityCount.count(priority)) {
		return priorityCount.at(priority);
	}
	return 0;
}

void ParityGame::print() const 
{
	for (auto &v : successors) {
		std::cout << v.first << " " << priority.at(v.first) << " " << owner.at(v.first) << " ";
		for (auto &s : v.second) {
			std::cout << s << " ";
		}
		std::cout << "\n";
	}
}

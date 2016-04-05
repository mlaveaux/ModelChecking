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

ParityGame::ParityGame(std::vector<std::set<Vertex>>& successors,
	std::vector<std::set<Vertex>>& predecessors,
	std::vector<int>& owner,
	std::vector<int>& priority,
	std::map<int, int>& priorityCount)
{
	this->owner = owner;
	this->successors = successors;
    this->predecessors = predecessors;
	this->priority = priority;
	this->priorityCount = priorityCount;
	if (priorityCount.count(0) == 0){
		this->maxPriority = priorityCount.size();
	} else {
		this->maxPriority = priorityCount.size() - 1;
	}
}

bool ParityGame::isEven(Vertex vertex) const 
{
	return (owner.at(vertex) == 0) ? true : false;
}

std::set<Vertex> ParityGame::getOutgoingVertices(Vertex vertex) const 
{
	return successors.at(vertex);
}

std::set<Vertex> ParityGame::getIncomingVertices(Vertex vertex) const
{
    return predecessors.at(vertex);
}

size_t ParityGame::getNumberOfVertices() const
{
    return successors.size();
}

int ParityGame::getPriority(Vertex vertex) const
{	
	return priority.at(vertex);
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
	for (int i = 0; i < successors.size(); ++i) {
		std::cout << i << " " << priority.at(i) << " " << owner.at(i) << " ";
		for (auto &s : successors.at(i)) {
			std::cout << s << " ";
		}
		std::cout << "\n";
	}
}

int ParityGame::getMaxPriority() const {
	return maxPriority;
}

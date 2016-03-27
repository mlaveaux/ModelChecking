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

#ifndef ParityGame_h
#define ParityGame_h

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <stdint.h>

using Measures = std::vector<unsigned int>;
using Vertex = uint32_t;

/**
 * Parity game object definition.
 *
 * @successors, mapping from a vertex to a set of its successor vertices.
 * @owner, mapping from a vertex to its owner. 0 for even, 1 for uneven.
 * @priority, mapping from a vertex to its priority.
 * @priorityCount, mapping from a priority to how often it occurs in the parity game.
 */
class ParityGame {

public:
	/**
	 * Constructor
	 */
	ParityGame(std::map<Vertex, std::set<Vertex>>& successors,
		std::map<Vertex, int>& owner, std::map<Vertex, int>& priority,
		std::map<int, int>& priorityCount);

	/**
	 * Returns the owner of a vertex. 
	 * 1 if even, 0 if uneven
	 */
	bool isEven(Vertex vertex) const;

	/**
	 * Returns the direct successors of a vertex. 
	 */
	std::set<Vertex> getOutgoingVertices(Vertex vertex) const;

	/**
	 * Returns the priority of a vertex.
	 */
	int getPriority(Vertex vertex) const;

	/**
	 * Returns the count of a priority.
	 */
	int getPriorityCount(int priority) const;

	/**
	 * Returns the number of vertices
	 */
	int getNumberOfVertices() const;

	/**
	 * Prints the parity game.
	 */
	void print() const;

private:
	std::map<Vertex, std::set<Vertex>> successors;
	std::map<Vertex, int> owner; 
	std::map<Vertex, int> priority;
	std::map<int, int> priorityCount;
};

#endif /* ParityGame_h */

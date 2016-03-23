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

using Measures = std::vector<unsigned int>;
using Vertex = uint32_t;

class ParityGame {

public:
	ParityGame(std::map<Vertex, std::set<Vertex>>& successors,
		std::map<Vertex, int>& owner, std::map<Vertex, int>& priority,
		std::map<int, int>& priorityCount);

	bool isEven(Vertex vertex);

	std::set<Vertex> getOutgoingVertices(Vertex vertex);

	void print();

private:
	std::map<Vertex, std::set<Vertex>> successors;
	std::map<Vertex, int> owner; 
	std::map<Vertex, int> priority;
	std::map<int, int> priorityCount;
};

#endif /* ParityGame_h */

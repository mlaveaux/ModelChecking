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
#include "PGSolver.h"

#include <algorithm>
#include <iostream>
#include <string.h>
#include <queue>


 /**
  * The main entry point for the program.
  */
int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: [--order=input|random|indegree|breadthfirst] <parity-game input filename>" << std::endl;
        return -1;
    }

    int argIndex = 1;
    std::string solveOrder = argv[argIndex++];
    const char* pgFilename = argv[argIndex++];

	try {
		ParityGame parityGame = parseParityGame(pgFilename);

		std::vector<Vertex> order;

		if (solveOrder == "--order=random") {
			// Vertices are handled such that order[i] = i.
			order = std::vector<Vertex>(parityGame.getNumberOfVertices());
			Vertex vert = 0;
			for (auto& next : order) {
				next = vert;
				++vert;
			}
			// Shuffle the order at random.
			std::random_shuffle(order.begin(), order.end());
		}
		else if (solveOrder == "--order=input") {
			order = std::vector<Vertex>(parityGame.getNumberOfVertices());
			// Vertices are handled such that order[i] = i.
			Vertex vert = 0;
			for (auto& next : order) {
				next = vert;
				++vert;
			}
		}
		else if (solveOrder == "--order=breadthfirst") {
			// Create the order and coloring vectors.
			order = std::vector<Vertex>(parityGame.getNumberOfVertices(), -1);
			std::vector<int> graphColoring(parityGame.getNumberOfVertices());

			// The queue for to be handled vertices.
			std::queue<Vertex> workQueue;

			// The current order being search and vertex beind handled.
			int ordering = 0;
			Vertex currentVertex = 0;

			while (ordering != parityGame.getNumberOfVertices()) {
				workQueue.push(currentVertex); // Add the first vertex.

				while (!workQueue.empty()) {
					// Pop the first element.
					Vertex current = workQueue.front();
					workQueue.pop();

					if (graphColoring[current] == 0) {
						// If the color is not yet set .
						for (auto& incomingVertex : parityGame.getIncomingVertices(current)) {
							workQueue.push(incomingVertex);
						}

						// Color the current vertex.
						graphColoring[current] = 1;
						order[ordering++] = current;
					}
				}

				// Select the smallest vertex not yet put into ordering.
				for (Vertex current = 0; current < graphColoring.size(); ++current) {
					if (graphColoring[current] == 0) {
						currentVertex = current;
						break;
					}
				}
			}
		}
		else if (solveOrder == "--order=indegree") {
			/*Vertex vert = 0;
			order = std::vector<Vertex>(parityGame.getNumberOfVertices());
			for (auto& next : order) {
				next = vert;
				++vert;
			}

			std::sort(order.begin(), order.end(), [&](Vertex a, Vertex b) {
				parityGame.getIncomingVertices(a).size() > parityGame.getIncomingVertices(b).size(;
			});*/

			std::vector<std::set<Vertex>> degrees(5, std::set<Vertex>());
			for (Vertex v = 0; v < parityGame.getNumberOfVertices(); v++) {
				size_t degree = parityGame.getIncomingVertices(v).size();
				if (degree >= degrees.size()) {
					degrees.resize(degree + 1, std::set<Vertex>());
				}
				degrees[parityGame.getIncomingVertices(v).size()].insert(v);
			}
			for (size_t i = degrees.size(); i-- > 0;) {
				for (Vertex v : degrees[i]) {
					order.push_back(v);
				}
			}
        }
        else {
            std::cerr << "input order must be one of [input|random|indegree|breadthfirst]"; 
            return -1;
        }

		// Solve the parity game.
		std::vector<bool> result = solveParityGame(parityGame, order);

		// Print the output sets.
		std::cout << "Even set: ";
		for (Vertex vert = 0; vert < result.size(); ++vert) {
			if (result[vert]) {
				std::cout << vert << " ";
			}
		}

		std::cout << std::endl << "Odd set: ";
		for (Vertex vert = 0; vert < result.size(); ++vert) {
			if (!result[vert]) {
				std::cout << vert << " ";
			}
		}
	}

    catch (std::exception& exception) {
        std::cerr << exception.what() << "\n";
        return -1;
    }

    return 0;
}
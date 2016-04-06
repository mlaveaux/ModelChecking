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
    // Input that might or should be specified.
    int liftingOrder = 0;
    bool fullOutput = false;
    const char* strPgFilename = nullptr;

    // Parse the input string into order, output and filename.
    for (int argIndex = 1; argIndex < argc; ++argIndex) {
        std::string argument = argv[argIndex];

        if (argument.find("--order=") != std::string::npos) {
            if (argument.find("input") != std::string::npos) {
                liftingOrder = 0;
            }
            else if (argument.find("random") != std::string::npos) {
                liftingOrder = 1;
            }
            else if (argument.find("indegree") != std::string::npos) {
                liftingOrder = 2;
            }
            else if (argument.find("breadthfirst") != std::string::npos) {
                liftingOrder = 3;
            }
        }
        else if (argument.find("--output=") != std::string::npos) {
            if (argument.find("winner") != std::string::npos) {
                fullOutput = false;
            }
            else if (argument.find("partition") != std::string::npos) {
                fullOutput = true;
            }
        }
        else {
            strPgFilename = argv[argIndex];
        }
    }

    // Check that all input was given.
    if (argc < 2 || strPgFilename == nullptr) {
        std::cout << "Usage: [--order=input|random|indegree|breadthfirst] [--output=winner|partition] <paritygame-filename>" << std::endl;
        return -1;
    }

    try {
        ParityGame parityGame = parseParityGame(strPgFilename);

        std::vector<Vertex> order;

        switch (liftingOrder) {
        case 0: {
            order = std::vector<Vertex>(parityGame.getNumberOfVertices());
            // Vertices are set such that order[i] = i.
            Vertex vert = 0;
            for (auto& next : order) {
                next = vert;
                ++vert;
            }
            break; 
        }
        case 1: {
            // Vertices are handled such that order[i] = i.
            order = std::vector<Vertex>(parityGame.getNumberOfVertices());
            Vertex vert = 0;
            for (auto& next : order) {
                next = vert;
                ++vert;
            }
            // Shuffle the order at random.
            std::random_shuffle(order.begin(), order.end());
            break;
        }
        case 2: {
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
            break;
        }
        case 3: {
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
        }

        // Solve the parity game.
        std::vector<bool> result = solveParityGame(parityGame, order, fullOutput);

        // Print the output sets.
        if (fullOutput) {
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

            std::cout << std::endl;
        }
        else {
            if (result[0]) {
                std::cout << "Even wins vertex 0" << std::endl;
            }
            else {
                std::cout << "Odd wins vertex 0" << std::endl;
            }
        }
    }

    catch (std::exception& exception) {
        std::cerr << exception.what() << "\n";
        return -1;
    }

    return 0;
}
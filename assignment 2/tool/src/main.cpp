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


/**
 * The main entry point for the program.
 */
int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: [--order=input|random|indegree|type2] <parity-game input filename>" << std::endl;
        return -1;
    }
    
    int argIndex = 1;
    std::string solveOrder = argv[argIndex++];
    const char* pgFilename = argv[argIndex++];
    
    try {
        ParityGame parityGame = parseParityGame(pgFilename);

        // Vertices are handled such that order[i] = i.
        std::vector<Vertex> order;
        Vertex vert = 0;
        for (auto& next : order) {
            next = vert;
            ++vert;
        }
        
        if (solveOrder == "--order=random") {
			order = std::vector<Vertex>(parityGame.getNumberOfVertices());
			Vertex vert = 0;
			for (auto& next : order) {
				next = vert;
				++vert;
			}
            // Shuffle the order at random.
            std::random_shuffle(order.begin(), order.end());
        }
		else if (solveOrder == "--order=input"){
			order = std::vector<Vertex>(parityGame.getNumberOfVertices());
			Vertex vert = 0;
			for (auto& next : order) {
				next = vert;
				++vert;
			}
		}
		else if (solveOrder == "--order=indegree"){
			std::vector<std::set<Vertex>> degrees(5, std::set<Vertex>());
			for (Vertex v = 0; v < parityGame.getNumberOfVertices(); v++){
				size_t degree = parityGame.getIncomingVertices(v).size();
				if (degree >= degrees.size()){
					degrees.resize(degree + 1, std::set<Vertex>());
				}
				degrees[parityGame.getIncomingVertices(v).size()].insert(v);
			}
			for (size_t i = degrees.size(); i-- > 0;){
				for (Vertex v : degrees[i]){
					order.push_back(v);
				}
			}
		}
		else if (solveOrder == "--order=type2"){

		}
		else{
			std::cout << "Usage: [--order=input|random|indegree|type2] <parity-game input filename>" << std::endl;
			return -1;
		}

        // Solve the parity game.
        std::vector<bool> result = solveParityGame(parityGame, order);

        /* Print the output sets.
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
        }  */    

    }
    catch (std::exception& exception) {
        std::cerr << exception.what() << "\n";
        return -1;
    }
    
    return 0;
}
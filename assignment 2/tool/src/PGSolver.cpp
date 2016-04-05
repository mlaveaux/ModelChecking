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

#include "PGSolver.h"

#include <assert.h>
#include <map>
#include <tuple>

using Measures = std::vector<uint32_t>;
const Measures TOP(1, 1);

/**
 * Gets the progress measures for a parity game
 */
static Measures getProgressMeasures(const ParityGame& game) {
    Measures progMeasures(game.getMaximumPriority() + 1);

    for (int i = 1; i < progMeasures.size(); i += 2) {
        progMeasures[i] = game.getPriorityCount(i);
    }

    return progMeasures;
}

/**
 * Computes whether a measure1 is lexicographically greater than measure2.
 *
 * @param[in] limit How many elements of the tuple to check.
 */
static bool lexicoGreaterThan(const Measures& measure1, const Measures& measure2, bool strict = true, unsigned int limit = -1)
{
    //cases for TOP
    if (measure2 == TOP) {
        if (measure1 == TOP) {
            return !strict;
        }
        else {
            return false;
        }
    }
    else {
        if (measure1 == TOP) {
            return true;
        }
        else { // when both are not TOP
            for (size_t index = 1; index < limit && index < measure1.size(); index += 2) {
                if (measure1[index] > measure2[index]) {
                    return true;
                }
            }
            return !strict; // Both are equal, so for strict this is false and otherwise true.
        }
    }
}

/**
 * Computes Prog, the progress measure.
 */
static Measures prog(const ParityGame& game, Measures& maxMeasures, const std::vector<Measures>& parProgMeasures, Vertex v, Vertex w)
{
    Measures newMeasure(maxMeasures.size(), 0);
    Measures succMeasure(parProgMeasures[w]);
    int priority = game.getPriority(v);

    //if the measure of w is TOP, the measure of v will be TOP as well
    if (succMeasure == TOP) {
        return TOP;
    }

    // case even priority
    if (priority % 2 == 0) {
        // fill with same values as w upto priority (no need to include priority since it is even), keep the rest zero
        for (int i = 1; i < priority; i += 2) {
            newMeasure[i] = succMeasure[i];
        }
    }
    // case for odd priority
    else {
        // try to make the measure strictly bigger than that of w, going from priority to 1
        // if not possible, it is TOP
        bool madeStrictlyGreater = false;
        for (int i = priority; i > 0; i -= 2) {
            if (!madeStrictlyGreater && succMeasure[i] < maxMeasures[i]) {
                newMeasure[i] = succMeasure[i] + 1;
                madeStrictlyGreater = true;
            }
            else {
                newMeasure[i] = succMeasure[i];
            }
        }
        if (!madeStrictlyGreater) {
            newMeasure = TOP;
            // does m = rhoish(w) = TOP also mean that rhoish(w) has to be adjusted?
        }
    }

    return newMeasure;
}

/**
* Lift the Measures for the specified vertex.
*/
Measures lift(const ParityGame& game, Measures maxMeasures, const std::vector<Measures>& progMeasures, Vertex vertex)
{
    Measures result;

    if (game.isEven(vertex)) {
        result = TOP;
		for (auto outgoingVertex : game.getOutgoingVertices(vertex)) {
			Measures progress = prog(game, maxMeasures, progMeasures, vertex, outgoingVertex);
			result = lexicoGreaterThan(progress, result) ? result : progress; // Minimize result
			// if it has the minimum value, return it immediately
			if (result == Measures(maxMeasures.size(), 0)) {
				return result;
			}
		}
    }
    else {
        result = Measures(maxMeasures.size(), 0);
		for (auto outgoingVertex : game.getOutgoingVertices(vertex)) {
			Measures progress = prog(game, maxMeasures, progMeasures, vertex, outgoingVertex);
			result = lexicoGreaterThan(progress, result) ? progress : result; // Maximize result
			// if it has the maximum value, return it immediately
			if (result == TOP) {
				return result;
			}
		}
    }

    return result;
}

std::vector<bool> solveParityGame(const ParityGame& game, const std::vector<Vertex>& order, bool fullPartition)
{
    // For every vector set the zeroed Measure tuple.
    std::vector<Measures> vertexToMeasures(game.getNumberOfVertices());
    for (auto& measure : vertexToMeasures) {
        measure = Measures(game.getMaximumPriority() + 1, 0);
    }

    // Gets the initial progress measures.
    Measures maxMeasures = getProgressMeasures(game);

    // Indicates whether the set of measures has been increased this iteration
    bool increased = false;

    do {
        increased = false;
        // Follow the order specified at the parameter.
        for (auto vertex : order) {
            // Lift a single vertex and and check whether its measures have increased.
            Measures& measures = vertexToMeasures[vertex];
            Measures newMeasures = lift(game, maxMeasures, vertexToMeasures, vertex);

            // If it has been lifted, fill it in in the measures and set it to increased
            if (lexicoGreaterThan(newMeasures, measures)) {
                vertexToMeasures[vertex] = newMeasures;
                increased = true;
            }
        }

    } while (increased);

    // Gather the set that don't equal Top and put true for them.
    if (fullPartition) {
        std::vector<bool> evenDominated = std::vector<bool>(game.getNumberOfVertices());

        Vertex index = 0;
        for (auto measures : vertexToMeasures) {
            if (measures != TOP) {
                // Not equal top implies winning set for even.
                evenDominated[index] = true;
            }

            ++index;
        }

        return evenDominated;
    }
    else {
        if (vertexToMeasures[0] != TOP) {
            return { true };
        }
        else {
            return { false };
        }
    }
}
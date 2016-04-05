#include "PGSolver.h"

#include <assert.h>
#include <map>
#include <tuple>
#include <stdint.h> //DONT REMOVE

using Measures = std::vector<uint32_t>;
const Measures TOP(1, 1);

/**
 * Gets the progress measures for a parity game
 */
static Measures getProgressMeasures(const ParityGame& game){
    Measures progMeasures(game.getMaximumPriority() + 1);

	for (int i = 1; i < progMeasures.size(); i += 2){
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
	const Measures& succMeasure = parProgMeasures[w];
	int priority = game.getPriority(v);

    // case even priority
	if (priority % 2 == 0) { 
		// fill with same values as w upto priority (no need to include priority since it is even), keep the rest zero
		for (int i = 1; i < priority; i += 2) {
			newMeasure[i] = succMeasure[i];
		}
	}
	// case for odd priority
	else{
		// try to make the measure strictly bigger than that of w, going from priority to 1
		// if not possible, it is TOP
		bool madeStrictlyGreater = false;
		for (int i = priority; i > 0; i -= 2) {
			if (!madeStrictlyGreater && succMeasure[i] < maxMeasures[i]) {
				newMeasure[i] = succMeasure[i] + 1;
				madeStrictlyGreater = true;
			}
			else{
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
static Measures lift(const ParityGame& game, Measures maxMeasures, const std::vector<Measures>& progMeasures, Vertex vertex)
{
	Measures result;
	if (game.isEven(vertex)){
		result = TOP;
	} else {
		result = Measures(maxMeasures.size(), 0);
	}
	//optimizations possible: look at selfloops (for instance, if vertex has self loop, is of player even, has odd priority: measure = TOP)

    // Sketch of code, but requires additional functionality in ParityGame.
    for (auto outgoingVertex : game.getOutgoingVertices(vertex)) {
        Measures progress = prog(game, maxMeasures, progMeasures, vertex, outgoingVertex);

        if (game.isEven(vertex)) {
            result = lexicoGreaterThan(progress, result) ? result : progress; // Minimize result
        } else {
            result = lexicoGreaterThan(progress, result) ? progress : result; // Maximize result
        }
    }

    return result;
}

std::vector<bool> solveParityGame(const ParityGame& game, const std::vector<Vertex>& order)
{
    // For every vector set the zeroed Measure tuple.
    std::vector<Measures> vertexToMeasures(game.getNumberOfVertices());
    for (auto& measure : vertexToMeasures) {
        measure = Measures(game.getMaximumPriority() + 1, 0);
    }

	// Gets the initial progress measures.
	Measures maxMeasures = getProgressMeasures(game);

    // Indicates whether some vertex can still be lifted.
    bool canLift = false;
    
    do {
        // Follow the order specified at the parameter.
        for (auto vertex : order) {
            // Lift a single vertex and and check whether its measures have increased.
            Measures& measures = vertexToMeasures[vertex];
            Measures newMeasures = lift(game, maxMeasures, vertexToMeasures, vertex);
            
            // Increase means that it can be lifted and it should stay when true.
            canLift = lexicoGreaterThan(measures, newMeasures) || canLift;

            if (canLift) {
                measures = newMeasures;
            }
        }

    } while (canLift);

    // Gather the set that don't equal Top and put true for them.
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

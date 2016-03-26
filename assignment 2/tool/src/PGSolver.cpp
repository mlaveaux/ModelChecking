#include "PGSolver.h"

#include <assert.h>
#include <map>
#include <tuple>

struct MeasuresSet {
    int size;
    std::vector<int> maxMeasures;

	MeasuresSet(int s) : size(s), maxMeasures(s, 0)
	{}
};

using Measures = std::vector<uint32_t>;
const Measures TOP(1,1);

/**
 * Gets the progress measures for a parity game
 */
struct MeasuresSet getProgressMeasures(ParityGame& game){
	int size = game.getNumberOfVertices();
	MeasuresSet progMeasures(size);
	for (int i = 1; i < size; i += 2){
		progMeasures.maxMeasures[i] = game.getPriorityCount(i);
	}
    return progMeasures;
}

/**
 * Computes whether a measure1 is lexicographically greater than measure2.
 * 
 * @param[in] limit How many elements of the tuple to check.
 */
bool lexicoGreaterThan(const Measures& measure1, const Measures& measure2, bool strict = true, unsigned int limit = -1)
{
	//cases for TOP
	if (measure2 == TOP){
		if (measure1 == TOP){
			return !strict;
		}
		else{
			return false;
		}
	}
	else{
		if (measure1 == TOP){
			return true;
		}
		else{ // when both are not TOP
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
 * Computes Prog
 */
Measures prog(ParityGame& game, MeasuresSet measuresSet, std::map<Vertex, Measures> parProgMeasures, unsigned int v, int w){
	int size = measuresSet.size;
	Measures newMeasure(size, 0);
	Measures succMeasure = parProgMeasures[v];
	int priority = game.getPriority(v);
	//case even priority
	if (priority % 2 == 0){
		//fill with same values as w upto priority (no need to include priority since it is even), keep the rest zero
		for (int i = 1; i < priority; i += 2){
			newMeasure[i] = succMeasure[i];
		}
	}
	//case for odd priority
	else{
		//try to make the measure strictly bigger than that of w, going from priority to 1
		//if not possible, it is TOP
		bool madeStrictlyGreater = false;
		for (int i = priority; i > 0; i -= 2){
			if (!madeStrictlyGreater && succMeasure[i] < measuresSet.maxMeasures[i]){
				newMeasure[i] = succMeasure[i] + 1;
				madeStrictlyGreater = true;
			}
			else{
				newMeasure[i] = succMeasure[i];
			}
		}
		if (!madeStrictlyGreater){
			newMeasure = TOP;
			//does m = rhoish(w) = TOP also mean that rhoish(w) has to be adjusted?
		}
	}
    return {};
}

/**
 * Lift the Measures for the specified vertex.
 */
Measures lift(ParityGame& game, MeasuresSet measuresSet, std::map<Vertex, Measures> progMeasures, Vertex vertex)
{
    Measures result = progMeasures[vertex];

	//optimizations possible: look at selfloops (for instance, if vertex has self loop, is of player even, has odd priority: measure = TOP)

    // Sketch of code, but requires additional functionality in ParityGame.
    for (auto outgoingVertex : game.getOutgoingVertices(vertex)) {
        Measures progress = prog(game, measuresSet, progMeasures, vertex, outgoingVertex);

        if (game.isEven(vertex)) {
            result = lexicoGreaterThan(progress, result) ? progress : result; // Minimize result
        } else {
            result = lexicoGreaterThan(progress, result) ? result : progress; // Maximize result
        }

        // Don't know what the first max means?
    }

    return std::move(result);
}

std::vector<bool> solveParityGame(ParityGame& game, const std::vector<Vertex>& order)
{
    // For every vector set the zeroed Measure tuple.
    std::map<Vertex, Measures> vertexToMeasures;

	//Get the set of progress measures
	MeasuresSet measuresSet = getProgressMeasures(game);

    // Indicates whether some vertex can still be lifted.
    bool canLift = false;
    
    do {
        // Follow the order specified at the parameter.
        for (auto vertex : order) {
            // Lift a single vertex and and check whether its measures have increased.
            Measures& measures = vertexToMeasures[vertex];
            Measures newMeasures = lift(game, measuresSet, vertexToMeasures, vertex);
            
            // Increase means that it can be lifted.
            canLift = lexicoGreaterThan(measures, newMeasures);
        }

    } while (canLift);

    // Gather the set that don't equal Top and put true for them.
    std::vector<bool> evenDominated = std::vector<bool>(game.getNumberOfVertices());
    
    for (auto element : vertexToMeasures) {
        //evenDominated[element.first] = element.second()
    }

    return{};
}

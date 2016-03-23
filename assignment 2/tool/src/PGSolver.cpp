#include "PGSolver.h"

#include <assert.h>
#include <map>

struct ProgMeasures {
    int size;
    std::vector<int> maxMeasures;
};

using Measures = std::vector<unsigned int>;
using Vertex = uint32_t;

/**
 * Gets the progress measures for a parity game
 */
struct ProgMeasures getProgressMeasures(ParityGame game){
    return {};
}

/**
 * Computes whether a measure1 is lexicographically greater than measure2.
 * 
 * @param[in] limit How many elements of the tuple to check.
 */
bool lexicoGreaterThan(const Measures& measure1, const Measures& measure2, bool strict = true, unsigned int limit = -1)
{
    for (size_t index = 0; index < limit && index < measure1.size(); ++index) {
        if (measure1[index] != measure2[index]) {
            if (measure1[index] > measure2[index]) {
                return true;
            }
        }
    }

    return !strict; // Both are equal, so for strict this is false and otherwise true.
}

/**
 * Computes Prog
 */
Measures prog(std::map<Vertex, Measures> parProgMeasures, unsigned int v, int w){
    return {};
}

/**
 * Lift the Measures for the specified vertex.
 */
Measures lift(const ParityGame& game, std::map<Vertex, Measures> progMeasures, Vertex vertex)
{
    Measures result = progMeasures[vertex];

    // Sketch of code, but requires additional functionality in ParityGame.
    for (auto outgoingVertex : game.getOutgoingVertices(vertex)) {
        Measures progress = prog(progMeasures, vertex, outgoingVertex);

        if (game.isEven(vertex)) {
            result = lexicoGreaterThan(progress, result) ? progress : result; // Minimize result
        } else {
            result = lexicoGreaterThan(progress, result) ? result : progress; // Maximize result
        }

        // Don't know what the first max means?
    }

    return std::move(result);
}

std::vector<bool> solveParityGame(const ParityGame& game, const std::vector<Vertex>& order)
{
    // For every vector set the zeroed Measure tuple.
    std::map<Vertex, Measures> vertexToMeasures;

    // Indicates whether some vertex can still be lifted.
    bool canLift = false;
    
    do {
        // Follow the order specified at the parameter.
        for (auto vertex : order) {
            // Lift a single vertex and and check whether its measures have increased.
            Measures& measures = vertexToMeasures[vertex];
            Measures newMeasures = lift(game, vertexToMeasures, vertex);
            
            // Increase means that it can be lifted.
            canLift = lexicoGreaterThan(measures, newMeasures);
        }

    } while (canLift);
}

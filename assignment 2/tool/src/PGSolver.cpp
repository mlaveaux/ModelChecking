#include "PGSolver.h"

#include <assert.h>
#include <map>

struct ProgMeasures {
    int size;
    std::vector<int> maxMeasures;
};

/**
 * Gets the progress measures for a parity game
 */
struct ProgMeasures getProgressMeasures(ParityGame game){
    return {};
}

/**
 * Computes whether a measure is greater than another measure
 * limit: upto how many elements to check
 * strict: whether it is strictly greater than or at least
 */
bool lexicoGreaterThan(const std::vector<int>& measure1, const std::vector<int>& measure2, int limit, bool strict)
{
    assert(measure1.size() >= limit);
    assert(measure2.size() >= limit);

    for (size_t index = 0; index < limit; ++index) {
        if (measure1[index] <= measure2[index] && strict) {
            return false;
        } else if (measure1[index] < measure2[index]) {
            return false;
        }
    }

    return true;
}

/**
 * Computes Prog
 */
std::vector<int> prog(std::map<int, std::vector<int>> parProgMeasures, int v, int w){
    return {};
}

/**
 * Bro do you even lift
 */
std::map<int, std::vector<int>> lift(std::map<int, std::vector<int>> parProgMeasures, int v){
    return {};
}

static std::vector<bool> solveParityGame(ParityGame game){
    return {};
}

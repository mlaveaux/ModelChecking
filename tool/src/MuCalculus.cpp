#define _CRT_SECURE_NO_WARNINGS

#include "MuCalculus.h"
#include "LabelledTransitionSystem.h"

#include <assert.h>
#include <exception>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

using std::ifstream;

/**
 * Parses a logic formula
 */
MuFormula* parseLogicFormula(std::string line, char pfp);

/**
 * Parses a subformula (line)
 * pfp is the last fixed point encountered (as defined in the header)
 */
MuFormula* parseSubFormula(std::string line, char pfp);

MuFormula::MuFormula(MuFormula* f1, MuFormula* f2, Op op, std::string varlabel, char pfp) :
    subformula(f1),
    subformula2(f2),
    operation(op),
    prevFixedPoint(pfp),
    varlabel(varlabel)
{}

//solves a mu calculus formula
std::set<int> MuFormula::solve(LabelledTransitionSystem& system) {

	std::set<int> set1;
	std::set<int> set2;
	std::set<int> temp_set;

    switch (operation) {
    case FALSE:
		//return empty set of states
		return std::set<int>();
    case TRUE:
		//return set of all states
		return system.getSetOfStates();
        break;
    case VAR:
        //fixed point magic
        break;
    case AND:
		//intersect set of states of the two subformula results
		set1 = subformula->solve(system);
		set2 = subformula2->solve(system);
		std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
			std::inserter(temp_set, temp_set.begin()));
		set1.clear(); set2.clear();
		return temp_set;
    case OR:
        //unite set of states of the two subformula results
		set1 = subformula->solve(system);
		set2 = subformula2->solve(system);
		std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(),
			std::inserter(temp_set, temp_set.begin()));
		set1.clear(); set2.clear();
		return temp_set;
    case DIAMOND:
        //diamond magic
        break;
    case BOX:
        //box magic
        break;
    case MU:
        //fixed point magic
        break;
    case NU:
        //fixed point magic
        break;
    }
}

MuFormula* MuFormula::parseMuFormula(const char* strFilename) {
    ifstream fin(strFilename);
    if (fin.bad()) {
        std::cout << "Could not open " << strFilename;
    }

    // read the formula
    std::string line;
    std::getline(fin, line);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    //parse the formula
    return parseSubFormula(line, 'x');
}

std::string MuFormula::toString() {
    switch (operation) {
    case FALSE:
        return "false";
    case TRUE:
        return "true";
    case VAR:
        return varlabel;
    case AND:
        return "(" + subformula->toString() + " && " + subformula2->toString() + ")";
    case OR:
        return "(" + subformula->toString() + " || " + subformula2->toString() + ")";
    case DIAMOND:
        return "<" + subformula->toString() + ">";
    case BOX:
        return "[" + subformula->toString() + "]";
    case MU:
        return "mu " + varlabel + ". " + subformula->toString();
    case NU:
        return "nu " + varlabel + ". " + subformula->toString();
    }

    assert(false); // All cases must be handled.
    return "Failed to parse MuFormula";
}

MuFormula* parseLogicFormula(std::string line, char pfp)
{
    //get the operation
    int opIndex = -1;
    bool opIsAnd = true;
    int depth = -1;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '(') {
            depth++;
        }
        else if (line[i] == ')') {
            depth--;
        }
        else if (line[i] == '&' && depth == 0) {
            opIndex = i;
            opIsAnd = true;
            break;
        }
        else if (line[i] == '|' && depth == 0) {
            opIndex = i;
            opIsAnd = false;
            break;
        }
    }
    //if no and or or is found at depth 0, throw excetion (should not happen)
    if (opIndex == -1) {
        throw 20;
    }

    //get the subformulas
    MuFormula* sub1;
    MuFormula* sub2;
    std::string subformula1 = line.substr(1, opIndex - 1);
    std::string subformula2 = line.substr(opIndex + 2, line.length() - opIndex - 3);

    if (subformula1.at(0) == '(') {
        sub1 = parseLogicFormula(subformula1, pfp);
    }
    else {
        sub1 = parseSubFormula(subformula1, pfp);
    }
    if (subformula2.at(0) == '(') {
        sub2 = parseLogicFormula(subformula2, pfp);
    }
    else {
        sub2 = parseSubFormula(subformula2, pfp);
    }

    //return the formula
    return new MuFormula(sub1, sub2, opIsAnd ? AND : OR, "", pfp);
}

MuFormula* parseSubFormula(std::string line, char pfp)
{
    switch (line.at(0)) {
    case 't':	//true
        return new MuFormula(nullptr, nullptr, TRUE, "", pfp);
    case 'f':	//false
        return new MuFormula(nullptr, nullptr, FALSE, "", pfp);
    case '(':	//start of logic formula
        return parseLogicFormula(line, pfp);
    case '<': {	//diamond
        size_t end = line.find('>');
        return new MuFormula(parseSubFormula(line.substr(end + 1, line.length() - end - 1), pfp), nullptr, DIAMOND, line.substr(1, end - 1), pfp);
    }
    case '[': {	//box
        size_t end = line.find(']');
        return new MuFormula(parseSubFormula(line.substr(end + 1, line.length() - end - 1), pfp), nullptr, BOX, line.substr(1, end - 1), pfp);
    }
    case 'm': { //greatest fixed point
        size_t dot = line.find('.');
        return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length() - dot - 1), 'm'), nullptr, MU, line.substr(2, dot - 2), pfp);
    }
    case 'n': { //least fixed point
        size_t dot = line.find('.');
        return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length() - dot - 1), 'n'), nullptr, NU, line.substr(2, dot - 2), pfp);
    }
    default:	//variable
        return new MuFormula(nullptr, nullptr, VAR, line, pfp);
    }

    assert(false); // All cases must be handled.
    return nullptr;
}

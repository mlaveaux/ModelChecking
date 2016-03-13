#include "MuCalculus.h"
#include "LabelledTransitionSystem.h"

#include <algorithm>
#include <assert.h>
#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

using std::ifstream;

/**
 * Parses a logic formula
 */
MuFormula* parseLogicFormula(std::string line, char pfp, std::map<std::string, char> fixedPoints);

/**
 * Parses a subformula (line)
 * pfp is the last fixed point encountered (as defined in the header)
 */
MuFormula* parseSubFormula(std::string line, char pfp, std::map<std::string, std::pair<char, bool>> fixedPoints);

MuFormula::MuFormula(MuFormula* f1, MuFormula* f2, Op op, std::string varlabel, char pfp, std::map<std::string, std::pair<char, bool>> fixedPoints) :
	subformula(f1),
	subformula2(f2),
	operation(op),
	prevFixedPoint(pfp),
	varlabel(varlabel),
	fixedPoints(fixedPoints)
{}

std::map<std::string, std::pair<char, bool>> MuFormula::getFixedPoints(){
	return fixedPoints;
}

//solves a mu calculus formula
std::set<int> MuFormula::solve(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables, bool naive) {
	std::set<int> result;

	switch (operation) {
	case FALSE:
		return std::set<int>(); // return empty set of states
	case TRUE:
		return system.getSetOfStates(); // return set of all states
		break;
	case VAR: {
		return variables[varlabel]; // return the approximation of the varlabel.
	}
	case AND: {
		//intersect set of states of the two subformula results
		std::set<int> subResult1 = subformula->solve(system, variables, naive);
		std::set<int> subResult2 = subformula2->solve(system, variables, naive);
		std::set_intersection(subResult1.begin(), subResult1.end(), subResult2.begin(), subResult2.end(),
			std::inserter(result, result.begin()));
		return result;
	}
	case OR: {
		//unite set of states of the two subformula results
		std::set<int> subResult1 = subformula->solve(system, variables, naive);
		std::set<int> subResult2 = subformula2->solve(system, variables, naive);
		std::set_union(subResult1.begin(), subResult1.end(), subResult2.begin(), subResult2.end(),
			std::inserter(result, result.begin()));
		return result;
	}
	case DIAMOND: {
		std::set<int> subResult1 = subformula->solve(system, variables, naive);
		//for each state
		for (int i = 0; i < system.getNumStates(); i++) {
			//for each out transition
			for (Transition trans : system.getOutTransitions(i)) {
				//if it has a transition with the correct label to a correct state, it complies with the formula
				if (trans.label == varlabel && subResult1.count(trans.toState) == 1) {
					result.insert(i);
					break;
				}
			}
		}
		return result;
	}
	case BOX: {
		std::set<int> subResult1 = subformula->solve(system, variables, naive);
		result = system.getSetOfStates();
		//for each state
		for (int i = 0; i < system.getNumStates(); i++) {
			//for each out transition
			for (Transition trans : system.getOutTransitions(i)) {
				//if it has a transition with the correct label to a wrong state, it does not comply with the formula
				if (trans.label == varlabel && subResult1.count(trans.toState) == 0) {
					result.erase(i);
					break;
				}
			}
		}
		return result;
	}
	case NU:
		if (naive){
			result = system.getSetOfStates(); // Start with all sets.
		}
		else{
			if (subformula->open) {
				if (prevFixedPoint == 'm') {
					subformula->openFormulaReset(system, variables);
				}
			}
		}
	case MU:
		bool reachedFixpoint = false; // Start with the empty set.
		// Result is the approximation for the varlabel.
		std::set<int> approximation;
		if (naive){
			approximation = result;
		}
		else{
			if (subformula->open && operation != NU) {
				if (prevFixedPoint == 'n') {
					subformula->openFormulaReset(system, variables);
				}
			}
			approximation = variables[varlabel];
		}
		while (true) {
			// Calculate the new approximation.
			std::set<int> newApprox = subformula->solve(system, variables, naive);

			// Check whether the fixed point is reached.
			if (approximation == newApprox) {
				break;
			}

			// Set the new approximation as the variable.
			approximation = newApprox;
		}

		return approximation;
	}
	assert(false); // All operations must return their own value;
	return{};
}

MuFormula* MuFormula::parseMuFormula(const char* strFilename) {
	ifstream fin(strFilename);
	if (fin.fail()) {
		std::cout << "Could not open " << strFilename;
		return nullptr;
	}

	// read the formula, skipping whitelines and comments (starting with %) 
	std::string line;
	do {
		std::getline(fin, line);
		std::cout << line << "\n";
	} while (line.length() < 2 || line.at(0) == '%');
	//remove spaces
	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

	//parse the formula
	return parseSubFormula(line, 'x', std::map<std::string, std::pair<char, bool>>());
}

void * MuFormula::openFormulaReset(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables) {
	if (operation == MU) {
		variables[varlabel] = std::set<int>();
	} else {
		variables[varlabel] = system.getSetOfStates();
	}
	
	return nullptr;
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
		return "<" + varlabel + ">" + subformula->toString();
	case BOX:
		return "[" + varlabel + "]" + subformula->toString();
	case MU:
		return "mu " + varlabel + ". " + subformula->toString();
	case NU:
		return "nu " + varlabel + ". " + subformula->toString();
	}

	assert(false); // All cases must be handled.
	return "Failed to parse MuFormula";
}

MuFormula* parseLogicFormula(std::string line, char pfp, std::map<std::string, std::pair<char, bool>> fixedPoints)
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
		sub1 = parseLogicFormula(subformula1, pfp, fixedPoints);
	}
	else {
		sub1 = parseSubFormula(subformula1, pfp, fixedPoints);
	}
	if (subformula2.at(0) == '(') {
		sub2 = parseLogicFormula(subformula2, pfp, fixedPoints);
	}
	else {
		sub2 = parseSubFormula(subformula2, pfp, fixedPoints);
	}

	//return the formula
	return new MuFormula(sub1, sub2, opIsAnd ? AND : OR, "", pfp, fixedPoints);
}

MuFormula* parseSubFormula(std::string line, char pfp, std::map<std::string, std::pair<char, bool>> fixedPoints)
{
	MuFormula* subformula;

	switch (line.at(0)) {
	case 't':	//true
		return new MuFormula(nullptr, nullptr, TRUE, "", pfp, fixedPoints);
	case 'f':	//false
		return new MuFormula(nullptr, nullptr, FALSE, "", pfp, fixedPoints);
	case '(':	//start of logic formula
		return parseLogicFormula(line, pfp, fixedPoints);
	case '<': {	//diamond
		size_t end = line.find('>');
		subformula = parseSubFormula(line.substr(end + 1, line.length() - end - 1), pfp, fixedPoints);
		fixedPoints = subformula->getFixedPoints();
		return new MuFormula(subformula, nullptr, DIAMOND, line.substr(1, end - 1), pfp, fixedPoints);
	}
	case '[': {	//box
		size_t end = line.find(']');
		return new MuFormula(parseSubFormula(line.substr(end + 1, line.length() - end - 1), pfp, fixedPoints), nullptr, BOX, line.substr(1, end - 1), pfp, fixedPoints);
	}
	case 'm': { //greatest fixed point
		size_t dot = line.find('.');
		std::string var = line.substr(2, dot - 2);
		//fixedPoints[var] = 'm'; 
		return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length() - dot - 1), 'm', fixedPoints), nullptr, MU, var, pfp, fixedPoints);
	}
	case 'n': { //least fixed point
		size_t dot = line.find('.');
		std::string var = line.substr(2, dot - 2);
		//fixedPoints[var] = 'n';
		return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length() - dot - 1), 'n', fixedPoints), nullptr, NU, var, pfp, fixedPoints);
	}
	default:	//variable
		return new MuFormula(nullptr, nullptr, VAR, line, pfp, fixedPoints);
	}

	assert(false); // All cases must be handled.
	return nullptr;
}

void * MuFormula::initVarMaps(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables) {
	switch (operation) {
	case FALSE:
	case TRUE:
	case VAR:
		break;
	case AND:
	case OR:
		subformula->initVarMaps(system, variables);
		subformula2->initVarMaps(system, variables);
		break;
	case BOX:
	case DIAMOND:
		subformula->initVarMaps(system, variables);
		break;
	case MU:
		variables[varlabel] = std::set<int>();
		subformula->initVarMaps(system, variables);
		break;
	case NU:variables[varlabel] = system.getSetOfStates();
		subformula->initVarMaps(system, variables);
		break;
	}
	return nullptr;
}

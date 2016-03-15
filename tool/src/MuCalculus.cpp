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
MuFormula* parseLogicFormula(std::string line, char pfp, std::map<std::string, char> vars);

/**
 * Parses a subformula (line)
 * pfp is the last fixed point encountered (as defined in the header)
 */
MuFormula* parseSubFormula(std::string line, char pfp, std::map<std::string, char> vars);

MuFormula::MuFormula(MuFormula* f1, MuFormula* f2, Op op, std::string varlabel, char pfp, std::map<std::string, char> vars) :
	subformula(f1),
	subformula2(f2),
	operation(op),
	prevFixedPoint(pfp),
	varlabel(varlabel),
	fixedPoints(vars),
	open(NULL)
{}

//solves a mu calculus formula
std::set<int> MuFormula::solve(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables, bool naive) {
	std::set<int> result;

	//for (auto mapelement : fixedPoints) { std::cout << mapelement.second << mapelement.first << "\n"; }

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
		if (naive) {
			result = system.getSetOfStates(); // Start with all sets.
		} else {
			if (prevFixedPoint == 'm') {
				subformula->openFormulaReset(system, variables, prevFixedPoint, operation);
			} 
		}
	case MU:
		// Result is the approximation for the varlabel.
		std::set<int>& approximation = variables[varlabel];

		if (naive) {
			approximation = result;
		} else {
			if (prevFixedPoint == 'n') {
				subformula->openFormulaReset(system, variables, prevFixedPoint, operation);
			} 
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

		return variables[varlabel];
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
	return parseSubFormula(line, 'x', std::map<std::string, char>());
}

void MuFormula::openFormulaReset(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables, char& surroundingBinder, Op &originalFixpoint) {
	if (open && (operation == NU || operation == MU)) {
		resetFormula(system, variables, surroundingBinder, originalFixpoint);
	} else {
		switch (operation) {
		case FALSE:
		case TRUE:
		case VAR:
			break;
		case AND:
		case OR:
			subformula->openFormulaReset(system, variables, prevFixedPoint, operation);
			subformula2->openFormulaReset(system, variables, prevFixedPoint, operation);
			break;
		case BOX:
		case DIAMOND:
			subformula->openFormulaReset(system, variables, prevFixedPoint, operation);
			break;
		case MU:
			subformula->openFormulaReset(system, variables, prevFixedPoint, operation);
			break;
		case NU:
			subformula->openFormulaReset(system, variables, prevFixedPoint, operation);
			break;
		}
	}
}

void MuFormula::resetFormula(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables, char& surroundingBinder, Op &originalFixpoint) {
	if (operation == originalFixpoint) {
		if (surroundingBinder == 'nu') {
			variables[varlabel] = system.getSetOfStates();
		} else {
			variables[varlabel] = std::set<int>();
		}
		subformula->openFormulaReset(system, variables, surroundingBinder, originalFixpoint);
	} 
	subformula->openFormulaReset(system, variables, surroundingBinder, originalFixpoint);
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

MuFormula* parseLogicFormula(std::string line, char pfp, std::map<std::string, char> vars)
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
		sub1 = parseLogicFormula(subformula1, pfp, vars);
	}
	else {
		sub1 = parseSubFormula(subformula1, pfp, vars);
	}
	if (subformula2.at(0) == '(') {
		sub2 = parseLogicFormula(subformula2, pfp, vars);
	}
	else {
		sub2 = parseSubFormula(subformula2, pfp, vars);
	}

	//return the formula
	return new MuFormula(sub1, sub2, opIsAnd ? AND : OR, "", pfp, vars);
}

MuFormula* parseSubFormula(std::string line, char pfp, std::map<std::string, char> vars)
{
	switch (line.at(0)) {
	case 't':	//true
		return new MuFormula(nullptr, nullptr, TRUE, "", pfp, vars);
	case 'f':	//false
		return new MuFormula(nullptr, nullptr, FALSE, "", pfp, vars);
	case '(':	//start of logic formula
		return parseLogicFormula(line, pfp, vars);
	case '<': {	//diamond
		size_t end = line.find('>');
		return new MuFormula(parseSubFormula(line.substr(end + 1, line.length() - end - 1), pfp, vars), nullptr, DIAMOND, line.substr(1, end - 1), pfp, vars);
	}
	case '[': {	//box
		size_t end = line.find(']');
		return new MuFormula(parseSubFormula(line.substr(end + 1, line.length() - end - 1), pfp, vars), nullptr, BOX, line.substr(1, end - 1), pfp, vars);
	}
	case 'm': { //greatest fixed point
		size_t dot = line.find('.');
		std::string var = line.substr(2, dot - 2);
		vars[var] = 'm';
		return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length() - dot - 1), 'm', vars), nullptr, MU, var, pfp, vars);
	}
	case 'n': { //least fixed point
		size_t dot = line.find('.');
		std::string var = line.substr(2, dot - 2);
		vars[var] = 'n';
		return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length() - dot - 1), 'n', vars), nullptr, NU, var, pfp, vars);
	}
	default:	//variable
		return new MuFormula(nullptr, nullptr, VAR, line, pfp, vars);
	}

	assert(false); // All cases must be handled.
	return nullptr;
}

void MuFormula::initVarMaps(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables) {
	if (operation == MU) {
		variables[varlabel] = std::set<int>();
	} else if (operation == NU) {
		variables[varlabel] = system.getSetOfStates();
	}

	if (operation == MU || operation == NU || operation == BOX || operation == DIAMOND) {
		subformula->initVarMaps(system, variables);
	} else if (operation == AND || operation == OR) {
		subformula->initVarMaps(system, variables);
		subformula2->initVarMaps(system, variables);
	}
}

void MuFormula::setFormulaClosedness(){
	if (open == NULL && (operation == MU || operation == NU)){
		std::set<std::string> childVars;
		std::set<std::string> boundVars;
		this->getChildVars(childVars);
		this->getBoundVars(boundVars);
		std::set<std::string> looseVars;
		std::set_difference(childVars.begin(), childVars.end(), boundVars.begin(), boundVars.end(),
			std::inserter(looseVars, looseVars.begin()));

		//std::cout << "--------------\nbinder:\n" << operation << varlabel << "\nchildren:\n";

		//for (auto child : childVars) { std::cout << child << "\n";}

		//std::cout << "bound vars: \n";

		//for (auto binder : boundVars) { std::cout << binder << "\n";}

		//std::cout << "loose vars: \n";

		//for (auto lv : looseVars) { std::cout << lv << "\n"; }

		//std::cout << "open: " << !looseVars.empty() << "\n";

		this->setOpen(!looseVars.empty());
		subformula->setFormulaClosedness();
	} else if (operation == BOX || operation == DIAMOND) {
		subformula->setFormulaClosedness();
	} else if (operation == AND || operation == OR) {
		subformula->setFormulaClosedness();
		subformula2->setFormulaClosedness();
	}
}

void MuFormula::getChildVars(std::set<std::string> &childVars){
	if (operation == MU || operation == NU || operation == VAR) {
		childVars.insert(varlabel);
	}

	switch (operation) {
	case FALSE:
	case TRUE:
	case VAR:
		break;
	case AND:
	case OR:
		subformula->getChildVars(childVars);
		subformula2->getChildVars(childVars);
		break;
	case BOX:
	case DIAMOND:
		subformula->getChildVars(childVars);
		break;
	case MU:
		subformula->getChildVars(childVars);
		break;
	case NU:
		subformula->getChildVars(childVars);
		break;
	}
}

void MuFormula::getBoundVars(std::set<std::string>& boundVars) {
	if (operation == MU || operation == NU) {
		boundVars.insert(varlabel);
	}

	switch (operation) {
	case FALSE:
	case TRUE:
	case VAR:
		break;
	case AND:
	case OR:
		subformula->getBoundVars(boundVars);
		subformula2->getBoundVars(boundVars);
		break;
	case BOX:
	case DIAMOND:
		subformula->getBoundVars(boundVars);
		break;
	case MU:
		subformula->getBoundVars(boundVars);
		break;
	case NU:
		subformula->getBoundVars(boundVars);
		break;
	}
}

void MuFormula::setOpen(bool open) {
	this->open = open;
}

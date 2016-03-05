#define _CRT_SECURE_NO_WARNINGS

#include "MuCalculus.h"
#include "LabelledTransitionSystem.h"

#include <fstream>
using std::ifstream;
#include <string>
#include <algorithm>
#include <iostream>

/*Constructor*/
MuFormula::MuFormula(MuFormula* f1, MuFormula* f2, Op op, std::string label, char pfp){
	subformula = f1;
	subformula2 = f2;
	operation = op;
	varlabel = label;
	prevFixedPoint = pfp;
}


//solves a mu calculus formula
void MuFormula::solve(LabelledTransitionSystem){
	switch (operation){
	case FALSE:
		//return empty set/list of states
		break;
	case TRUE:
		//return full set/list of states
		break;
	case VAR:
		//fixed point magic
		break;
	case AND:
		//intersect set/list of states of the two subformula results
		break;
	case OR:
		//unite set/list of states of the two subformula results
		break;
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

/*
 Parses a file for a MuFormula and returns the biggest MuFormula
 */
MuFormula* MuFormula::parseMuFormula(const char*){
	ifstream fin;
	std::string line;
	fin.open("data.txt"); // open a file
	if (!fin.good())
		throw 1; // exit if file not found

	bool done = false;

	// read the formula
	std::getline(fin, line);
	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
	//parse the formula
	return parseSubFormula(line, 'x');
}

/*
 Parses a subformula (line)
 pfp is the last fixed point encountered (as defined in the header)
 */
MuFormula* parseSubFormula(std::string line, char pfp){
	switch (line.at(0)){
	case 't':	//true
		return new MuFormula(nullptr, nullptr, TRUE, NULL, pfp);
		break;
	case 'f':	//false
		return new MuFormula(nullptr, nullptr, FALSE, NULL, pfp);
		break;
	case '(':	//start of logic formula
		parseLogicFormula(line, pfp);
		break;
	case '<':	//diamond
		int end = line.find('>');
		return new MuFormula(parseSubFormula(line.substr(end + 1, line.length - end - 1), pfp), nullptr, DIAMOND, line.substr(1, end - 1), pfp);
		break;
	case '[':	//box
		int end = line.find(']');
		return new MuFormula(parseSubFormula(line.substr(end + 1, line.length - end - 1), pfp), nullptr, BOX, line.substr(1, end - 1), pfp);
		break;
	case 'm':	//greatest fixed point
		int dot = line.find('.');
		return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length - dot - 1), 'm'), nullptr, MU, line.substr(2, dot - 2), pfp);
		break;
	case 'n':	//least fixed point
		int dot = line.find('.');
		return new MuFormula(parseSubFormula(line.substr(dot + 1, line.length - dot - 1), 'n'), nullptr, NU, line.substr(2, dot - 2), pfp);
		break;
	default:	//variable
		return new MuFormula(nullptr, nullptr, VAR, line, pfp);
		break;
	}
}

/*
  Parses a logic formula
  */
MuFormula* parseLogicFormula(std::string line, char pfp){
	
	//get the operation
	int opIndex = -1;
	bool opIsAnd = true;
	int depth = -1;
	for (int i = 0; i < line.length; i++){
		if (line[i] == '('){
			depth++;
		}
		else if (line[i] == ')'){
			depth--;
		}
		else if (line[i] == '&' && depth == 0){
			opIndex = i;
			opIsAnd = true;
			break;
		}
		else if (line[i] == '|' && depth == 0){
			opIndex = i;
			opIsAnd = false;
			break;
		}
	}
	//if no and or or is found at depth 0, throw excetion (should not happen)
	if (opIndex == -1){
		throw 20;
	}

	//get the subformulas
	MuFormula* sub1;
	MuFormula* sub2;
	std::string subformula1 = line.substr(1, opIndex - 1);
	std::string subformula2 = line.substr(opIndex + 2, line.length - opIndex - 3);

	if (subformula1.at(0) == '('){
		sub1 = parseLogicFormula(subformula1, pfp);
	}
	else{
		sub1 = parseSubFormula(subformula1, pfp);
	}
	if (subformula2.at(0) == '('){
		sub2 = parseLogicFormula(subformula2, pfp);
	}
	else{
		sub2 = parseSubFormula(subformula2, pfp);
	}

	//return the formula
	return new MuFormula(sub1, sub2, opIsAnd ? AND : OR, NULL, pfp);
}

/*
 Converts a MuFormula to a string (for testing purposes)
*/
std::string MuFormula::toString(){
	switch (operation){
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
}

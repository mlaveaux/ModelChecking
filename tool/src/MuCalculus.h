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

#ifndef TOOL_MUCALCULUS_H_
#define TOOL_MUCALCULUS_H_

#include "LabelledTransitionSystem.h"

#include <string>
#include <map>

/*
Represents the possible operations or values of the mu-calculus
*/
enum Op {
	FALSE,		//the value false
	TRUE,		//the value true
	VAR,		//a variable
	AND,		//logical and
	OR,			//logical or
	DIAMOND,	//the diamond operator
	BOX,		//the box operator
	MU,			//the least fixed point operator
	NU			//the greatest fixed point operator
};

/**
Represents a mu-calculus formula, recursively

subformula is only defined in case op in {AND, OR, DIAMOND, BOX, MU, NU}

subformula2 is only defined in case op in {AND, OR}

varlabel is only defined in case op in
- {DIAMOND, BOX} then it is a transition label
- {VAR, MU, NU} then it is a variable name

prevFixedPoint is either
- 'm' if the previous fixed point was a least fixed point
- 'n' if the previous fixed point was a greatest fixed point
- 'x' if there is no previous fixed point
this is needed for the Emerson-Lei expansion on the algorithm

fixedPoints maps all variables of fixed point formulas within the formula (including itself)
each variable is mapped to a pair containing
 - the fixed point it is bounded with
 - whether the formula is open (true = open, false = closed)
*/
class MuFormula
{
public:
	MuFormula(MuFormula* f1, MuFormula* f2, Op op, std::string varlabel, char pfp, std::map<std::string, std::pair<char, bool>> fixedPoints);

	/**
     * Solves this mu-calculus formula
     *
     * variables contains a set for each fixpoint variable of the current approximation.
     */
    std::set<int> solve(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables, bool naive);

	/**
	 * Computes the result of a mu-calculus formula 
	 * using Emerson and Lei's optimisation to compute fixed points.
	 */
	std::set<int> MuFormula::emersonLeiSolve(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables);
    
    /**
     * Parses a file for a MuFormula and returns the biggest MuFormula.
     */
	static MuFormula* parseMuFormula(const char* strFilename);

	/**
	 * If the formula is open we reset the state of its varlabel
	 */
	void * openFormulaReset(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables);

    /**
     * Converts a MuFormula to a string (for testing purposes)
     */
	std::string toString();

	/**
	 * Initialises the variables map.
	 *
	 */
	void* initVarMaps(LabelledTransitionSystem& system, std::map<std::string, std::set<int>>& variables);

	/**
	 * Getter for the included fixedpoints
	 */
	std::map<std::string, std::pair<char, bool>> getFixedPoints();

private:
    MuFormula* subformula;
    MuFormula* subformula2;
    Op operation;
    std::string varlabel;
    char prevFixedPoint;
	std::map<std::string, std::pair<char, bool>> fixedPoints;
};

#endif // TOOL_MUCALCULUS_H_

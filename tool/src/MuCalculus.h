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

/*
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
*/
class MuFormula
{
private:
	MuFormula* subformula;
	MuFormula* subformula2;
	Op operation;
	std::string varlabel;
	char prevFixedPoint;

public:
	MuFormula(MuFormula* f1, MuFormula* f2, Op op, std::string label, char pfp);
	//solves this mu-calculus formula
	//TODO: change void to data type used for collection of states
	void solve(LabelledTransitionSystem);

	static MuFormula* parseMuFormula(const char* strFilename);
	std::string toString();
};

#endif // TOOL_MUCALCULUS_H_
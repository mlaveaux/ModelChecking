#include "MuCalculus.h"
#include "LabelledTransitionSystem.h"

#include <fstream>
#include <string>

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
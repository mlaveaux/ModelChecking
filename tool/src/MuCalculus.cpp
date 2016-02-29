#include "MuCalculus.h"
#include "LinearTransitionSystem.h"

#include <fstream>
#include <string>



//solves a mu calculus formula
void MuFormula::solve(LinearTransitionSystem){
	switch (op){
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
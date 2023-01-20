#include "Rule.h"

Rule::Rule(const bool* surviveRule, const bool* birthRule) {
	memcpy(_surviveRule, surviveRule, 9);
	memcpy(_birthRule, birthRule, 9);
}

bool Rule::willSurvive(int numNeighbors) {
	return (numNeighbors < 9) && _surviveRule[numNeighbors];
}

bool Rule::willBorn(int numNeighbors) {
	return (numNeighbors < 9) && _birthRule[numNeighbors];
}

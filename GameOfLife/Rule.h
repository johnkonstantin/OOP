#ifndef GOL_RULE_H
#define GOL_RULE_H

#include <string>

class Rule {
private:
	bool _surviveRule[9] = {false};
	bool _birthRule[9] = {false};

public:
	Rule() = default;
	Rule(const bool[9], const bool[9]);

	bool willSurvive(int numNeighbors);
	bool willBorn(int numNeighbors);
};


#endif

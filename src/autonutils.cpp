// ================================ Imports ================================ //

#include "autonutils.h"
#include <vector>
#include <algorithm>
#include <functional>

// ========================== Autonomous Utilities ========================== //

autonutils::RoutineManager::RoutineManager() {}
autonutils::RoutineManager::~RoutineManager() {}

std::vector<int> autonutils::RoutineManager::find(int s)
{
	std::vector<int> matches;
	for (Routine &r : routines)
	{
		if (r.side == s || r.side == autonutils::FieldOrigin::Both) matches.push_back(r.id);
	}
	return matches;
}

void autonutils::RoutineManager::add(int i, int s, std::function<void()> a)
{
	Routine r;
	r.id = i, r.side = s;
	r.action = a;
	routines.push_back(r);
}

void autonutils::RoutineManager::exec(int i)
{
	for (Routine &r : routines)
	{
		if (r.id == i)
			r.action();
	}
}
#include "stdafx.h"
#include "Caster.h"

bool Caster::IsValidCasterValue(int caster)  {
	return FirstCaster <= caster && caster <= LastCaster;
}


int Caster::NumStrandsOnCaster(int caster) {
	if (caster == C2 || caster == C4 || caster == C5)
		return 1;
	else
		return 2;
}
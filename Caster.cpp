#include "stdafx.h"
#include "Caster.h"

bool Caster::IsValidCasterValue(int caster)  {
	return FirstCaster <= caster && caster <= LastCaster;
}


bool Caster::NumStrandsOnCaster(int caster) {
	return caster == C2;
}
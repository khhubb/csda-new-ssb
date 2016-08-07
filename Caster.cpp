#include "stdafx.h"
#include "Caster.h"

bool Caster::IsValidCasterValue(int caster)  {
	return FirstCaster <= caster && caster <= LastCaster;
}

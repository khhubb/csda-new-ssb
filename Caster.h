#pragma once
namespace Caster
{
	enum Caster {
		C1 = 1,
		C2,
		C3,
		C4,
		C5
	};



	enum {
		FirstCaster = C1,
		LastCaster = C5,
		CasterArrayLen = LastCaster + 1
	};

	bool IsValidCasterValue(int caster);
	bool NumStrandsOnCaster(int caster);

}


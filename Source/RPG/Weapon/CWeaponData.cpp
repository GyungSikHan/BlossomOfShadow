#include "CWeaponData.h"

UCSkills* UCWeaponData::GetSkills(int32 InIndex)
{
	if (Skills.Num() == 0)
		return nullptr;
	if (Skills.Num() - 1 < InIndex)
		return nullptr;

	return Skills[InIndex];
}

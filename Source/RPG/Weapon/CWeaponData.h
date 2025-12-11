#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class RPG_API UCWeaponData : public UObject
{
	GENERATED_BODY()

private:
	friend class UCWeaponAsset;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attahcment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }

public:
	class UCSkills* GetSkills(int32 InIndex);
private:
	UPROPERTY()
		class ACAttachment* Attahcment;
	UPROPERTY()
		class UCEquipment* Equipment;
	UPROPERTY()
		class UCDoAction* DoAction;
	UPROPERTY()
	TArray< class UCSkills* > Skills;
};

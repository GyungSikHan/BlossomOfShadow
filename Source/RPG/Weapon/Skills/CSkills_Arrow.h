#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_Arrow.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_Arrow : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
	TSubclassOf<AActor> ArrowClass;
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
	FVector ArrowLocation;

public:
	FORCEINLINE bool IsArrowSkill() { return bArrowSkill; }

public:
	UCSkills_Arrow();

	virtual void Skill_Pressed() override;
	
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;

private:
	bool bArrowSkill;

};

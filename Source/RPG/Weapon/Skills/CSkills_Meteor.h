#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_Meteor.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_Meteor : public UCSkills
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
	TSubclassOf<class ACMeteor> MeteorClass;
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
	FVector MeteorLocation;

public:
	UCSkills_Meteor();

	virtual void Skill_Pressed() override;

	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;
	
};

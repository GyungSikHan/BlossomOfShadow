#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CSkills_PlayAnimHit.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_PlayAnimHit : public UCSkills
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Action")
	bool isTarget;
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FHitData> HitDatas;

public:
	virtual void Skill_Pressed() override;
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;

private:
	UFUNCTION()
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther);
	UFUNCTION()
	void OnAttachmentEndCollision();

private:
	TArray<ACharacter*> Hitted;
	int32 HitIndex;
};

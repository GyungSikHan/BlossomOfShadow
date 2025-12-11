#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS()
class RPG_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther) override;
	virtual void OnAttachmentEndCollision() override;

private:
	//int32 Index;
	bool bEnable;
	bool bExsit;
	TArray<ACharacter*> Hitted;
	FRotator Target;
	float RandomIndex = 0.4;
};
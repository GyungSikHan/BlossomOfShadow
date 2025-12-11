#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/CDoAction.h"
#include "CDoAction_Warp.generated.h"

UCLASS(Blueprintable)
class RPG_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()
public:
	UCDoAction_Warp();

	virtual void BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
		TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitData) override;
	void Tick(float InDeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
	class APlayerController* PlayerController;
	class UDecalComponent* Decal;
	class UCAIBehaviorComponent* Behavior;

private:
	FVector MoveToLocation;
};

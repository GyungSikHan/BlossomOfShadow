#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "RPG/Characters/IMPStamina.h"
#include "CDoAction.generated.h"

UCLASS()
class RPG_API UCDoAction : public UObject
{
	GENERATED_BODY()
public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCDoAction();

	virtual void BeginPlay(class ACAttachment* InAttachment, class UCEquipment* InEquipment, ACharacter* InOwner, TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitData);
	virtual void Tick(float InDeltaTime) { }

	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();



public:
	UFUNCTION()
		virtual void OnBeginEquip() {};
	UFUNCTION()
		virtual void OnUnequip() {};

	UFUNCTION()
		virtual void OnAttachmentBeginCollision() {}
	UFUNCTION()
		virtual void OnAttachmentEndCollision() {}
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther) {}
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther) {}

protected:
	bool bBeginAction;
	bool bInAction;

	class ACharacter* OwnerCharacter;
	UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;

	int32 Index;

	IIMPStamina* character;
};

#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/CDoAction.h"
#include "CDoAction_Bow.generated.h"

UCLASS(Blueprintable)
class RPG_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class ACArrow> ArrowClass;

public:
	UCDoAction_Bow();

	virtual void BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitData) override;

	virtual void DoAction() override;
	virtual void Begin_DoAction()override;
	virtual void End_DoAction()	override;

	virtual void OnBeginEquip() override;
	virtual void OnUnequip()override;

	virtual void Tick(float InDeltaTime) override;

public:
	void End_BowString();

private:
	void CreateArrow();
	ACArrow* GetAttachedArrow();

	void DestroyArrow();

private:
	UFUNCTION()
	void OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter);
	UFUNCTION()
	void OnArrowEndPlay(ACArrow* InDestroyer);

private:
	USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	float* Bending;
	FVector OriginLocation;
	bool bAttachedString = true;

private:
	const bool* bEquipped;

private:
	TArray<ACArrow*> Arrows;
	class UCWeaponComponent* Weapon;
	bool test;
};

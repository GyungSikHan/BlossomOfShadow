#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CFeetComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class RPG_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bBow_Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bDefence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bShouldMove;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK")
	FFeetData FeetData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK")
	bool bFeet;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EWeaponType WeaponType = EWeaponType::Max;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	FVector FootTraces(FVector foot, FVector pelvis);

private:
	ACharacter* OwnerCharacter;
	class UCharacterMovementComponent* Movement;
	UCWeaponComponent* Weapon;
	class UCStateComponent* State;
	FRotator PrevRotation;
};

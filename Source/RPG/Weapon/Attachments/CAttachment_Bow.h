#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class RPG_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
	FVector2D ViewPitchRange = FVector2D(-90, +65);
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UPoseableMeshComponent* PoseableMesh;

public:
	float* GetBend();

public:
	ACAttachment_Bow();

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

private:
	FVector2D OriginViewPitchRange;

};

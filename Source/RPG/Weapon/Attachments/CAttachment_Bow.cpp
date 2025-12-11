#include "CAttachment_Bow.h"
#include "RPG/Weapon/Animinstance/CAnimInstance_Bow.h"
#include "AnimEncoding.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();
}

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = this->CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(Root);

	PoseableMesh = this->CreateDefaultSubobject<UPoseableMeshComponent>("PoseableMesh");
	PoseableMesh->SetupAttachment(Root);

	USkeletalMesh* mesh;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> skelAsset(L"/Script/Engine.SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	mesh = skelAsset.Object;
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	ConstructorHelpers::FClassFinder<UCAnimInstance_Bow> animAsset(L"/Script/Engine.AnimBlueprint'/Game/Weapon/Bow/ABP_CAnimInstance_Bow.ABP_CAnimInstance_Bow_C'");
	animInstance = animAsset.Class;
	SkeletalMesh->SetAnimInstanceClass(animInstance);
}


void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();
	AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);

	//PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	PoseableMesh->SetSkinnedAssetAndUpdate(SkeletalMesh->GetSkinnedAsset());
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
}

void ACAttachment_Bow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow_Left");

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
		OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;

		controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
	}

}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();
	AttachTo("Holster_Bow");

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}
}

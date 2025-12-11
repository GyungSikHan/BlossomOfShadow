#include "CDoAction_Bow.h"
#include "RPG/Components/CStateComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Weapon/CEquipment.h"
#include "RPG/Weapon/Add_On/CArrow.h"
#include "RPG/Weapon/Attachments/CAttachment_Bow.h"
#include "RPG/Weapon/Skills/CSkills.h"

UCDoAction_Bow::UCDoAction_Bow()
{
}

void UCDoAction_Bow::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionDatas, InHitData);

	SkeletalMesh = Cast<USkeletalMeshComponent>(InAttachment->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	PoseableMesh = Cast<UPoseableMeshComponent>(InAttachment->GetComponentByClass(UPoseableMeshComponent::StaticClass()));

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = bow->GetBend();
	OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();
}

void UCDoAction_Bow::DoAction()
{
	if(State->IsIdleMode() == false)
		return;

	if (character != nullptr && Index == 0)
	{
		if ((character->GetMp() - DoActionDatas[Index].MP) < 0 || (character->GetStamina() - DoActionDatas[Index].Stamina) < 0)
			return;
	}

	Super::DoAction();

	if (character != nullptr)
	{
		character->UseMp(DoActionDatas[Index].MP);
		character->UseStamina(DoActionDatas[Index].Stamina);
	}
	DoActionDatas[0].DoAction(OwnerCharacter);						
}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	bAttachedString = false;

	*Bending = 0;
	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	if(ArrowClass == nullptr)
		return;

	ACArrow* arrow = GetAttachedArrow();
	if(arrow == nullptr)
		return;
	
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	
	arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::UCDoAction_Bow::OnArrowHit);
	arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::UCDoAction_Bow::UCDoAction_Bow::OnArrowEndPlay);
	
	FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
	arrow->Shoot(forward);
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	CreateArrow();
}


void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreateArrow();
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0;
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
			Arrows[i]->Destroy();
	}
}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);

	if(bCheck == false)
		return;

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Bending");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100;
	bAttachedString = true;
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)
		return;

	FTransform transform;
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if(arrow == nullptr)
		return;
	
	arrow->AddIgnoreActor(OwnerCharacter);
	
	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");
	
	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACArrow* arrow : Arrows)
		if (!!arrow->GetAttachParentActor())
			return arrow;

	return nullptr;
}

void UCDoAction_Bow::DestroyArrow()
{
	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
			Arrows[i]->Destroy();
	}
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	if(HitDatas.Num() <= 0)
		return;

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}

void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}
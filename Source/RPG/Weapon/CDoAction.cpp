#include "CDoAction.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Characters/IMPStamina.h"
#include "RPG/Utilities/CLog.h"

UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
    TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitData)
{
    character = Cast<IIMPStamina>(InOwner);
    OwnerCharacter = InOwner;
    World = OwnerCharacter->GetWorld();

    State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
    Movement = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));

    DoActionDatas = InDoActionDatas;
    HitDatas = InHitData;
}

void UCDoAction::DoAction()
{
    bInAction = true;
    State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
    bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
    bInAction = false;
    bBeginAction = false;

    State->SetIdleMode();
    Movement->Move();
    Movement->DisableFixedCamera();
}

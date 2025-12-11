#include "CStatusComponent.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent()
{
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	Health = MaxHealth;
	MP = MaxMP;
	Stamina = MaxStamina;
}

void UCStatusComponent::SetHP(float InAmount)
{
	Health = InAmount;
}

void UCStatusComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::UseMp(float InAmount)
{
	MP += (InAmount * -1.0f);
	MP = FMath::Clamp(MP, 0.0f, MaxMP);
}

void UCStatusComponent::UseStamina(float InAmount)
{
	Stamina += (InAmount * -1.0f);
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
}

void UCStatusComponent::RecorveryMaxHP(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::RecorveryMp(float InAmount)
{
	MP += InAmount;
	MP = FMath::Clamp(MP, 0.0f, MaxMP);
}

void UCStatusComponent::RecorveryStamina(float InAmount)
{
	Stamina += InAmount;
	Stamina = FMath::Clamp(Stamina, 0, MaxStamina);
}

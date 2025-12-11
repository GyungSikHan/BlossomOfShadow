#include "CMontagesComponent.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent():
OwnerCharacter{}
{
	for (int i = 0; i < (int32)EStateType::Max; i++)
	{
		Datas[i] = nullptr;
	}
}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable ==nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<FMontagesData*> datas;
	DataTable->GetAllRows<FMontagesData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontagesData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
				continue;
			}
		}//for(data)
	}//for(i)

	//#if LOG_UCMontagesComponent
	for (FMontagesData* data : datas)
	{
		if (data == nullptr)
			continue;

		FString str;
		str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));
		str.Append(" / ");
		str.Append(data->Montage->GetPathName());
	}
	//#endif
}

void UCMontagesComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontagesComponent::PlayRollMode(int32 InIndex)
{
	if (InIndex == 0)
		PlayAnimMontage(EStateType::Roll_B);
	else if (InIndex == 1)
		PlayAnimMontage(EStateType::Roll_F);
	else if (InIndex == 2)
		PlayAnimMontage(EStateType::Roll_L);
	else if (InIndex == 3)
		PlayAnimMontage(EStateType::Roll_R);
	else if (InIndex == 4)
		PlayAnimMontage(EStateType::Roll_FL);
	else if (InIndex == 5)
		PlayAnimMontage(EStateType::Roll_FR);
	else if (InIndex == 6)
		PlayAnimMontage(EStateType::Roll_BL);
	else if (InIndex == 7)
		PlayAnimMontage(EStateType::Roll_BR);

}

void UCMontagesComponent::PlayHitDefenceMode()
{
	PlayAnimMontage(EStateType::HitAndDefence);
}

void UCMontagesComponent::PlayHitLandMode()
{
	PlayAnimMontage(EStateType::HitLand);
}

void UCMontagesComponent::PlayKoandGetup()
{
	PlayAnimMontage(EStateType::KoandGetup);
}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	if (OwnerCharacter == nullptr)
		return;

	FMontagesData* data = Datas[(int32)InType];

	if (data == nullptr || data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montage data");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}

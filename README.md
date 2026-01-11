# 📌 프로젝트 소개

“Blossom of Shadows”는 언리얼 엔진 기반으로 제작된 RPG 게임으로, 광활한 맵에서 외계에 적들을 물리치고 포탈을 넘어 외계 행성의 적을 쓰러트리는 이야기를 담고 있으며, 플레이어는 수많은 적들을 물리치고 살아남아 세상을 지켜내야 합니다.

# 🎮 게임 개발
>
> - **인원**: 1인  
> - **기간**: 24.01 ~ 24.10
> - **목적**: 언리얼 엔진과 C++을 공부한 것을 바탕으로 처음부터 끝까지 혼자서 게임을 개발하는 경험을 통해 엔진의 사용 방법을 익히기 위해 게임을 제작
> - **기술**: C++, Unreal Engine 5.4, Blueprint
</aside>

## 🖼 In-Game Screenshot

# **Core System Implementation**

## Character
### ✔ 설계 의도

### ✔ 구현 내용
#### ↳ AI
- [AI Controller](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Characters/AI/CAIController.cpp#L72-)

```cpp
void ACAIController::OnPossess(APawn* InPawn)
{
Super::OnPossess(InPawn);
	   Enemy = Cast<ACEnemy_AI>(InPawn);
	   State = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
	   MyTeam = Enemy->GetTeamID();
	   SetGenericTeamId(MyTeam);
	   if(Enemy->GetBehaviorTree() == nullptr)
		      return;
	   UBlackboardComponent* blackboard = Blackboard.Get();
	   UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, blackboard);

   Behavior = Cast<UCAIBehaviorComponent>(Enemy->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	   Behavior->SetBlackboard(blackboard);

   RunBehaviorTree(Enemy->GetBehaviorTree());
}
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	   TArray < AActor* > actors;
	   Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	   if(actors.Num() <= 0)
	   {
		      Blackboard->SetValueAsObject("Target", nullptr);
		      return;
	   }
	   for (AActor* actor : actors)
	   {
		      ACCharacter* character = Cast<ACCharacter>(actor);
		      float team = character->GetTeam();
		      if (team != MyTeam)
		      {
			         Behavior->SetTeamID(team);
			         Blackboard->SetValueAsObject("Target", actor);
			
			         return;
		      }
	   }
}
```

## Component

### ✔ 설계 의도

### ✔ 구현 내용

#### ↳ MontageComponent 

- [FMontageData](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CMontagesComponent.h#L11-L22)
    - Montage Component에 있는 FMontagesData 구조체는 FTableRowBase를 상속받아 구현
    - 데이터 테이블 구현시 MontagesData라는 이름으로 검색하여, 구조체의 정보를 토대로 값을 가져올 수 있는 데이터 테이블을 구현
    - 구조체의 정보는 StateComponent에 만들어 놓은 Enum 타입의 정보 중 한 가지 타입으로 설정한 뒤, 캐릭터의 상태가 설정해 놓은 상태일 때 저장된 몽타주를 설정한 속도로 실행할 수 있는 정보를 가진 구조체
    
         <table>
            <tr>
                <td align="center">
                    <img src="image-1.png" width="300"><br>
                    <em>몽타주 데이터 테이블</em>
                </td>
        </table>

```cpp
USTRUCT()
struct FMontagesData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStateType Type = EStateType::Max;
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
};
```
- [UCMontageComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CMontagesComponent.cpp#L54-L115)
    - 데이터 테이블을 통해 데이터를 더 편리하고 직관적으로 관리할 수 있게 구현
    - 캐릭터가 Component를 객체지향 5대 원칙 중 개방-폐쇄 원칙에 위배되지 않도록, 코드 수정 없이 기능을 변경할 수 있게 구현
        <table>
            <tr>
                <td align="center">
                    <img src="image-2.png" width="500"><br>
                    <em>몽타주 데이터 테이블</em>
                </td>
                <td align="center">
                    <img src="image-3.png" width="400"><br>
                    <em>데이터 테이블을 활용한 몽타주 재생</em>
                </td>
            </tr>
        </table>

        
```cpp
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
```

#### ↳ CZoomComponent
- [UCZoomComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CZoomComponent.cpp)
    - 마우스 휠을 입력시 TargetArmLength를 변화시켜 카메라가 이동하도록 구현
    - FMath::Clamp() 함수를 사용하여 카메라 이동이 자연스럽게 이루어지도록 구현

    <table>
        <tr>
            <td align="center">
                <img src="image-6.png" width="500"><br>
                <em>기본</em>
            </td>
            <td align="center">
                <img src="image-7.png" width="500"><br>
                <em>Zoom In</em>
            </td>
            <td align="center">
                <img src="image-8.png" width="500"><br>
                <em>Zoom Out</em>
            </td>
        </tr>
    </table>

```cpp

void UCZoomComponent::BeginPlay()
{
Super::BeginPlay();

   SpringArm = Cast<USpringArmComponent>(GetOwner()
    ->GetComponentByClass(USpringArmComponent::StaticClass()));
	   if(SpringArm == nullptr)
		      return;

   CurrentValue = SpringArm->TargetArmLength;
}

void UCZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	  if(SpringArm == nullptr)
		      return;

            if(UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, CurrentValue) == true)
		      return;
	   SpringArm->TargetArmLength = 
    UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, 
    CurrentValue, DeltaTime, InterpSpeed);
}

void UCZoomComponent::SetZoomValue(float InValue)
{
	   CurrentValue += (Speed * InValue);
	   CurrentValue = FMath::Clamp(CurrentValue, Range.X, Range.Y);
}
```

#### ↳ CTargetComponent
- [UCTargetComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CTargetComponent.cpp)
    - 키 입력 시 카메라 안에 들어오는 캐릭터들 중 일정 범위 내에서 하나의 캐릭터에 카메라를 고정하는 기능을 하는 컴포넌트
    - UKismetSystemLibrary::SphereTraceMultiByProfile() 함수를 사용하여 일정 범위 내에서 Hit된 캐릭터들을 가져옴
    - Hit된 캐릭터들을 이용해 UCBF_NearlyAngle::GetNearlyFrontAngle() 함수에서 하나를 선택해 리턴함
    - 반환된 캐릭터는 ChangeTarget() 함수에서 이펙트를 추가하고, Target 변수에 그 캐릭터를 저장
    - TickTarget() 함수에서 플레이어와 카메라가 좌우로 움직일 때 계속해서 Target 캐릭터로 돌아가도록 구현

        <table>
            <tr>
                <td align="center">
                <img src="image-9.png" width="200"><br>
                <em>Target</em>
            </td>
        </table> 
```cpp
void UCTargetComponent::BeginTarget()
{
    .....
	   TArray<FHitResult> hitResults;
	   UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), 
    start, end, TraceDistance, 
    "Targetting", false, actors, DrawDebug, hitResults, true);

	   for (FHitResult hit : hitResults)
		  if (hit.GetActor()->GetClass() != Character->GetClass())
			     targets.Add(Cast<ACharacter>(hit.GetActor()));

	   UCBF_NearlyAngle* helpers{};
	   ACharacter* target = helpers->GetNearlyFrontAngle(Character, 
    targets);
	   
    ChangeTarget(target);
}

void UCTargetComponent::ChangeTarget(ACharacter* InCandidate)
{
   	if(InCandidate == nullptr)
	   {
		      EndTarget();
		      return;
	   }

	   bTargetting = true;

	   if (Particle != nullptr)
		      Particle->DestroyComponent();

	   Particle = 
    UGameplayStatics::SpawnEmitterAttached(ParticleAsset, 
    InCandidate
    ->GetMesh(), "Root", ParticleLocation, FRotator(0, 0, 0), 
    ParticleScael);
	   Target = InCandidate;
}

void UCTargetComponent::TickTarget()
{
	   FRotator ControlRotation = Character->GetControlRotation();
	   FRotator find = 
    UKismetMathLibrary::FindLookAtRotation(Character
    ->GetActorLocation(), Target->GetActorLocation());
	   FRotator ownerToTarget = FRotator(ControlRotation.Pitch, 
    find.Yaw, find.Roll);

    if(UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, ownerToTarget, FinishAngle))
	   {
		      Character->GetController()
        ->SetControlRotation(ownerToTarget);
		      if (bMovingFocus == true)
			         bMovingFocus = false;
	   }
	   else
		      Character->GetController()
        ->SetControlRotation(UKismetMathLibrary::RInterpTo
        (ControlRotation,UKismetMathLibrary::MakeRotator
        (ownerToTarget.Roll, ownerToTarget.Pitch, 
        ownerToTarget.Yaw), GetWorld()->GetDeltaSeconds(), 
        InterpSpeed));
}
```
- [UCBF_NearlyAngle](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Utilities/CBF_NearlyAngle.cpp)
    - UCBF_NearlyAngle::GetNearlyFrontAngle 함수를 통해 UCTargetComponent에서 전달된 캐릭터들과 플레이어 사이의 내적을 계산하여, 가장 가까운 캐릭터를 반환하도록 구현
```cpp
ACharacter* UCBF_NearlyAngle::GetNearlyFrontAngle(ACharacter* InCharacter, TArray<ACharacter*> InArray)
{
	   float angle{};
	   ACharacter* candidate{};

	   for (ACharacter* array : InArray)
	   {
		      FVector location = array->GetActorLocation() - InCharacter->GetActorLocation();
		      FVector forwardVector = UKismetMathLibrary::GetForwardVector(InCharacter
        ->GetControlRotation());
		      float dot = UKismetMathLibrary::Dot_VectorVector(location.GetSafeNormal2D(), 
        forwardVector);

		      if(dot >= angle)
		      {
			        angle = dot;
			        candidate = array;
		      }
	   }

	   return candidate;
}
```
#### ↳ UCFeetComponent
<table>
    <tr>
        <td align="center">
        <img src="image-12.png" width="150"><br>
        <em>IK 적용전</em>
    </td>
        <td align="center">
        <img src="image-13.png" width="150"><br>
        <em>IK 적용후</em>
    </td>
</table> 
- [FFeetData](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CFeetComponent.h#L8-L28)
    - 캐릭터의 AnimInstance에서 IK를 위해 가져와야 할 값을 묶은 구조체
    <table>
        <tr>
            <td align="center">
            <img src="image-10.png" width="500"><br>
            <em>IK 설정 값</em>
        </td>
    </table> 

```cpp
USTRUCT(BlueprintType)
struct FFeetData
{
GENERATED_BODY()

public:
	   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	  	FVector LeftDistance = FVector::ZeroVector; //X

	   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	  	FVector RightDistance = FVector::ZeroVector; //X

   	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
  		FVector PelvisDistance = FVector::ZeroVector; //Z

	   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
		  FRotator LeftRotation = FRotator::ZeroRotator;

	   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
		  FRotator RightRotation = FRotator::ZeroRotator;
};
```

- [UCFeetComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CFeetComponent.cpp#L24-82)
    - IK를 적용할 소켓을 가져와 Trace() 함수에서 소켓의 X, Y 값과 캐릭터의 위치 Z 값을 합쳐 새로운 벡터를 만들어 LineTrace가 시작할 벡터를 생성
    - 새로 구한 벡터의 Z 값에서 캡슐의 절반 높이와 TraceDistance 값을 각각 빼주어, 발보다 조금 더 아래까지 구한 값을 Z로 설정하고, 소켓의 X, Y 값과 합쳐 LineTrace가 끝날 벡터 생성
    - 두 벡터를 이용하여 UKismetSystemLibrary::LineTraceSingle() 함수로 Hit된 ImpactPoint와 TraceEnd의 차이를 OffsetDistance만큼 보정하고, TraceDistance만큼 빼주어 소켓이 올라가야 할 거리를 구함
    - Hit된 Normal 벡터의 Y, X 값을 가져와 Normal 벡터의 Z 값과 DeAtn2를 사용하여 회전할 각도를 각각 구한 후, roll과 pitch 값에 저장
    - roll에는 Y, Z 값을 이용해 구한 값, pitch에는 X, Z 값을 사용한 값에 -를 붙인 값을 넣어 회전 값을 구함
    - 그 이유는 Unreal 엔진에서 사용하는 좌표계와 애니메이션을 만들 때 사용하는 좌표계가 달라서 이렇게 설정해야 함
    - Trace에서 구한 값을 이용하여 TickComponent() 함수에서 왼발과 오른발 중 더 작은 값을 사용하여 PelvisDistance.Z와 LeftDistance.X/RightDistance.X, Left/RightRotation을 FInterpTo/RInterpTo 함수를 사용해 자연스럽게 변화 함

```cpp
void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance, rightDistance;
	FRotator leftRotation, rightRotation;

	Trace(LeftSocket, leftDistance, leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
 	  FVector soket = OwnerCharacter->GetMesh()->GetSocketLocation(InName);
   	float z = OwnerCharacter->GetActorLocation().Z;
	   FVector start = FVector(soket.X, soket.Y, z);
   	z = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
    FVector end = FVector(soket.X, soket.Y, z);

	   TArray<AActor*> ignores;
	   ignores.Add(OwnerCharacter);
	   FHitResult hitResult;
	   UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, true, 
    ignores, DrawDebug, hitResult, true, FLinearColor::Green, FLinearColor::Red);
	   OutDistance = 0;
	   OutRotation = FRotator::ZeroRotator;
	   if(hitResult.bBlockingHit == false)
		      return;
	   float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	   OutDistance = length + OffsetDistance - TraceDistance;
	   float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	   float pitch = -UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);
	   OutRotation = FRotator(pitch, 0, roll);
}
```
- [UCAnimInstace](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Characters/CAnimInstance.cpp#L35-L88)
    - CFeetComponent에서 구한 값을 FFeetData 변수에 저장한 후, CAnimInstance 클래스에서 이를 가져와 블루프린트에서 Transform Bone과 TwoBoneIK에 적용하여 자연스러운 애니메이션을 구현하기 위해 IK를 적용
    -조금 더 자연스러운 애니메이션을 위해 걷거나 뛰는 애니메이션마다 Left/Right Foot이라는 이름의 Curve를 만들어 Alpha 값으로 사용하고, 이동 시 어색한 애니메이션이 자연스럽게 변화하도록 구현
    <table>
        <tr>
            <td align="center">
            <img src="image-14.png" width="500"><br>
            <em>AinmInstace IK 적용</em>
        </td>
    </table> 

    
```cpp
void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	   Super::NativeUpdateAnimation(DeltaSeconds);

	.....
	   
    UCFeetComponent* feet = Cast<UCFeetComponent>(OwnerCharacter
    ->GetComponentByClass(UCFeetComponent::StaticClass()));

	   bFeet = false;
	   if(feet != nullptr)
	   {
		      bFeet = true;
		      FeetData = feet->GetData();
	   }
}
```

#### ↳ 이외의 Component
- [UCStateComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CStateComponent.cpp)
    - Idle, Hit, Equipment 등 행동을 관리하는 컴포넌트로 구현
- [UCStatusComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CStatusComponent.cpp)
    - 캐릭터들의 체력, MP, Stamina를 관리하고 변화를 적용하는 컴포넌트로 구현
- [UCWeaponComponent](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Components/CWeaponComponent.cpp)
    - 캐릭터가 무기 장착, 공격 및 스킬 사용시 무기에 직접 명령을 내리는 것이 아니라 이 컴포넌트를 통해 명령을 내리도록 구현

#### ↳ Player Input
- [ACPlayer 설정](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Characters/CPlayer.h#L35-L76)
    - 플레이어의 입력은 언리얼 엔진 5에서 새롭게 추가된 EnhancedInput.InputMappingContext와 EnhancedInput.InputAction을 이용하여 구현
    - CPlayer에서 직렬화된 변수로 블루프린트에서 만든 EnhancedInput.InputAction을 연결해 주었고, EnhancedInput.InputMappingContext에서는 원하는 키 입력과 트리거 등에 의해 EnhancedInput.InputAction이 호출될 수 있도록 구현

    <table>
        <tr>
            <td align="center">
            <img src="image-15.png" width="500"><br>
            <em>IMC 매핑</em>
        </td>
    </table> 
```cpp
UCLASS()
class RPG_API ACPlayer : public ACCharacter
{
	GENERATED_BODY()
    //Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Jumping;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Avoid;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Sword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Hammer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Rod;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Bow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Warp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Zooming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Targetting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ChangeTargetting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Skill1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Skill2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Menu;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Pause;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ControllUI;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
    ....
}
```

## Component/Interface
### ✔ 설계 의도

### ✔ 구현 내용

#### ↳ Character Interface  
- [ICharacter](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Characters/Interface/ICharacter.h#L7-L22), [CAnimNotify_EndState](https://github.com/GyungSikHan/BlossomOfShadow/blob/main/Source/RPG/Notifys/CAnimNotify_EndState.cpp#L9-L47)
    - ICharacter는 플레이어, 몬스터, 보스 등에서 동일하게 사용할 수 있는 애니메이션 몽타주 함수를 인터페이스 화
    - ICharacter 인터페이스를 상속받아 구현된 CCharacter 클래스의 함수들은 애니메이션 몽타주가 끝날 때 이를 알리는 Notify 클래스를 만들어, Switch 문을 통해 현재 상태에 맞는 case에서 함수 호출
    - 인터페이스 함수들이 호출되면 EStateType을 Idle 상태로 되돌리는 등의 로직을 CCharacter에 구현하여, 다른 행동을 할 수 있도록 함
    
    
        <table>
            <tr>
                <td align="center">
                    <img src="image-4.png" width="400"><br>
                    <em>Back Step 몽타주 재생 후 상태 변경</em>
                </td>
                <td align="center">
                    <img src="image-5.png" width="380"><br>
                    <em>Dead 몽타주 재생 후 상태 변경</em>
                </td>
            </tr>
        </table>

```cpp
void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
....

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());

	if (character == nullptr)
		return;

	switch (StateType)
	{
	case EStateType::BackStep:
		character->End_BackStep();
		break;
	case EStateType::Roll_F:
	case EStateType::Roll_B:
	case EStateType::Roll_L:
	case EStateType::Roll_R:
	case EStateType::Roll_FR:
	case EStateType::Roll_FL:
	case EStateType::Roll_BR:
	case EStateType::Roll_BL:
		character->End_Roll();
		break;
	case EStateType::Hitted:
		character->End_Hitted();
		break;
	case EStateType::Dead:
		character->End_Dead();
		break;
	default:
		break;
	}
}
```




# **Troubleshooting**

### 1) 🎯 

### 2) 🎯 

### 3) 🎯 

# **Retrospective (느낀점)**

# 게임 플레이 영상
<p align="center">
  <a href="https://www.youtube.com/watch?v=sI_5kmsh7MY">
    <img src="image.png" width="1000">
  </a>
</p>

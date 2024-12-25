// YunMinSeong. All right reserved. 


#include "Characters/BladeEnemyCharacterBase.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/BoxComponent.h"
//Combat
#include "Components/Combat/EnemyCombatComponent.h"
#include "Items/Weapons/WeaponBase.h"
//AsyncLoad
#include "Engine/AssetManager.h"
//Niagara
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
//UI
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/ProgressBar/BladeProgressBarWidget.h"

#include "BladeFunctionLibrary.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"
#include "BladeGameplayTags.h"
#include "GameModes/BladeGameModeBase.h"
//Debug
#include "CustomDebugHelper.h"


ABladeEnemyCharacterBase::ABladeEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	//컨트롤러의 회전 방향을 따라 회전할지 결정
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//캐릭터가 이동하는 방향으로 자동으로 회전할지 결정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);
	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
}

void ABladeEnemyCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//Debug::Print("PossessedBy");
	InitEnemyStartUpdate();
}
UBaseCombatComponent* ABladeEnemyCharacterBase::GetCombatComponent() const
{
	Super::GetCombatComponent();
	return EnemyCombatComponent;
}
void ABladeEnemyCharacterBase::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor)) {
		if (UBladeFunctionLibrary::IsTargetPawnHostile(this,HitPawn)) {
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void ABladeEnemyCharacterBase::InitEnemyStartUpdate()
{
	if (CharacterStartUpData.IsNull()) return;
	int32 AbilityLevel = 1;
	if (ABladeGameModeBase* BladeGameMode = Cast<ABladeGameModeBase>(GetWorld()->GetAuthGameMode())) {
		switch (BladeGameMode->GetCurrentGameDifficulty())
		{
		case EBladeGameDifficulty::Easy:
			AbilityLevel = 1;
			break;
		case EBladeGameDifficulty::Normal:
			AbilityLevel = 2;
			break;
		case EBladeGameDifficulty::Hard:
			AbilityLevel = 3;
			break;
		case EBladeGameDifficulty::VeryHard:
			AbilityLevel = 4;
			break;
		default:
			break;
		}
	}
	//비동기 로딩.
	UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterStartUpData.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
		[this, AbilityLevel]() {
			if (UDataAsset_StartUpDataBase* LoadData = CharacterStartUpData.Get()) {
				
				LoadData->GiveToAbilitySystemComponent(BladeAbilitySystemComponent, AbilityLevel);
				//Debug::Print(TEXT("Spawn Enemy Weapon"), FColor::Green);
			}
		}
	));
}

void ABladeEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetDissolveTimeline();
	//Debug::Print("BeginPlay");
	if (UBladeWidgetBase* HealthWidget = Cast<UBladeWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject())) {
		HealthWidget->InitEnemyCreatedWidget(this);
	}
	StartDissolveTimeline.SetPlayRate(0.2);
	StartDissolveTimeline.ReverseFromEnd();

	if (!EntryMontagesToPlay.IsEmpty()) {
		int32 SelectEntryAnim= FMath::RandRange(0, EntryMontagesToPlay.Num()-1);
		PlayAnimMontage(EntryMontagesToPlay[SelectEntryAnim]);
	}
}
void ABladeEnemyCharacterBase::Tick(float Sec)
{
	Super::Tick(Sec);
	DissolveTimeline.TickTimeline(Sec);
	StartDissolveTimeline.TickTimeline(Sec);
}
#pragma region EnemyDie | Dissolve
void ABladeEnemyCharacterBase::SetDissolveTimeline()
{
	if (DissolveCurve) {
		FOnTimelineFloat DissolveCallBack;
		FOnTimelineEvent DissolveTimelineFinishCallback;
		DissolveTimelineFinishCallback.BindUFunction(this, "DissolveEndFunc");
		DissolveCallBack.BindUFunction(this, "DissolveFunc");
		DissolveTimeline.AddInterpFloat(DissolveCurve, DissolveCallBack);
		DissolveTimeline.SetTimelineFinishedFunc(DissolveTimelineFinishCallback);
		DissolveTimeline.SetPlayRate(0.3f);

		FOnTimelineFloat StartDissolveCallBack;
		StartDissolveCallBack.BindUFunction(this, "DissolveFunc");
		StartDissolveTimeline.AddInterpFloat(DissolveCurve, StartDissolveCallBack);
		StartDissolveTimeline.SetPlayRate(0.3f);
	}
	else {
		Debug::Print("Fill Dissolve Curve", FColor::Red);
	}
}
UPawnUIComponent* ABladeEnemyCharacterBase::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}
UEnemyUIComponent* ABladeEnemyCharacterBase::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}
void ABladeEnemyCharacterBase::OnEnemyDied()
{
	EnemyUIComponent->RemoveEnemyWidgetIfAny();
	GetMesh()->bPauseAnims = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(DissolveNiagara.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this]() {
		if (!DissolveNiagara.IsValid()) {
			return;
		}
			//비동기 로딩 완료 후에 Get()을 호출하여 로드된 데이터를 가져옴
			UNiagaraSystem* LoadData = Cast<UNiagaraSystem>(DissolveNiagara.Get());
			FLinearColor FirstColor;
			//UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
			if (IsValid(GetMesh())) {
				UMaterialInterface* MaterialInterface = GetMesh()->GetMaterial(0);
				if (MaterialInterface&& MaterialInterface->GetVectorParameterValue(FHashedMaterialParameterInfo("DissolveEdgeColor"), FirstColor)) {
					if (LoadData && IsValid(GetMesh())) {
						UNiagaraComponent* NiaComp = UNiagaraFunctionLibrary::SpawnSystemAttached(LoadData, GetMesh(), FName(""), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
						NiaComp->SetNiagaraVariableLinearColor(TEXT("DissolveParticleColor"), FirstColor);

					}
				}
			}
	}));
	DissolveTimeline.PlayFromStart();
}
void ABladeEnemyCharacterBase::DissolveFunc(float value)
{//Dissolve progress
	GetMesh()->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), value);
	if (bHasChildMesh) {
		TArray<USceneComponent*> ChildrenComponents;
		GetMesh()->GetChildrenComponents(false, ChildrenComponents);
		for (USceneComponent* Child : ChildrenComponents) {
			USkeletalMeshComponent* ChildStaticMesh = Cast<USkeletalMeshComponent>(Child);
			if (!ChildStaticMesh) continue;
			ChildStaticMesh->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), value);
		}
	}
	if (AWeaponBase* WeaponRef=EnemyCombatComponent->GetCharacterCurrentEquippedWeapon()) {
		WeaponRef->GetWeaponWeaponMesh()->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), value);
	}
	/*
	if (bHasWeaponMesh) {
		if (WeaponRef) {
			WeaponRef->GetWeaponWeaponMesh()->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), value);
		}
	}*/
}
void ABladeEnemyCharacterBase::DissolveEndFunc()
{//this Func is called when DissolveTimeline Ends.
	BladeAbilitySystemComponent->TryActivateAbilityByTag(BladeGameplayTags::Enemy_Ability_SpawnPotion);//전리품 생성.
	FTimerHandle DestroyDelay;
	GetWorld()->GetTimerManager().SetTimer(DestroyDelay, FTimerDelegate::CreateLambda([this]() {
		Destroy(); 
		if (AWeaponBase* WeaponRef = EnemyCombatComponent->GetCharacterCurrentEquippedWeapon()) {
			WeaponRef->DestroyWeapon();
		}
		//if (WeaponRef) {
		//	WeaponRef->DestroyWeapon();
		//}
	}), 1, false);
}
#pragma endregion




#if WITH_EDITOR
void ABladeEnemyCharacterBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandBoneName)) {
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandBoneName);
	}
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandBoneName)) {
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandBoneName);
	}
}
#endif

/*
UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterStartUpData.ToSoftObjectPath(),[this](){
			if(UDataAsset_StartUpDataBase* LoadData= CharacterStartUpData.Get()){
				LoadData->GiveToAbilitySystemComponent(BladeAbilitySystemComponent);
				Debug::Print(TEXT("Spawn Enemy Weapon"), FColor::Green);
			}
		}
	);
*/
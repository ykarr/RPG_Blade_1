// YunMinSeong. All right reserved. 


#include "Characters/BladePlayerCharacter.h"
//CharacterMovement
#include "GameFramework/CharacterMovementComponent.h"
//input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Input/BladeInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
//tag
#include "BladeGameplayTags.h"
//Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//Anim
#include "AnimInstances/Player/BladePlayerAnimInstance.h"
//AbilitySystem.
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
//combat
#include "Components/Combat/PlayerCombatComponent.h"
//UI
#include "Components/UI/PlayerUIComponent.h"
#include "Widgets/Overlay/PauseOverlayWidget.h"

#include "Engine\AssetManager.h"
#include "BladeFunctionLibrary.h"
#include "GameModes/BladeGameModeBase.h"
//Debug
#include "CustomDebugHelper.h"
ABladePlayerCharacter::ABladePlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; //¸¶Âû·Â
	
	bUseControllerRotationPitch = false;//²ô´ö²ô´ö
	bUseControllerRotationRoll = false;//°¼¿ô°¼¿ô
	bUseControllerRotationYaw = false;//µµ¸®µµ¸®

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 65.f);
	//CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bDoCollisionTest = false;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
	static ConstructorHelpers::FClassFinder<UBladePlayerAnimInstance> FindAnimBP(TEXT("AnimBlueprint'/Game/Blueprints/Characters/AnimBP/ABP_Player.ABP_Player_C'"));
	if (FindAnimBP.Succeeded()) {
		//GetMesh()->SetAnimInstanceClass(FindAnimBP.Class);
		GetMesh()->SetAnimClass(FindAnimBP.Class);
	}
}

void ABladePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Debug::Print(TEXT("hero"));
}

void ABladePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(), EBladeInputMode::GameOnly);
	if (!CharacterStartUpData.IsNull()) {
		//µ¿±â(Synchronous) loading ½ÃÀÛ. 
		if (UDataAsset_StartUpDataBase* LoadData = CharacterStartUpData.LoadSynchronous()) {
			int32 AbilityApplyLevel = 1;
			if (ABladeGameModeBase* BladeGameMode = Cast<ABladeGameModeBase>(GetWorld()->GetAuthGameMode())) {
				switch (BladeGameMode->GetCurrentGameDifficulty())
				{
				case EBladeGameDifficulty::Easy:
					AbilityApplyLevel = 1;
					break;
				case EBladeGameDifficulty::Normal:
					AbilityApplyLevel = 2;
					break;
				case EBladeGameDifficulty::Hard:
					AbilityApplyLevel = 3;
					break;
				case EBladeGameDifficulty::VeryHard:
					AbilityApplyLevel = 4;
					break;
				default:
					break;
				}
			}
			//ÀåÂø.
			LoadData->GiveToAbilitySystemComponent(BladeAbilitySystemComponent, AbilityApplyLevel);
		}
	}
	if (!BladeAbilitySystemComponent || !BladeAttributeSet) {
		Debug::Print(TEXT("AbilitySystemComponent or BladeAttributeSet is Not Valid"));
	}
}



UBaseCombatComponent* ABladePlayerCharacter::GetCombatComponent() const
{
	Super::GetCombatComponent();
	return PlayerCombatComponent;
}

UPawnUIComponent* ABladePlayerCharacter::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* ABladePlayerCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

void ABladePlayerCharacter::Input_Pause()
{
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftPauseOverlayClass.ToSoftObjectPath(),FStreamableDelegate::CreateLambda([&](){
		if (SoftPauseOverlayClass.Get()) {
			//UPauseOverlayWidget¿¡¼­ »ç¿ëÇÏ°í RemoveFromParent»ç¿ë. pauseÀÚÃ¼´Â ¸¹ÀÌ »ç¿ë¾ÈÇÒµíÇØ¼­.CreateWidgetÀ¸·Î »ý¼ºÇÏ´Â°É·Î.
			CreateWidget<UPauseOverlayWidget>(GetWorld(), SoftPauseOverlayClass.Get())->AddToViewport(2);
		}
	}));
}

void ABladePlayerCharacter::Input_Move(const FInputActionValue& value)
{
	const FVector2D MoveVec = value.Get<FVector2D>();
	const FRotator MoveRotVec(0.0f, GetControlRotation().Yaw, 0.0f);
	if (MoveVec.Y != 0) {
		//Forward
		const FVector ForwardDirection = MoveRotVec.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MoveVec.Y);
	}
	if (MoveVec.X!=0) {
		//Right
		const FVector RightDirection = MoveRotVec.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MoveVec.X);
	}
	
}
void ABladePlayerCharacter::Input_Look(const FInputActionValue& value)
{
	const FVector2D LookVec = value.Get<FVector2D>();
	if (LookVec.X!=0) {
		AddControllerYawInput(LookVec.X);
	}
	if (LookVec.Y != 0) {
		AddControllerPitchInput(LookVec.Y);
	}
}
void ABladePlayerCharacter::Input_SwitchTargetTriggered(const FInputActionValue& value)
{
	SwitchDirection=value.Get<FVector2D>();
}
void ABladePlayerCharacter::Input_SwitchTargetCompleted(const FInputActionValue& value)
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X>0.f?BladeGameplayTags::Player_Event_SwitchTarget_Right:BladeGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);

}
void ABladePlayerCharacter::Input_PickUpPotionStarted(const FInputActionValue& value)
{
	FGameplayEventData Data;
	//Data.Instigator = this;
	//Data.Target = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,BladeGameplayTags::Player_Event_UsePotions, Data);
}
void ABladePlayerCharacter::Jump()
{
	Super::Jump();
}
void ABladePlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	BladeAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}
void ABladePlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	BladeAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void ABladePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetController<APlayerController>()->GetLocalPlayer());
	check(subsystem);
	subsystem->AddMappingContext(DataAsset_InputConfig->DefaultMappingContext, 0);

	UBladeInputComponent* BladeInputComponent = CastChecked<UBladeInputComponent>(PlayerInputComponent);
	BladeInputComponent->BindNativeInputAction(DataAsset_InputConfig, BladeGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	BladeInputComponent->BindNativeInputAction(DataAsset_InputConfig, BladeGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	BladeInputComponent->BindNativeInputAction(DataAsset_InputConfig, BladeGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);

	BladeInputComponent->BindNativeInputAction(DataAsset_InputConfig, BladeGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	BladeInputComponent->BindNativeInputAction(DataAsset_InputConfig, BladeGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);
	BladeInputComponent->BindNativeInputAction(DataAsset_InputConfig, BladeGameplayTags::InputTag_PickUp_Potions, ETriggerEvent::Started, this, &ThisClass::Input_PickUpPotionStarted);

	BladeInputComponent->BindAbilityInputAction(DataAsset_InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}
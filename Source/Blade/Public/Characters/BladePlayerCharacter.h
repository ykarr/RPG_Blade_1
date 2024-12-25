// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Characters/BladeCharacterBase.h"
#include "GameplayTagContainer.h"
#include "BladePlayerCharacter.generated.h"

struct FInputActionValue;
class UDataAsset_InputConfig;
class UCameraComponent;
class USpringArmComponent;
class UPlayerCombatComponent;
class UPlayerUIComponent;
class UPauseOverlayWidget;
/**
 * 
 */
UCLASS()
class BLADE_API ABladePlayerCharacter : public ABladeCharacterBase
{
	GENERATED_BODY()
public:
	// ICombatComponentInterface을(를) 통해 상속됨
	virtual UBaseCombatComponent* GetCombatComponent()  const override;
	// IPawnUIInterface을(를) 통해 상속됨
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent()const override;
protected:
	ABladePlayerCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
#pragma region Components
public:
protected:
private:
	UPROPERTY(EditDefaultsOnly,Category="Camera",meta=(AllowPrivateAccess=true))
	UCameraComponent* FollowCamera;
	UPROPERTY(EditDefaultsOnly,Category="Camera",meta=(AllowPrivateAccess=true))
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly,blueprintReadOnly,Category="Combat",meta=(AllowPrivateAccess=true))
	UPlayerCombatComponent* PlayerCombatComponent;
	UPROPERTY(EditDefaultsOnly,blueprintReadOnly,Category="UI",meta=(AllowPrivateAccess=true))
	UPlayerUIComponent* PlayerUIComponent;
#pragma endregion
#pragma region inputs
public:
protected:
	UFUNCTION(BlueprintCallable)
	void Input_Pause();
private:

	/*Input FUNC*/
	void Input_Move(const FInputActionValue& value);
	void Input_Look(const FInputActionValue& value);
	void Input_SwitchTargetTriggered(const FInputActionValue& value);
	void Input_SwitchTargetCompleted(const FInputActionValue& value);
	void Input_PickUpPotionStarted(const FInputActionValue& value);
	virtual void Jump() override;

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

	UPROPERTY(EditdefaultsOnly, Category = "InputData")
	TSoftClassPtr<UPauseOverlayWidget> SoftPauseOverlayClass;
	UPROPERTY(EditdefaultsOnly, Category = "InputData")
	UPauseOverlayWidget* PauseOverlay;
	UPROPERTY(EditdefaultsOnly, Category = "InputData")
	UDataAsset_InputConfig* DataAsset_InputConfig;
	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;
#pragma endregion
public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }
	FORCEINLINE UCameraComponent* GeCameraComponent() const { return FollowCamera; }
};

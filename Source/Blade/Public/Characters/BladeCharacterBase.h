// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatComponentInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "BladeCharacterBase.generated.h"

class UBladeAbilitySystemComponent;
class UBladeAttributeSet;
class UDataAsset_StartUpDataBase;
class UNiagaraSystem;
class UMotionWarpingComponent;
UCLASS()
class BLADE_API ABladeCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatComponentInterface, public IPawnUIInterface
{
	GENERATED_BODY()
public:
	// IAbilitySystemInterface을(를) 통해 상속됨
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ICombatComponentInterface을(를) 통해 상속됨
	virtual UBaseCombatComponent* GetCombatComponent()  const override;
	// IPawnUIInterface을(를) 통해 상속됨
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
protected:
	// Sets default values for this character's properties
	ABladeCharacterBase();
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	UBladeAbilitySystemComponent* BladeAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	UBladeAttributeSet* BladeAttributeSet;
	//메모리 상에 객체가 존재하지 않을 수 있기 때문에 메모리 사용을 최적화하는데 좋음. 객체가 사용될 때까지 메모리에 올리지 않아도 됨.
	//Synchronous loading과 Asynchronous loading이 있으며 Synchronous(동기식)은 로딩이 완료될때까지 게임이 일시 중지됨.
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterData")
	TSoftObjectPtr<UNiagaraSystem> DissolveNiagara;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;
public:
	FORCEINLINE UBladeAbilitySystemComponent* GetBladeAbilitySystemComponent() const {return BladeAbilitySystemComponent;}
	FORCEINLINE UBladeAttributeSet* GetBladeAttributeSet() const { return BladeAttributeSet; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return  MotionWarpingComponent; }



	


	

};
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
	// IAbilitySystemInterface��(��) ���� ��ӵ�
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ICombatComponentInterface��(��) ���� ��ӵ�
	virtual UBaseCombatComponent* GetCombatComponent()  const override;
	// IPawnUIInterface��(��) ���� ��ӵ�
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
protected:
	// Sets default values for this character's properties
	ABladeCharacterBase();
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	UBladeAbilitySystemComponent* BladeAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	UBladeAttributeSet* BladeAttributeSet;
	//�޸� �� ��ü�� �������� ���� �� �ֱ� ������ �޸� ����� ����ȭ�ϴµ� ����. ��ü�� ���� ������ �޸𸮿� �ø��� �ʾƵ� ��.
	//Synchronous loading�� Asynchronous loading�� ������ Synchronous(�����)�� �ε��� �Ϸ�ɶ����� ������ �Ͻ� ������.
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
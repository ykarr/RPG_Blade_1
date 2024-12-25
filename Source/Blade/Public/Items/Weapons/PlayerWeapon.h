// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WeaponBase.h"
#include "BladeTypes/BladeStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "PlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API APlayerWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, blueprintReadonly,Category="WeaponData")
	FBladePlayerWeaponData PlayerWeaponData;

	UFUNCTION(BlueprintCallable) //GrantedAbilitySpecHandles의 Setter
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(Blueprintpure)//GrantedAbilitySpecHandles의 Getter
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
private:
	//능력을 Equip에서 저장 UnEquip에서 삭제?
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};

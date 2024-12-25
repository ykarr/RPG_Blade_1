// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtentionComponent.h"
#include "GameplayTagContainer.h"
#include "BaseCombatComponent.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8 {
	CurrentWeapon,
	RightHand,
	LeftHand
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADE_API UBaseCombatComponent : public UPawnExtentionComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(Blueprintcallable, Category = "Blade|Combat")
	void RegisterSpawnWeapon(FGameplayTag InWeaponTagToRegister, AWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(Blueprintcallable, Category = "Blade|Combat")
	AWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet)const;

	UFUNCTION(BlueprintCallable, Category = "Blade|Combat")
	AWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UPROPERTY(BlueprintReadWrite, Category = "Blade|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category="Blade|Combat")
	void ToggleWeaponCollisionEnable(bool bShouldEnable, EToggleDamageType ToggleDamageType= EToggleDamageType::CurrentWeapon);
	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleCurrentBodyCollision(bool bShouldEnable, EToggleDamageType ToogleDamageType);
	TArray<AActor*> OverlappedActors;
private:
	TMap<FGameplayTag, AWeaponBase*> CharacterCarriedWeaponMap;
		
};

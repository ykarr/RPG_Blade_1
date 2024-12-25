// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Items/BladePickupBase.h"
#include "PickupPotionBase.generated.h"

class UBladeAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BLADE_API APickupPotionBase : public ABladePickupBase
{
	GENERATED_BODY()
public:
	void Consume(UBladeAbilitySystemComponent* ASC, int32 ApplyLevel);
protected:
	UFUNCTION(BlueprintImplementableEvent,meta=(DisPlayName="On Potion Consumed"))
	void BP_OnPotionConsumed();
	virtual void OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PotionEffectClass;
};

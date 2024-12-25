// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/BaseCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UEnemyCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;
protected:
	
	virtual void ToggleCurrentBodyCollision(bool bShouldEnable, EToggleDamageType ToogleDamageType)override;
};

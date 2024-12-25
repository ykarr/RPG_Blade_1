// YunMinSeong. All right reserved. 


#include "Characters/BladeCharacterBase.h"
#include "AbilitySystem/BladeAbilitySystemComponent.h"
#include "AbilitySystem/BladeAttributeSet.h"
#include "MotionWarpingComponent.h"
// Sets default values
ABladeCharacterBase::ABladeCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BladeAbilitySystemComponent = CreateDefaultSubobject<UBladeAbilitySystemComponent>(TEXT("BladeAbilitySystemComponent"));
	BladeAttributeSet= CreateDefaultSubobject<UBladeAttributeSet>(TEXT("BladeAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void ABladeCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (BladeAbilitySystemComponent) {
		BladeAbilitySystemComponent->InitAbilityActorInfo(this, this);
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("forgot to assign start up data to %s"), *GetName());
	}
}

UPawnUIComponent* ABladeCharacterBase::GetPawnUIComponent()const
{
	return nullptr;
}

UBaseCombatComponent* ABladeCharacterBase::GetCombatComponent()  const
{
	return nullptr;
}

UAbilitySystemComponent* ABladeCharacterBase::GetAbilitySystemComponent() const
{
	return  GetBladeAbilitySystemComponent();
}

// YunMinSeong. All right reserved. 


#include "BladeTypes/BladeStructTypes.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"


bool FBladePlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
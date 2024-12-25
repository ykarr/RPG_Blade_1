// YunMinSeong. All right reserved. 


#include "Items/Weapons/PlayerWeapon.h"
#include "CustomDebugHelper.h"
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	//Debug::Print("SpawnWeapon");
}

void APlayerWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> APlayerWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}

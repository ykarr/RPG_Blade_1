// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/PlayerUnequipAbility.h"
//Character
#include "Characters/BladePlayerCharacter.h"
//Components
#include "Components/UI/PlayerUIComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
//Weapon
#include "Items\Weapons\WeaponBase.h"
#include "Items\Weapons\PlayerWeapon.h"
//input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//AbilitySystem
#include "AbilitySystem\BladeAbilitySystemComponent.h"
void UPlayerUnequipAbility::DetachWeaponToPlayerHand()
{
	AWeaponBase& WeaponBase = *GetPlayerCombatComponentFromActorInfo()->GetCharacterCarriedWeaponByTag(InWeaponTag);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	WeaponBase.AttachToComponent(GetOwningComponentFromActorInfo(), AttachmentRules, SocketName);//SwordRightHandSocket
	UnLinkWeaponData();
	GetPlayerCombatComponentFromActorInfo()->CurrentEquippedWeaponTag = FGameplayTag();
}

void UPlayerUnequipAbility::UnLinkWeaponData()
{
	//GetOwningComponentFromActorInfo()->LinkAnimClassLayers(*WeaponData.WeaponAnimLayerToLink);
	APlayerWeapon& WeaponBase = *GetPlayerCombatComponentFromActorInfo()->GetPlayerCarriedWeaponByTag(InWeaponTag);
	FBladePlayerWeaponData& WeaponData = WeaponBase.PlayerWeaponData;

	GetOwningComponentFromActorInfo()->UnlinkAnimClassLayers(*WeaponData.WeaponAnimLayerToLink);
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetPlayerControllerFromActorInfo())->GetLocalPlayer());
	check(subsystem);
	subsystem->RemoveMappingContext(WeaponData.WeaponInputMappingContext);
	
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles = WeaponBase.GetGrantedAbilitySpecHandles();
	GetBladeAbilitySystemComponentFromActoInfo()->RemoveGrantPlayerWeaponAbilities(GrantedAbilityHandles);

	GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnEquippedWeaponChanged.Broadcast(nullptr);
	for (const FBladePlayerSpecialAbilitySet& BladePlayerSpecialAbilitySet :WeaponData.SpecialWeaponAbilities) {
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnAbilityIconSlotUpdate.Broadcast(BladePlayerSpecialAbilitySet.InputTag, nullptr);
	}
	//GetPlayerCharacterFromActorInfo()->bUseControllerRotationYaw = false;
}

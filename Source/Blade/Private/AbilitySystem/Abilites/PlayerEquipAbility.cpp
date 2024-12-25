// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/PlayerEquipAbility.h"
#include "Items\Weapons\WeaponBase.h"
#include "Items/Weapons/PlayerWeapon.h"
#include "GameFramework\CharacterMovementComponent.h"
//Character
#include "Characters/BladePlayerCharacter.h"
//Components
#include "Components/UI/PlayerUIComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
//controller
#include "Controllers\BladeController.h"
//input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//AbilitySystemComponent
#include "AbilitySystem\BladeAbilitySystemComponent.h"

#include "AnimInstances/Player/BladePlayerLinkedAnimLayer.h"
void UPlayerEquipAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UPlayerEquipAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo,ActivationInfo,bReplicateEndAbility,bWasCancelled);
}



void UPlayerEquipAbility::AttachWeaponToPlayer()
{
	checkf(InWeaponTag.IsValid(), TEXT("Fill WeaponTag"));
	checkf(SocketName.IsValid(), TEXT("Fill SocketName"));
	AWeaponBase& WeaponBase=*GetPlayerCombatComponentFromActorInfo()->GetCharacterCarriedWeaponByTag(InWeaponTag);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	WeaponBase.AttachToComponent(GetOwningComponentFromActorInfo(), AttachmentRules,SocketName);//SwordRightHandSocket

	//무기를 장착한경우 이동속도 250.
	/*APawn& OwningPlayerPawn = *Cast<APawn>(GetAvatarActorFromActorInfo());
	UCharacterMovementComponent& MovementComponent =* Cast<UCharacterMovementComponent>(OwningPlayerPawn.GetMovementComponent());
	MovementComponent.MaxWalkSpeed = 250.f;*/

	//플레이어의 무기 데이터 가져오기.
	//FBladePlayerWeaponData& WeaponData = GetPlayerCombatComponentFromActorInfo()->GetPlayerCarriedWeaponByTag(InWeaponTag)->PlayerWeaponData;
	
	LinkWeaponData();
	//Tag설정
	GetPlayerCombatComponentFromActorInfo()->CurrentEquippedWeaponTag = InWeaponTag;
	
}

void UPlayerEquipAbility::LinkWeaponData()
{
	APlayerWeapon& WeaponBase= *GetPlayerCombatComponentFromActorInfo()->GetPlayerCarriedWeaponByTag(InWeaponTag);
	FBladePlayerWeaponData& WeaponData= WeaponBase.PlayerWeaponData;
	//무기데이터에서 AnimlayerToLink를 가져와서 설정. 애니메이션이 바뀜.
	GetOwningComponentFromActorInfo()->LinkAnimClassLayers(*WeaponData.WeaponAnimLayerToLink);

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetPlayerControllerFromActorInfo())->GetLocalPlayer());
	check(subsystem);
	subsystem->AddMappingContext(WeaponData.WeaponInputMappingContext,3);

	
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
	GetBladeAbilitySystemComponentFromActoInfo()->GrantPlayerWeaponAbilities(WeaponData.DefaultWeaponAbilities,WeaponData.SpecialWeaponAbilities, GetAbilityLevel(), GrantedAbilityHandles);
	WeaponBase.AssignGrantedAbilitySpecHandles(GrantedAbilityHandles);

	GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnEquippedWeaponChanged.Broadcast(WeaponBase.PlayerWeaponData.SoftWeaponIconTexture);
	for (const FBladePlayerSpecialAbilitySet& BladePlayerSpecialAbilitySet :WeaponData.SpecialWeaponAbilities) {
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnAbilityIconSlotUpdate.Broadcast(BladePlayerSpecialAbilitySet.InputTag, BladePlayerSpecialAbilitySet.AbilityIconMaterial);
		float TotalCooldownTime=0;
		float RemainingCooldownTime=0;
		if (GetAbilityRemainingCooldownByTag(BladePlayerSpecialAbilitySet.AbilityCoolDownTag, TotalCooldownTime, RemainingCooldownTime)) {
			GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnAbilityCoolDownBegin.Broadcast(BladePlayerSpecialAbilitySet.InputTag,TotalCooldownTime, RemainingCooldownTime);
		}
		
	}
	//마우스 보는 방향으로 캐릭터 움직임.
	//GetPlayerCharacterFromActorInfo()->bUseControllerRotationYaw = true;
	//GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->bOrientRotationToMovement = false;
}

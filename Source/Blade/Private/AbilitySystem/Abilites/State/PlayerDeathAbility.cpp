// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/State/PlayerDeathAbility.h"
#include "Characters\BladePlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Controllers/BladeController.h"
#include "GameModes/BladeSurvialGameMode.h"
void UPlayerDeathAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerDeathAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetOwningComponentFromActorInfo()->bPauseAnims = true;
	GetPlayerCharacterFromActorInfo()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	if (ABladeSurvialGameMode* GameMode = Cast<ABladeSurvialGameMode>(GetWorld()->GetAuthGameMode())) {
		GameMode->SetGameModeState(EBladeSurvialGameModeState::PlayerDied);
	}
	FInputModeUIOnly InputMode;
	if (GetPlayerControllerFromActorInfo()) {
		GetPlayerControllerFromActorInfo()->SetInputMode(InputMode);
		GetPlayerControllerFromActorInfo()->SetShowMouseCursor(true);
	}
}

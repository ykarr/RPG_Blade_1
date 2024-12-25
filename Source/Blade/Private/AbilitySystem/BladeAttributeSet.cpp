// YunMinSeong. All right reserved. 


#include "AbilitySystem/BladeAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"

#include "BladeFunctionLibrary.h"
#include "BladeGameplayTags.h"
//UI Interface
#include "Interfaces/PawnUIInterface.h"
//UI Components
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"
//Debug
#include "CustomDebugHelper.h"
UBladeAttributeSet::UBladeAttributeSet()
{
    InitMaxHealth(1.f);
    InitCurrentHealth(1.f);
    InitMaxRage(1.f);
    InitCurrentRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
}

void UBladeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
   // CachedPawnUIInterface
    
    if (!CachedPawnUIInterface.IsValid()) {
        CachedPawnUIInterface =TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
    }
    checkf(CachedPawnUIInterface.IsValid(), TEXT("IPawnUIInterface Error"));
    UPawnUIComponent* PawnUIComponent= CachedPawnUIInterface->GetPawnUIComponent();
    checkf(PawnUIComponent, TEXT("PawnUIComponent Error"), CachedPawnUIInterface);
    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
        const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
        SetCurrentHealth(NewCurrentHealth);
        //HP UI.
        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
    }
    if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute()) {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
        SetCurrentRage(NewCurrentRage);
        if (GetCurrentRage() == GetMaxRage()) {
            UBladeFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BladeGameplayTags::Player_State_Rage_Full);
        }
        else if (GetCurrentRage()==0.f) {
            UBladeFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BladeGameplayTags::Player_State_Rage_None);
        }
        else {
            UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), BladeGameplayTags::Player_State_Rage_Full);
            UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), BladeGameplayTags::Player_State_Rage_None);

        }
        if (UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent()) {
            PlayerUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage()/GetMaxRage());
        }
    }
    if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute()) {
        const float OldHealth = GetCurrentHealth();
        const float DamageDone = GetDamageTaken();

        const float NewCurrentHealth = FMath::Clamp(OldHealth-DamageDone, 0.f, GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);
        const FString DebugString = FString::Printf(TEXT("Old Health: %f, Damage: %f, New CurrentHealth: %f"),
            OldHealth, DamageDone, NewCurrentHealth);
        //Debug::Print(DebugString,FColor::Green);
        // Notify the UI
        //HP UI.
        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
        if (GetCurrentHealth()<=0.f) {
            Debug::Print("Die");
            //블루프린트 라이브러리로 만든 함수. 동일한 태그를 추가하지 않도록 설정되어있음.
            UBladeFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),BladeGameplayTags::Shared_Status_Dead);
        }
    }

   
}

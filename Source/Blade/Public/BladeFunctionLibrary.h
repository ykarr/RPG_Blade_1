// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "BladeTypes\BladeEnumTypes.h"
#include "BladeFunctionLibrary.generated.h"

class UBladeAbilitySystemComponent;
class UBaseCombatComponent;
class UBladeGameInstance;
struct FScalableFloat;
/**
 * BlueprintFunctionLibrary에는 3가지 유형의 함수가 있음.
 * 1. CPP에서만 사용할 수 있는 유형.
 * 2. 블루프린트에서만 사용할 수 있는 유형.
 * 3. 두가지 모두에 사용할 수 있는 유형.
 */
UCLASS()
class BLADE_API UBladeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UBladeAbilitySystemComponent* NativeGetBladeASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category="Blade|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable,Category="Blade|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);
	
	UFUNCTION(Blueprintpure,Category="Blade|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);


	//블루프린트 전용.
	UFUNCTION(Blueprintcallable, category = "Slash|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EBladeConfirmType& OutConfirmType);

	static UBaseCombatComponent* NativeGetCombatComponent(AActor* InActor);
	UFUNCTION(Blueprintcallable, category = "Slash|FunctionLibrary", meta = (DisplayName = "GetCombatComponent", ExpandEnumAsExecs = "OutValidType"))
	static UBaseCombatComponent* BP_GetCombatComponent(AActor* InActor, EBladehValidType& OutValidType);

	UFUNCTION(Blueprintcallable, category = "Slash|FunctionLibrary", meta = (CompactNodeTitle="Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat,float InLevel=1.f);

	
	UFUNCTION(BlueprintPure, category = "Slash|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintPure, category = "Slash|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintPure, category = "Slash|FunctionLibrary")
	static bool ApplyGameEffectSpecHandleToTargetActor(AActor* InInstigator,AActor* InTargetActor, const FGameplayEffectSpecHandle& InEffectSpecHandle);

	UFUNCTION(BlueprintCallable, category = "Slash|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0f", UpdateInterval = "0.1"))
	static void CoolDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EBladeCountDownActionInput CountDownInput,UPARAM(DisplayName="Output") EBladeCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);
	
	UFUNCTION(BlueprintPure, category = "Slash|FunctionLibrary",meta=(WorldContext="WorldContextObject"))
	static UBladeGameInstance* GetBladeGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, category = "Slash|FunctionLibrary",meta=(WorldContext="WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject,EBladeInputMode InInputMode);
	UFUNCTION(BlueprintCallable, category = "Slash|FunctionLibrary")
	static void SaveCurrentGameDifficulty(EBladeGameDifficulty InDifficulty);
	UFUNCTION(BlueprintCallable, category = "Slash|FunctionLibrary")
	static bool TryLoadSaveGameDifficulty(EBladeGameDifficulty& OutSaveDifficulty);
};
// #include "BladeFunctionLibrary.h"
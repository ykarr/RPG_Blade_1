// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "BladeInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBladeInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);
	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

template<class UserObject, typename CallbackFunc>
inline void UBladeInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input Config Data asset is Null"));
	if (UInputAction* FoundAction= InInputConfig->FindNativeInputActionByTag(InInputTag)) {
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template<class UserObject, typename CallbackFunc>
inline void UBladeInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input Config Data asset is Null"));
	for (const FBladeInputActionConfig& AbilityInputActionConfig: InInputConfig->AbilityInputActions) {
		if (!AbilityInputActionConfig.IsValid())continue;
		//매개변수로 태그 전달.
		BindAction(AbilityInputActionConfig.InputAction,ETriggerEvent::Started,ContextObject,InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction,ETriggerEvent::Completed,ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}

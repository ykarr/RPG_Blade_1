// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FBladeInputActionConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;
	
	bool IsValid() const {
		return InputTag.IsValid() && InputAction;
	}
};
/**
 * 
 */
UCLASS()
class BLADE_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FBladeInputActionConfig> NativeInputActions;//¿Ãµø.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FBladeInputActionConfig> AbilityInputActions;//¿Â¬¯.

	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InputTag)const;
};

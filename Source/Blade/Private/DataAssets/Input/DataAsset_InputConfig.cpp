// YunMinSeong. All right reserved. 


#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FBladeInputActionConfig& InputActionConfig :NativeInputActions) {
		if (InputActionConfig.InputTag == InputTag && InputActionConfig.InputAction) {
			return InputActionConfig.InputAction;
		}
	}
	return nullptr;
}

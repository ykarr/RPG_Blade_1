// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BladeCharacterAnimInstance.h"
#include "BladePlayerAnimInstance.generated.h"

class ABladePlayerCharacter;
/**
 * 
 */
UCLASS()
class BLADE_API UBladePlayerAnimInstance : public UBladeCharacterAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
	ABladePlayerCharacter* OwningPlayerCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;
	
	float IdleElapsedTime;
};

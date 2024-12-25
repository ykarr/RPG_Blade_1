// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BladeBaseAnimInstance.h"
#include "BladeCharacterAnimInstance.generated.h"

class ABladeCharacterBase;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class BLADE_API UBladeCharacterAnimInstance : public UBladeBaseAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY()
	ABladeCharacterBase* OwningCharacter;
	
	UPROPERTY()
	UCharacterMovementComponent* OwningCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	float GroundSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	bool bHasAcceleration;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	bool IsFalling;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	FVector PlayerVelocity;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	float LocomotionDirection;
private:
	bool CharacterValidCheck();
};

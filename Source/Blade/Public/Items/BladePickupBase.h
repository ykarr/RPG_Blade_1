// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BladePickupBase.generated.h"

class USphereComponent;
UCLASS()
class BLADE_API ABladePickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

protected:
	ABladePickupBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	USphereComponent* PickupSphereComponent;
	
	UFUNCTION()
	virtual void OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);
UCLASS()
class BLADE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	void DestroyWeapon();
	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
protected:
	AWeaponBase();
	UFUNCTION()
	virtual void WeaponCollisionBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void WeaponCollisionEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Weapons")
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Weapons")
	UBoxComponent* WeaponCollisionBox;
public:	
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox()const { return WeaponCollisionBox; }
	FORCEINLINE UStaticMeshComponent* GetWeaponWeaponMesh()const { return WeaponMesh; }
};

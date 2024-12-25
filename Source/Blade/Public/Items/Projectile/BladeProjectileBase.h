// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "BladeProjectileBase.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
struct FGameplayEventData;

UENUM(Blueprinttype)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

UCLASS()
class BLADE_API ABladeProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//void Homing();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileData")
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;
protected:
	ABladeProjectileBase();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category=" ProjectileData")
	UBoxComponent* ProjectileCollisionBox;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="ProjectileData")
	UNiagaraComponent* ProjectileNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "ProjectileData")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "ProjectileData")
	EProjectileDamagePolicy ProjectileDamagePolicy= EProjectileDamagePolicy::OnHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "ProjectileData")
	USoundBase* ProjectileImpactSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "ProjectileData")
	USoundBase* ProjectileSpawnSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "ProjectileData")
	USoundBase* ProjectileFlySound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "ProjectileData")
	UNiagaraSystem* ProjectileImpactNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "ProjectileData")
	UNiagaraSystem* ProjectileMuzzleFX;
	
	

	UFUNCTION()
	virtual void OnProjectileHit (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	UFUNCTION()
	virtual void OnProjectileBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSpawnProjectileHitFX(const FVector& HitLocation);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Spawn projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

private:
	void HandleApplyProjectileDamage(APawn* InHitPawn,const FGameplayEventData& InPayload);

	TArray<AActor*> OverlappedActors;
};

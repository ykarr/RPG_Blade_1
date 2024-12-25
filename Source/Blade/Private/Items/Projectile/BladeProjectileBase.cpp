// YunMinSeong. All right reserved. 


#include "Items/Projectile/BladeProjectileBase.h"
#include "Components\BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BladeFunctionLibrary.h"

#include "Kismet\GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
//Tags
#include "BladeGameplayTags.h"
//Debug
#include "CustomDebugHelper.h"
ABladeProjectileBase::ABladeProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this,&ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 700.f;
	ProjectileMovementComponent->MaxSpeed = 900.f;
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.f, 0.f);
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	//4초후 삭제됨.
	InitialLifeSpan = 4.f;
}

void ABladeProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if (ProjectileDamagePolicy==EProjectileDamagePolicy::OnBeginOverlap) {
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	}
	if (ProjectileSpawnSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSpawnSound, GetActorLocation());
	}
	if (ProjectileFlySound) {
		UGameplayStatics::SpawnSoundAttached(ProjectileFlySound, GetRootComponent(),FName(),FVector::ZeroVector,EAttachLocation::KeepRelativeOffset,true);
	}
	if (ProjectileMuzzleFX) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileMuzzleFX,GetActorLocation(), GetActorForwardVector().ToOrientationRotator());
	}
}

void ABladeProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnSpawnProjectileHitFX(Hit.ImpactPoint);
	APawn* HitPawn = Cast<APawn>(OtherActor);
	//Debug::Print(OtherActor->GetActorNameOrLabel());
	if (!HitPawn|| !UBladeFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn)) {
		//Pawn이 없거나 TargetPawn이 적대적이지 않은경우.
		//Debug::Print("Fail");
		Destroy();
		return;
	}
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UBladeFunctionLibrary::NativeDoesActorHaveTag(HitPawn,BladeGameplayTags::Player_State_Blocking);
	if (bIsPlayerBlocking) {
		bIsValidBlock=UBladeFunctionLibrary::IsValidBlock(this, HitPawn);
	}
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
	if (bIsValidBlock) {
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn,BladeGameplayTags::Player_Event_SuccessfullBlock, Data);
	}
	else {
		//막지 못한경우.
		//ApplyDamage
		HandleApplyProjectileDamage(HitPawn, Data);
	}
	Destroy();
}

void ABladeProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Debug::Print("OnProjectileBeginOverlap");
	if (OverlappedActors.Contains(OtherActor)) {
		return;
	}
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn || !UBladeFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn)) {
		//Pawn이 없거나 TargetPawn이 적대적이지 않은경우.
		return;
	}
	OverlappedActors.AddUnique(OtherActor);
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
	HandleApplyProjectileDamage(HitPawn, Data);
}

void ABladeProjectileBase::OnSpawnProjectileHitFX(const FVector& HitLocation)
{
	if (ProjectileImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileImpactSound, HitLocation);
	}
	if (ProjectileImpactNiagara) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileImpactNiagara, HitLocation);
	}
}

void ABladeProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	if (ProjectileDamageEffectSpecHandle.IsValid()&& GetInstigator()&& InHitPawn) {
		const bool bWasApplied= UBladeFunctionLibrary::ApplyGameEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);
		if (bWasApplied) {
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InHitPawn,BladeGameplayTags::Shared_Event_HitReact, InPayload);
		}
	}
}





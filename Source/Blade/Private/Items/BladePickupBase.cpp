// YunMinSeong. All right reserved. 


#include "Items/BladePickupBase.h"
#include "Components/SphereComponent.h"
// Sets default values
ABladePickupBase::ABladePickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PickupSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(PickupSphereComponent);
	PickupSphereComponent->SetSphereRadius(50.f);
	PickupSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnPickupCollisionSphereBeginOverlap);
}

void ABladePickupBase::OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}



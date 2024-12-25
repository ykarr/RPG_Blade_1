// YunMinSeong. All right reserved. 


#include "Items/Weapons/WeaponBase.h"
#include "Components/BoxComponent.h"

#include "BladeFunctionLibrary.h"
//Debug
#include "CustomDebugHelper.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::WeaponCollisionBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::WeaponCollisionEndOverlap);
}

void AWeaponBase::WeaponCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�𸮾� �������� � ����(Actor)�� �ٸ� ���Ϳ��� ������ ���ƴ���, �� �ൿ�� ��ü�� �˾Ƴ��� ���� �Լ�
	APawn* WeaponOwningPawn=GetInstigator<APawn>();
	//���� ���� ��ȿ���� �ʴٸ� �����Ҵ� �Ҵ��ϴ°��� ������.
	checkf(WeaponOwningPawn,TEXT("Forgot to assign an instiagtor as Owning Pawn of the Weapon : %s"),*GetName());
	if (APawn* HitPawn = Cast<APawn>(OtherActor)) {
		if (UBladeFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn)) {
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
	/*if (APawn* HitPawn = Cast<APawn>(OtherActor)) {
		if (WeaponOwningPawn != HitPawn) {
			//Debug::Print(GetName()+"Begin Overlap With "+ *HitPawn->GetName(),FColor::Green);
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}*/
}

void AWeaponBase::WeaponCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//�𸮾� �������� � ����(Actor)�� �ٸ� ���Ϳ��� ������ ���ƴ���, �� �ൿ�� ��ü�� �˾Ƴ��� ���� �Լ�
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	//���� ���� ��ȿ���� �ʴٸ� �����Ҵ� �Ҵ��ϴ°��� ������.
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instiagtor as Owning Pawn of the Weapon : %s"), *GetName());
	if (APawn* HitPawn = Cast<APawn>(OtherActor)) {
		if (UBladeFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn)) {
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
	/*if (APawn* HitPawn = Cast<APawn>(OtherActor)) {
		if (WeaponOwningPawn != HitPawn) {
			//Debug::Print(GetName() + "End Overlap With " + *HitPawn->GetName(), FColor::Red);
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}*/
}
void AWeaponBase::DestroyWeapon()
{
	Destroy();
}
// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/PlayerRollAbility.h"
#include "Characters/BladePlayerCharacter.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
void UPlayerRollAbility::ComputeRollDirectionAndDistance()
{
	FVector LastMoveInputVec=GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector();
	FVector NormalLastMoveInputVec = LastMoveInputVec.GetSafeNormal();

	
	UMotionWarpingComponent* MWC= GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent();
	MWC->AddOrUpdateWarpTargetFromLocationAndRotation(FName("RollingDirection"),FVector::ZeroVector, FRotationMatrix::MakeFromX(NormalLastMoveInputVec).Rotator());

	FVector Start = GetPlayerCharacterFromActorInfo()->GetActorLocation() + NormalLastMoveInputVec * RollingDistanceScalableFloat.GetValueAtLevel(GetAbilityLevel());
	FVector End= GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1*500;
	FHitResult hit;
	//RollTargetLocation
	
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End + Start, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, hit, true)) {
		MWC->AddOrUpdateWarpTargetFromLocation(FName("RollTargetLocation"), hit.ImpactPoint);
	}
	else {
		MWC->RemoveWarpTarget(FName("RollTargetLocation"));
	}
}

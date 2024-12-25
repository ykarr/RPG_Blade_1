// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Characters/BladeCharacterBase.h"
#include "Interfaces/EnemyDieInterface.h"
#include "Components/TimelineComponent.h"
#include "BladeEnemyCharacterBase.generated.h"

class UBoxComponent;
class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;
class AWeaponBase;
/**
 * 
 */
UCLASS()
class BLADE_API ABladeEnemyCharacterBase : public ABladeCharacterBase,public IEnemyDieInterface
{
	GENERATED_BODY()
public:
	// ICombatComponentInterface��(��) ���� ��ӵ�
	virtual UBaseCombatComponent* GetCombatComponent()  const override;
	// IEnemyDieInterface��(��) ���� ��ӵ�
	virtual void OnEnemyDied() override;
	// IPawnUIInterface��(��) ���� ��ӵ�
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent()const override;
	virtual void BeginPlay() override;
	virtual void Tick(float Sec) override;

protected:
	ABladeEnemyCharacterBase();
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
#if WITH_EDITOR
	//~ UObject Interface. �𸮾� �����Ϳ��� C++ Ŭ������ Ư�� ������Ƽ�� �����Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)override;
#endif
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandBoneName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandBoneName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Start|Data")
	TArray<UAnimMontage*> EntryMontagesToPlay;
private:
	void InitEnemyStartUpdate();
	void SetDissolveTimeline();
	UFUNCTION()
	void DissolveFunc(float value);
	UFUNCTION()
	void DissolveEndFunc();
	UPROPERTY(EditDefaultsOnly,Category="Data", meta = (AllowPrivateAccess = true))
	bool bHasWeaponMesh=false;
	UPROPERTY(EditDefaultsOnly,Category="Data", meta = (AllowPrivateAccess = true))
	bool bHasChildMesh=false;
	UPROPERTY(EditDefaultsOnly,Category="Data", meta = (AllowPrivateAccess = true))
	UCurveFloat* DissolveCurve;
	FTimeline DissolveTimeline;
	FTimeline StartDissolveTimeline;
#pragma region Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat",meta=(AllowprivateAccess=true))
	UBoxComponent* LeftHandCollisionBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat",meta=(AllowprivateAccess=true))
	UBoxComponent* RightHandCollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat",meta=(AllowprivateAccess=true))
	UEnemyCombatComponent* EnemyCombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowprivateAccess = true))
	UEnemyUIComponent* EnemyUIComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowprivateAccess = true))
	UWidgetComponent* EnemyHealthWidgetComponent;
#pragma endregion
public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent;}
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const {return LeftHandCollisionBox;}
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const {return RightHandCollisionBox;}

	
};

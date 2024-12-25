// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameModes/BladeGameModeBase.h"
#include "BladeSurvialGameMode.generated.h"

class ABladeEnemyCharacterBase;

UENUM(BlueprintType)
enum class EBladeSurvialGameModeState :uint8 {
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};
USTRUCT(BlueprintType)
struct FBladeEnemyWaveSpawnInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ABladeEnemyCharacterBase> SoftEnemyClassToSpawn;
	UPROPERTY(EditAnywhere)
	int32 MinSpawnCount=1;
	UPROPERTY(EditAnywhere)
	int32 MaxSpawnCount = 3;


};
USTRUCT(BlueprintType)
struct FBladeEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<FBladeEnemyWaveSpawnInfo> EnemyWaveSpawnerDefinitions;
	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave=1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate, EBladeSurvialGameModeState, CurrentState);

class UWaveTextWidget;
class UWaveTextNoCountWidget;
class ULostScreenOverlayWidget;
class ABladeEnemyCharacterBase;
class UBladeWidgetBase;
/**
 * 
 */
UCLASS()
class BLADE_API ABladeSurvialGameMode : public ABladeGameModeBase
{
	GENERATED_BODY()
public:
	void RegisterSpawnEnemies(const TArray<ABladeEnemyCharacterBase*>& InRegisterSpawnEnemies);
	void SetGameModeState(EBladeSurvialGameModeState InState);
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSec) override;
	//UFUNCTION(BlueprintCallable)
	
private:
	bool DoesFinishedAllWave()const;
	void PreLoadNextWaveEnemies();
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;
	FBladeEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow()const;
	UFUNCTION()
	void OnGameModeStateChanged(EBladeSurvialGameModeState InState);
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	UPROPERTY()
	EBladeSurvialGameModeState CurrentGameModeState;
	
	UPROPERTY()
	int32 CurrentEnemiesCounter = 0;
	UPROPERTY()
	int32 TotalSpawnEnemiesThisWaveCounter = 0;
	UPROPERTY()
	int32 CurrentWaveCount=1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveData", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemySpawnerDataTable;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveData", meta = (AllowPrivateAccess = "true"))
	int32 TotalWaveToSpawn;
	//Wait Time
	FTimerHandle WidgetDelay;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveData", meta = (AllowPrivateAccess = "true"))
	float SapwnWaitTIme = 5.f;
	UPROPERTY()
	float ElapsedTime=0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveData", meta = (AllowPrivateAccess = "true"))
	float SpawnEneyDelayTime=2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveData", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime=5.f;
	//Spawn Loc Actors
	UPROPERTY()
	TArray<AActor*> TargetPoints;
	UPROPERTY()
	TMap<TSoftClassPtr<ABladeEnemyCharacterBase>, UClass* > PreLoadedEnemyClassMap;
	UPROPERTY(BlueprintAssignable)
	FOnSurvialGameModeStateChangedDelegate OnSurvialGameModeStateChanged;

#pragma region Widget
public:
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UWaveTextWidget> WaveTextWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UWaveTextNoCountWidget> WaveTextNoCountWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSoftClassPtr<ULostScreenOverlayWidget> SoftLostScreenOverlayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSoftClassPtr<UBladeWidgetBase> SoftWintScreenOverlayWidgetClass;
	//TSubclassOf<ULostScreenOverlayWidget> LostScreenOverlayWidgetClass;

private:
	void CreateWaveWidget(FText* InTextDisplay, float TotalCountDownTime);
	void CreateWaveNoCountWidget(FText* InTextDisplay);

	UPROPERTY()
	UBladeWidgetBase* WinScreenOverlayWidget;
	UPROPERTY()
	ULostScreenOverlayWidget* LostScreenOverlayWidget;
	UPROPERTY()
	UWaveTextWidget* WaveTextWidget;
	UPROPERTY()
	UWaveTextNoCountWidget* WaveTextNoCountWidget;

	FTimerHandle LostWidgetStartDelay;
	FTimerHandle WinWidgetStartDelay;
#pragma endregion

};

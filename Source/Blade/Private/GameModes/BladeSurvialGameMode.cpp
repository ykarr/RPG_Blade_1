// YunMinSeong. All right reserved. 


#include "GameModes/BladeSurvialGameMode.h"
//Widget
#include "Widgets/GameModeWidget/WaveTextWidget.h"
#include "Widgets/GameModeWidget/WaveTextNoCountWidget.h"
#include "Widgets/Overlay/LostScreenOverlayWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
//ASync Loading
#include "Engine\AssetManager.h"
//Navigation
#include "NavigationSystem.h"

#include "Characters\BladeEnemyCharacterBase.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet\GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "BladeFunctionLibrary.h"
//Debug
#include "CustomDebugHelper.h"
void ABladeSurvialGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame( MapName,  Options,  ErrorMessage);
	EBladeGameDifficulty SavedDifficulty;
	
	if (UBladeFunctionLibrary::TryLoadSaveGameDifficulty(SavedDifficulty)) {
		GameDifficulty = SavedDifficulty;
	}
}
void ABladeSurvialGameMode::BeginPlay()
{
	Super::BeginPlay();
	OnSurvialGameModeStateChanged.AddDynamic(this,&ThisClass::OnGameModeStateChanged);
	
	checkf(EnemySpawnerDataTable, TEXT("Please Fill EnemySpawnerDataTable"));
	SetGameModeState(EBladeSurvialGameModeState::WaitSpawnNewWave);
	TotalWaveToSpawn=EnemySpawnerDataTable->GetRowNames().Num();
	PreLoadNextWaveEnemies();
}
void ABladeSurvialGameMode::PreLoadNextWaveEnemies()
{
	if (DoesFinishedAllWave()) {
		return;
	}
	PreLoadedEnemyClassMap.Empty();
	for (const FBladeEnemyWaveSpawnInfo& SpawnInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions) {
		if (SpawnInfo.SoftEnemyClassToSpawn.IsNull()) continue;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(SpawnInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
			[&]() {
				if (UClass* LoadedEnemyClass = SpawnInfo.SoftEnemyClassToSpawn.Get()) {
					//PreLoadedEnemyClassMap.Emplace(SpawnInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					PreLoadedEnemyClassMap.Add(SpawnInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
				}
			}));
	}
}

void ABladeSurvialGameMode::Tick(float DeltaSec)
{
	Super::Tick(DeltaSec);
	if (CurrentGameModeState == EBladeSurvialGameModeState::WaitSpawnNewWave) {
		ElapsedTime += DeltaSec;
		if (ElapsedTime >= SapwnWaitTIme) {
			ElapsedTime = 0;
			SetGameModeState(EBladeSurvialGameModeState::SpawningNewWave);
		}
	}
	if (CurrentGameModeState==EBladeSurvialGameModeState::SpawningNewWave) {
		ElapsedTime += DeltaSec;
		if (ElapsedTime >= SpawnEneyDelayTime) {
			CurrentEnemiesCounter+= TrySpawnWaveEnemies();
			ElapsedTime = 0;
			SetGameModeState(EBladeSurvialGameModeState::InProgress);
		}

	}
	if (CurrentGameModeState == EBladeSurvialGameModeState::WaveCompleted) {
		ElapsedTime += DeltaSec;
		if (ElapsedTime>=WaveCompletedWaitTime) {
			ElapsedTime = 0;
			CurrentWaveCount++;
			if (DoesFinishedAllWave()) {
				SetGameModeState(EBladeSurvialGameModeState::AllWavesDone);
			}
			else {
				SetGameModeState(EBladeSurvialGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void ABladeSurvialGameMode::RegisterSpawnEnemies(const TArray<ABladeEnemyCharacterBase*>& InRegisterSpawnEnemies)
{
	for (ABladeEnemyCharacterBase* SapwnedEnemy :InRegisterSpawnEnemies) {
		if (SapwnedEnemy) {
			CurrentEnemiesCounter++;
			TotalSpawnEnemiesThisWaveCounter++;
			SapwnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
		}
	}
}

void ABladeSurvialGameMode::CreateWaveWidget(FText* InTextDisplay,float TotalCountDownTime)
{
	if (!WaveTextWidget) {
		WaveTextWidget=CreateWidget<UWaveTextWidget>(GetWorld()->GetFirstPlayerController(), WaveTextWidgetClass, FName("WaveWidget"));
	}
	WaveTextWidget->AddToViewport(1);
	if (InTextDisplay) {
		
		WaveTextWidget->SetWaveMainText(InTextDisplay);
		WaveTextWidget->StartCountDown(TotalCountDownTime);
	}
}

void ABladeSurvialGameMode::CreateWaveNoCountWidget(FText* InTextDisplay)
{

	WaveTextNoCountWidget=CreateWidget<UWaveTextNoCountWidget>(GetWorld()->GetFirstPlayerController(), WaveTextNoCountWidgetClass,FName("WaveTextNoCountWidget"));
	WaveTextNoCountWidget->AddToViewport(1);
	WaveTextNoCountWidget->SetWaveMainText(InTextDisplay);
}

bool ABladeSurvialGameMode::DoesFinishedAllWave() const
{
	return CurrentWaveCount>TotalWaveToSpawn;
}

void ABladeSurvialGameMode::SetGameModeState(EBladeSurvialGameModeState InState)
{
	CurrentGameModeState = InState;
	OnSurvialGameModeStateChanged.Broadcast(CurrentGameModeState);
}


int32 ABladeSurvialGameMode::TrySpawnWaveEnemies()
{
	if (TargetPoints.IsEmpty()) {
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPoints);
	}
	//TargetPoints가 비어있을때 오류메시지 출력 
	checkf(!TargetPoints.IsEmpty(), TEXT("TargetActor is Not Found"));

	uint32 EnemiesSpawnedThisTime = 0;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (const FBladeEnemyWaveSpawnInfo& SpawnInfo: GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions) {
		if (SpawnInfo.SoftEnemyClassToSpawn.IsNull()) continue;
		const int32 NumtoSpawn = FMath::RandRange(SpawnInfo.MinSpawnCount, SpawnInfo.MaxSpawnCount);
		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnInfo.SoftEnemyClassToSpawn);
		for (int32 i = 0; i < NumtoSpawn; i++) {
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPoints.Num() - 1);
			const FVector SPawnLocOrigin = TargetPoints[RandomTargetPointIndex]->GetActorLocation();
			//벡터가 가리키는 방향으로 회전 생성
			const FRotator SpawnRot = TargetPoints[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
			FVector RandomLoc;
			UNavigationSystemV1::K2_GetRandomPointInNavigableRadius(this, SPawnLocOrigin, RandomLoc,500.f);
			ABladeEnemyCharacterBase* SpawnEnemyCharacter=GetWorld()->SpawnActor<ABladeEnemyCharacterBase>(LoadedEnemyClass, FVector(RandomLoc.X, RandomLoc.Y, SPawnLocOrigin.Z * 150.f), SpawnRot, SpawnParams);
			//ABladeEnemyCharacterBase* SpawnEnemyCharacter=GetWorld()->SpawnActor<ABladeEnemyCharacterBase>(LoadedEnemyClass, RandomLoc, SpawnRot, SpawnParams);
			if (SpawnEnemyCharacter) {
				SpawnEnemyCharacter->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
				
				EnemiesSpawnedThisTime++;
				TotalSpawnEnemiesThisWaveCounter++;
			}
			if (!ShouldKeepSpawnEnemies()) {
				return EnemiesSpawnedThisTime;
			}
		}
	}
	return EnemiesSpawnedThisTime;
}

bool ABladeSurvialGameMode::ShouldKeepSpawnEnemies() const
{//소환해야하는 적의 수를 다 채웠는지 검사.
	return TotalSpawnEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

FBladeEnemyWaveSpawnerTableRow* ABladeSurvialGameMode::GetCurrentWaveSpawnerTableRow() const
{
	//
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
	//Contextstring은 행을 찾을 수 없을때 나오는 메시지.
	FBladeEnemyWaveSpawnerTableRow* FoundRow=EnemySpawnerDataTable->FindRow<FBladeEnemyWaveSpawnerTableRow>(RowName,FString());
	checkf(FoundRow, TEXT("RowData error"));
	return FoundRow;
}

void ABladeSurvialGameMode::OnGameModeStateChanged(EBladeSurvialGameModeState InState)
{
	FText DisplayWavetext;

	switch (InState)
	{
	case EBladeSurvialGameModeState::WaitSpawnNewWave:
		//FText DisplayWavetext = LOCTEXT("DisplayWaveMaintext", "Wave {0}");
		// FText::Format을 이용하여 값 삽입
		DisplayWavetext = FText::Format(NSLOCTEXT("WaveMainTextKey", "WaveWaiting", "Wave {0} Wating"), FText::AsNumber(CurrentWaveCount));
		CreateWaveWidget(&DisplayWavetext, SapwnWaitTIme);
		break;
	case EBladeSurvialGameModeState::SpawningNewWave:
		DisplayWavetext = FText::Format(NSLOCTEXT("WaveMainTextKey", "WaveSpawn", "Wave {0}"), FText::AsNumber(CurrentWaveCount));
		SpawnEneyDelayTime = 2.f;
		//NameSpace로 그룹화, Key는 고유한 것, 내용.
		CreateWaveNoCountWidget(&DisplayWavetext);
		if (TotalWaveToSpawn==CurrentWaveCount) {
			SpawnEneyDelayTime = 4.f;
			GetWorld()->GetTimerManager().ClearTimer(WidgetDelay);
			GetWorld()->GetTimerManager().SetTimer(WidgetDelay, FTimerDelegate::CreateLambda([&]() {
				FText LamdaDisplayWavetext =NSLOCTEXT("WaveMainTextKey", "WaveSpawn", "Last Wave!");
				CreateWaveNoCountWidget(&LamdaDisplayWavetext);
			}), 2.f, false);
		}
	
		break;
	case EBladeSurvialGameModeState::InProgress:
		break;
	case EBladeSurvialGameModeState::WaveCompleted:
		DisplayWavetext = FText::Format(NSLOCTEXT("WaveMainTextKey", "WaveWaiting", "Wave {0} Completed"), FText::AsNumber(CurrentWaveCount));
		CreateWaveWidget(&DisplayWavetext, WaveCompletedWaitTime);
		break;
	case EBladeSurvialGameModeState::AllWavesDone:
		Debug::Print("AllWavesDone");
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0, 1, 3, FLinearColor::Black);
		GetWorld()->GetTimerManager().ClearTimer(WinWidgetStartDelay);
		GetWorld()->GetTimerManager().SetTimer(WinWidgetStartDelay, FTimerDelegate::CreateLambda([&]() {
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				SoftWintScreenOverlayWidgetClass.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda([&]() {
					if (SoftWintScreenOverlayWidgetClass.Get()) {
						WinScreenOverlayWidget = CreateWidget<ULostScreenOverlayWidget>(GetWorld(), SoftWintScreenOverlayWidgetClass.Get());
						//숫자가 클수록 앞에.
						WinScreenOverlayWidget->AddToViewport(10);
					}
					})
			);
			}), 3.f, false);
		break;
	case EBladeSurvialGameModeState::PlayerDied:
		Debug::Print("PlayerDied");
		//모든 위젯을 지우고 카메라 매니져를 가져와 검은화면으로 만든다.
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0,1,2,FLinearColor::Black);
		GetWorld()->GetTimerManager().ClearTimer(LostWidgetStartDelay);
		GetWorld()->GetTimerManager().SetTimer(LostWidgetStartDelay,FTimerDelegate::CreateLambda([&](){
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				SoftLostScreenOverlayWidgetClass.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda([&](){
					if (SoftLostScreenOverlayWidgetClass.Get()) {
						LostScreenOverlayWidget = CreateWidget<ULostScreenOverlayWidget>(GetWorld(), SoftLostScreenOverlayWidgetClass.Get());
						//숫자가 클수록 앞에.
						LostScreenOverlayWidget->AddToViewport(10);
					}
				})
			);
		}), 2.f, false);
		break;
	default:
		break;
	}
}

void ABladeSurvialGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentEnemiesCounter--;
	if (ShouldKeepSpawnEnemies()) {
		CurrentEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if (CurrentEnemiesCounter==0) {
		TotalSpawnEnemiesThisWaveCounter = 0;
		CurrentEnemiesCounter = 0;
		SetGameModeState(EBladeSurvialGameModeState::WaveCompleted);
	}
}


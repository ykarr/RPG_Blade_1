//
#include "BladeGameInstance.h"
#include "MoviePlayer.h"
TSoftObjectPtr<UWorld> UBladeGameInstance::GetGameLevelByTag(FGameplayTag InTag)const
{
    for (const FBladeGameLevelSet& GamelevelSet: GameLevelSets) {
        if (!GamelevelSet.IsValid()) continue;
        if (GamelevelSet.LevelTag== InTag) {
            return GamelevelSet.Level;
        }
    }
    return TSoftObjectPtr<UWorld>();
}

void UBladeGameInstance::Init()
{
    Super::Init();
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMap);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnDestinationWorldLoaded);
}

void UBladeGameInstance::OnPreLoadMap(const FString& PreLoadMap)
{
    FLoadingScreenAttributes LoadingScreenAttributes;
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    //로딩화면 최소 2초 유지.
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
    LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UBladeGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
    GetMoviePlayer()->StopMovie();
}

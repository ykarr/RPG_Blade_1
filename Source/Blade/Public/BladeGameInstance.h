// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "BladeGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FBladeGameLevelSet{

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(Categories="GameData"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const {
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

/**
 * 
 */
UCLASS()
class BLADE_API UBladeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure,meta=(GameplayTagFilter="GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag) const;
protected:
	virtual void Init()override;
	virtual void OnPreLoadMap(const FString& PreLoadMap/* MapName */);
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld/* LoadedWorld */);
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FBladeGameLevelSet> GameLevelSets;
};

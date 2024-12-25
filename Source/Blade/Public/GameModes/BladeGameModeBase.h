// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BladeTypes/BladeEnumTypes.h"
#include "BladeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API ABladeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABladeGameModeBase();
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Game Settings")
	EBladeGameDifficulty GameDifficulty;

public:
	FORCEINLINE EBladeGameDifficulty GetCurrentGameDifficulty()const { return GameDifficulty; }
};

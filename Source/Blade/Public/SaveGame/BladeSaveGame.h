// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BladeTypes\BladeEnumTypes.h"
#include "BladeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBladeSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	EBladeGameDifficulty SaveGameDifficulty=EBladeGameDifficulty::Normal;
};

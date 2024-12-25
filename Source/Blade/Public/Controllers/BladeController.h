// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "BladeController.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API ABladeController : public APlayerController,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	ABladeController();
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;
private:
	FGenericTeamId PlayerTeamID;
};

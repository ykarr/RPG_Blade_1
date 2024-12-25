// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"

class UPawnUIComponent;
class UPlayerUIComponent;
class UEnemyUIComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLADE_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPawnUIComponent* GetPawnUIComponent() const  = 0;
	virtual UPlayerUIComponent* GetPlayerUIComponent()const;
	virtual UEnemyUIComponent* GetEnemyUIComponent()const;
	
};

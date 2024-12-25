// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatComponentInterface.generated.h"


class UBaseCombatComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLADE_API ICombatComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UBaseCombatComponent* GetCombatComponent() const = 0;
};

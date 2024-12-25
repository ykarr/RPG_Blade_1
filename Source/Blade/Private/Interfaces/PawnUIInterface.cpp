// YunMinSeong. All right reserved. 


#include "Interfaces/PawnUIInterface.h"

// Add default functionality here for any IPawnUIInterface functions that are not pure virtual.

UPlayerUIComponent* IPawnUIInterface::GetPlayerUIComponent() const
{
	return nullptr;
}

UEnemyUIComponent* IPawnUIInterface::GetEnemyUIComponent() const
{
	return nullptr;
}

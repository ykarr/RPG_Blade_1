// YunMinSeong. All right reserved. 


#include "Controllers/BladeController.h"

ABladeController::ABladeController()
{
	PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId ABladeController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

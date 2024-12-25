// YunMinSeong. All right reserved. 


#include "AnimInstances/Player/BladePlayerLinkedAnimLayer.h"
#include "AnimInstances/Player/BladePlayerAnimInstance.h"
UBladePlayerAnimInstance* UBladePlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UBladePlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());

}

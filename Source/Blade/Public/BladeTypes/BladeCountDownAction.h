// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BladeTypes\BladeEnumTypes.h"
/**
 * FPendingLatentAction�� Unreal Engine���� ������ �۾�(Latent Action)�� �����ϰ� �����ϱ� ���� �߻� Ŭ����
 */
class FBladeCountDownAction:public FPendingLatentAction
{
public:
	FBladeCountDownAction(float InTotalCountTime,float InUpdateInterval,float& InOutRemainingTime, EBladeCountDownActionOutput& InBladeCountDownOutput,const FLatentActionInfo& LatentInfo)
		:bNeedToCancel(false),
		TotalCountDownTime(InTotalCountTime),
		UpdateInterval(InUpdateInterval),
		OutRemainingTime(InOutRemainingTime),
		BladeCountDownOutput(InBladeCountDownOutput),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget),
		ElapsedInterval(0.f),
		ElapsedTimeSinceStart(InTotalCountTime- InOutRemainingTime)
	{}
	virtual void UpdateOperation(FLatentResponse& Response)override;
	void CancelAction();
private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EBladeCountDownActionOutput& BladeCountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};

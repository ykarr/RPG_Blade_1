// YunMinSeong. All right reserved. 


#include "BladeTypes\BladeCountDownAction.h"

void FBladeCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	// 만약 작업을 취소해야 하는 경우
	if (bNeedToCancel) {
		BladeCountDownOutput = EBladeCountDownActionOutput::Cancelled;
		// 작업을 종료하고 지정된 콜백 함수(ExecutionFunction)를 트리거
		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);
		return;
	}
	// 카운트다운이 완료된 경우 (전체 카운트다운 시간을 다 소모했을 때)
	if (ElapsedTimeSinceStart >= TotalCountDownTime) {
		BladeCountDownOutput = EBladeCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);
		return;
	}
	// 카운트다운 간격을 아직 다 채우지 못한 경우
	if (ElapsedInterval<UpdateInterval) {
		// 경과된 시간을 계속 누적
		ElapsedInterval += Response.ElapsedTime();// 프레임당 경과 시간을 누적
	}
	else {// 카운트다운이 갱신될 타이밍이 되었을 때
		// 업데이트 간격이 0.1이상이면 UpdateInterval값을 사용하고, 0이하일경우 Response.ElapsedTime()값을 이용.
		// Response.ElapsedTime() 실제로 경과된 시간으로, 프레임마다 경과된 시간값을 반환
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval:Response.ElapsedTime();
		// 남은 시간을 계산
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;
		BladeCountDownOutput = EBladeCountDownActionOutput::Updated;
		// 콜백 함수(ExecutionFunction)를 트리거하여 카운트다운 갱신을 알림
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		// 간격을 0으로 초기화하여 다음 갱신 간격을 시작
		ElapsedInterval = 0.f;
	}
}

void FBladeCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

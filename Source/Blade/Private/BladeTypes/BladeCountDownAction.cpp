// YunMinSeong. All right reserved. 


#include "BladeTypes\BladeCountDownAction.h"

void FBladeCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	// ���� �۾��� ����ؾ� �ϴ� ���
	if (bNeedToCancel) {
		BladeCountDownOutput = EBladeCountDownActionOutput::Cancelled;
		// �۾��� �����ϰ� ������ �ݹ� �Լ�(ExecutionFunction)�� Ʈ����
		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);
		return;
	}
	// ī��Ʈ�ٿ��� �Ϸ�� ��� (��ü ī��Ʈ�ٿ� �ð��� �� �Ҹ����� ��)
	if (ElapsedTimeSinceStart >= TotalCountDownTime) {
		BladeCountDownOutput = EBladeCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);
		return;
	}
	// ī��Ʈ�ٿ� ������ ���� �� ä���� ���� ���
	if (ElapsedInterval<UpdateInterval) {
		// ����� �ð��� ��� ����
		ElapsedInterval += Response.ElapsedTime();// �����Ӵ� ��� �ð��� ����
	}
	else {// ī��Ʈ�ٿ��� ���ŵ� Ÿ�̹��� �Ǿ��� ��
		// ������Ʈ ������ 0.1�̻��̸� UpdateInterval���� ����ϰ�, 0�����ϰ�� Response.ElapsedTime()���� �̿�.
		// Response.ElapsedTime() ������ ����� �ð�����, �����Ӹ��� ����� �ð����� ��ȯ
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval:Response.ElapsedTime();
		// ���� �ð��� ���
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;
		BladeCountDownOutput = EBladeCountDownActionOutput::Updated;
		// �ݹ� �Լ�(ExecutionFunction)�� Ʈ�����Ͽ� ī��Ʈ�ٿ� ������ �˸�
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		// ������ 0���� �ʱ�ȭ�Ͽ� ���� ���� ������ ����
		ElapsedInterval = 0.f;
	}
}

void FBladeCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

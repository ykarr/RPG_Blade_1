// YunMinSeong. All right reserved. 


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageToken.h"
#include "AbilitySystem\BladeAttributeSet.h"
#include "BladeGameplayTags.h"

//Debug
#include "CustomDebugHelper.h"
struct FBladeDamgeCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	FBladeDamgeCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBladeAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBladeAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBladeAttributeSet, DamageTaken, Target, false)
	};
};
static const FBladeDamgeCapture& GetBladeDamageCapture() {
	static FBladeDamgeCapture BladeDamgeCapture;
	return BladeDamgeCapture;
}
UGEExecCalc_DamageToken::UGEExecCalc_DamageToken()
{
	/*Slow way of doing Capture*/
	/*FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		USlashAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(USlashAttributeSet,AttackPower)
	);
	//���� �÷��� ȿ���� �����ϴ� �ҽ�
	FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
		AttackPowerProperty,
		EGameplayEffectAttributeCaptureSource::Source,
		false
	);
	RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);*/
	//Gameplay Effect�� ����� �� �ʿ��� �Ӽ�(Attribute)���� �����ϰ�, �ش� �Ӽ����� ĸó�ؼ� GameplayEffectSpec�� ��꿡 ���
	RelevantAttributesToCapture.Add(GetBladeDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetBladeDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetBladeDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageToken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec=ExecutionParams.GetOwningSpec();
	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/

	FAggregatorEvaluateParameters EvaluateParameters;
	//Source�� �ɷ��� �ߵ��� ĳ���ͳ� �������� �Ӽ��� �ǹ��մϴ�.
	//Target�� �ɷ��� ����Ǵ� ���, �� �����ڳ� �������� �Ӽ��� �ǹ��մϴ�.
	//GetAggregatedTags()**�� Gameplay Effect�� ���õ� ��� �±׸� �������� �Լ��Դϴ�.
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	// Gameplay Ability System���� ĸó�� �Ӽ��� �������� Gameplay Effect�� ȿ�� ũ�⸦ ����ϴ� �Լ��Դϴ�.
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetBladeDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes) {
		if (TagMagnitude.Key.MatchesTagExact(BladeGameplayTags::Shared_SetByCaller_BaseDamage)) {
			BaseDamage=TagMagnitude.Value;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}
		if (TagMagnitude.Key.MatchesTagExact(BladeGameplayTags::Player_SetByCaller_AttackType_Light)) {
			UsedLightAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedLightAttackComboCount"), UsedLightAttackComboCount);
		}
		if (TagMagnitude.Key.MatchesTagExact(BladeGameplayTags::Player_SetByCaller_AttackType_Heavy)) {
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);*/
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetBladeDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);

	if (UsedLightAttackComboCount !=0) {
		//�޺��� 1�ΰ��(���۰���) ���������� 0
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05f+1.f;
		BaseDamage *= DamageIncreasePercentLight;
		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}
	if (UsedHeavyAttackComboCount != 0) {
		//�޺��� 1�ΰ��(���۰���) ���������� 0
		const float DamageIncreasePercentHeavy = (UsedHeavyAttackComboCount - 1) * 0.15f + 1.f;
		BaseDamage *= DamageIncreasePercentHeavy;
		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);
	if (FinalDamageDone > 0.f) {
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetBladeDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}

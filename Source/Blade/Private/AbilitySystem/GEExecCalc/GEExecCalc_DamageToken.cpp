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
	//게임 플레이 효과를 적용하는 소스
	FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
		AttackPowerProperty,
		EGameplayEffectAttributeCaptureSource::Source,
		false
	);
	RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);*/
	//Gameplay Effect가 적용될 때 필요한 속성(Attribute)들을 정의하고, 해당 속성들을 캡처해서 GameplayEffectSpec의 계산에 사용
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
	//Source는 능력을 발동한 캐릭터나 공격자의 속성을 의미합니다.
	//Target은 능력이 적용되는 대상, 즉 피해자나 수신자의 속성을 의미합니다.
	//GetAggregatedTags()**는 Gameplay Effect와 관련된 모든 태그를 가져오는 함수입니다.
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	// Gameplay Ability System에서 캡처된 속성을 바탕으로 Gameplay Effect의 효과 크기를 계산하는 함수입니다.
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
		//콤보가 1인경우(시작공격) 데미지증가 0
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05f+1.f;
		BaseDamage *= DamageIncreasePercentLight;
		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}
	if (UsedHeavyAttackComboCount != 0) {
		//콤보가 1인경우(시작공격) 데미지증가 0
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

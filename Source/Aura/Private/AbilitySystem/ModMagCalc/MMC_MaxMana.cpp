


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// Intelligence 속성을 캡처하도록 설정.
	// UAuraAttributeSet 클래스의 Intelligence 속성을 가져와 AttributeToCapture에 설정.
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();

	// Intelligence 속성의 값을 타겟(Target)에서 캡처하도록 설정.
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// bSnapshot을 false로 설정하여, 속성 값을 지속적으로 업데이트된 최신 값으로 참조.
	IntDef.bSnapshot = false;

	// Intelligence 속성을 캡처해야 하는 중요한 속성 목록에 추가.
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 소스와 타겟에서 수집한 태그.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 평가에 사용할 매개변수의 설정.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Intelligence 속성을 가져와 값을 설정하고, 0보다 작은 경우 0으로 고정.
	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);
	Int = FMath::Max<float>(Int, 0.f);

	// CombatInterface를 사용해 소스 객체에서 플레이어 레벨을 가져옴.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// 최대 체력 값을 계산하여 반환. 기본값 50 + Intelligence에 따른 증가 + 플레이어 레벨에 따른 증가.
	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
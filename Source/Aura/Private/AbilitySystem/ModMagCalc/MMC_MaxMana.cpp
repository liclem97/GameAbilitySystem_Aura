


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// Intelligence �Ӽ��� ĸó�ϵ��� ����.
	// UAuraAttributeSet Ŭ������ Intelligence �Ӽ��� ������ AttributeToCapture�� ����.
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();

	// Intelligence �Ӽ��� ���� Ÿ��(Target)���� ĸó�ϵ��� ����.
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// bSnapshot�� false�� �����Ͽ�, �Ӽ� ���� ���������� ������Ʈ�� �ֽ� ������ ����.
	IntDef.bSnapshot = false;

	// Intelligence �Ӽ��� ĸó�ؾ� �ϴ� �߿��� �Ӽ� ��Ͽ� �߰�.
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// �ҽ��� Ÿ�ٿ��� ������ �±�.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// �򰡿� ����� �Ű������� ����.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Intelligence �Ӽ��� ������ ���� �����ϰ�, 0���� ���� ��� 0���� ����.
	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);
	Int = FMath::Max<float>(Int, 0.f);

	// CombatInterface�� ����� �ҽ� ��ü���� �÷��̾� ������ ������.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// �ִ� ü�� ���� ����Ͽ� ��ȯ. �⺻�� 50 + Intelligence�� ���� ���� + �÷��̾� ������ ���� ����.
	return 50.f + 2.f * Int + 15.f * PlayerLevel;
}
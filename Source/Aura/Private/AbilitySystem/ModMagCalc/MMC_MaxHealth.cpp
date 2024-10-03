


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// Vigor �Ӽ��� ĸó�ϵ��� ����.
	// UAuraAttributeSet Ŭ������ Vigor �Ӽ��� ������ AttributeToCapture�� ����.
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	// Vigor �Ӽ��� ���� Ÿ��(Target)���� ĸó�ϵ��� ����.
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// bSnapshot�� false�� �����Ͽ�, �Ӽ� ���� ���������� ������Ʈ�� �ֽ� ������ ����.
	VigorDef.bSnapshot = false;

	// Vigor �Ӽ��� ĸó�ؾ� �ϴ� �߿��� �Ӽ� ��Ͽ� �߰�.
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// �ҽ��� Ÿ�ٿ��� ������ �±�.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// �򰡿� ����� �Ű������� ����.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Vigor �Ӽ��� ������ ���� �����ϰ�, 0���� ���� ��� 0���� ����.
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	// �ִ� ü�� ���� ����Ͽ� ��ȯ. �⺻�� 80 + Vigor�� ���� ���� + �÷��̾� ������ ���� ����.
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
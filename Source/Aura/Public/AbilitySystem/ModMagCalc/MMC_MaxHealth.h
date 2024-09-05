

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * Gameplay Effect�� Modifier Magnitude (������ ũ��)�� Ŀ���� ������ ���� ����ϴ� �� ���,
 * �ִ� ü��(MaxHealth)�� ũ�⸦ �������� ����ϴ� ������ �����Ѵ�.
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();

	/**
	 * @brief ĳ������ �ִ� ü��(MaxHealth)�� ����ϴ� �Լ�.
	 *
	 * �� �Լ��� �־��� GameplayEffectSpec�� ������� ĳ������ �ִ� ü���� ����մϴ�.
	 * ĳ������ Vigor�� ����Ͽ� �ִ� ü�� ���� �������� �����մϴ�.
	 *
	 * @param Spec : GameplayEffectSpec����, �ִ� ü���� ����ϴ� �� �ʿ��� ĳ������ �Ӽ� �� ��Ÿ ������ �����մϴ�.
	 * @return ���� �ִ� ü�� ��(MaxHealth).
	 */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	// FGameplayEffectAttributeCaptureDefinition : 
	// GameplayEffect�� ����� �� Vigor �Ӽ� ���� ĸó�Ͽ� ȿ�� ��꿡 �ʿ��� �����͸� ����.
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};

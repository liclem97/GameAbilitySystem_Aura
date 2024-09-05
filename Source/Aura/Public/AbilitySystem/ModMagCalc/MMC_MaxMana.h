

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * Gameplay Effect�� Modifier Magnitude (������ ũ��)�� Ŀ���� ������ ���� ����ϴ� �� ���,
 * �ִ� ����(MaxMana)�� ũ�⸦ �������� ����ϴ� ������ �����Ѵ�.
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxMana();

	/**
	* @brief ĳ������ �ִ� ����(MaxMana)�� ����ϴ� �Լ�.
	*
	* �� �Լ��� �־��� GameplayEffectSpec�� ������� ĳ������ �ִ� ������ ����մϴ�.
	* ĳ������ Intelligence�� ����Ͽ� �ִ� ���� ���� �������� �����մϴ�.
	*
	* @param Spec : GameplayEffectSpec����, �ִ� ������ ����ϴ� �� �ʿ��� ĳ������ �Ӽ� �� ��Ÿ ������ �����մϴ�.
	* @return ���� �ִ� ���� ��(MaxMana).
	*/
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// FGameplayEffectAttributeCaptureDefinition : 
	// GameplayEffect�� ����� �� Intelligence �Ӽ� ���� ĸó�Ͽ� ȿ�� ��꿡 �ʿ��� �����͸� ����.
	FGameplayEffectAttributeCaptureDefinition IntDef;
};

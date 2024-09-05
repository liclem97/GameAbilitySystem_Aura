

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * Gameplay Effect의 Modifier Magnitude (수정자 크기)를 커스텀 로직을 통해 계산하는 데 사용,
 * 최대 마나(MaxMana)의 크기를 동적으로 계산하는 역할을 수행한다.
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxMana();

	/**
	* @brief 캐릭터의 최대 마나(MaxMana)를 계산하는 함수.
	*
	* 이 함수는 주어진 GameplayEffectSpec을 기반으로 캐릭터의 최대 마나를 계산합니다.
	* 캐릭터의 Intelligence를 고려하여 최대 마나 값을 동적으로 결정합니다.
	*
	* @param Spec : GameplayEffectSpec으로, 최대 마나를 계산하는 데 필요한 캐릭터의 속성 및 기타 정보를 포함합니다.
	* @return 계산된 최대 마나 값(MaxMana).
	*/
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// FGameplayEffectAttributeCaptureDefinition : 
	// GameplayEffect가 실행될 때 Intelligence 속성 값을 캡처하여 효과 계산에 필요한 데이터를 제공.
	FGameplayEffectAttributeCaptureDefinition IntDef;
};

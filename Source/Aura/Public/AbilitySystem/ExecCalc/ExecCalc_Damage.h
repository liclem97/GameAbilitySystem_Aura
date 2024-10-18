

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 데미지 GameplayEffect의 효과를 계산하고 이를 캐릭터에게 적용하는데 사용한다.
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage();
	
	/*
	 * @brief GameplayEffect가 적용될 때 커스텀 로직을 통해 특정 효과(예: 데미지)를 계산하는 함수.
	 * 
	 * @param ExecutionParams : 효과를 계산하는 데 필요한 다양한 데이터를 제공하는 구조체 (공격자, 방어자, 속성 등)
	 * @param OutExecutionOutput : 함수가 계산한 결과를 저장하는 구조체. 계산된 효과(예: 데미지, 힐 등)가 이곳에 저장되어 나중에 적용됨.
	 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutPut) const override;
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
							const FGameplayEffectSpec& Spec,
							FAggregatorEvaluateParameters& EvalutationParamters,
							const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;
};

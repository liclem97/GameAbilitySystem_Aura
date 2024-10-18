

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * ������ GameplayEffect�� ȿ���� ����ϰ� �̸� ĳ���Ϳ��� �����ϴµ� ����Ѵ�.
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage();
	
	/*
	 * @brief GameplayEffect�� ����� �� Ŀ���� ������ ���� Ư�� ȿ��(��: ������)�� ����ϴ� �Լ�.
	 * 
	 * @param ExecutionParams : ȿ���� ����ϴ� �� �ʿ��� �پ��� �����͸� �����ϴ� ����ü (������, �����, �Ӽ� ��)
	 * @param OutExecutionOutput : �Լ��� ����� ����� �����ϴ� ����ü. ���� ȿ��(��: ������, �� ��)�� �̰��� ����Ǿ� ���߿� �����.
	 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutPut) const override;
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
							const FGameplayEffectSpec& Spec,
							FAggregatorEvaluateParameters& EvalutationParamters,
							const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;
};



#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * UAuraAbilitySystemGlobals Ŭ����
 *
 * �� Ŭ������ UAbilitySystemGlobals�� ��ӹ޾� Aura ������Ʈ�� �°� Ŀ���͸������ �ɷ� �ý����� ���� ������ �����մϴ�.
 * �ַ� ���� ������ �߻��ϴ� GameplayEffect�� ���ؽ�Ʈ�� �Ҵ��ϴ� ������ �ϸ�,
 * �⺻ FGameplayEffectContext ��� Ŀ���ҵ� ���ؽ�Ʈ�� �����Ͽ� ������Ʈ�� �䱸 ���׿� �´� �߰����� �����͸� ó���� �� �ְ� �մϴ�.
 *
 * �ֿ� ����:
 * - AllocGameplayEffectContext() �Լ��� �������̵��Ͽ� ���� ������ ������ FGameplayEffectContext�� �����մϴ�.
 * - Aura ������Ʈ�� �ɷ� �ý��ۿ� �´� �۷ι� ���� �� ����� �����մϴ�.
 *
 * @see UAbilitySystemGlobals
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;	
};

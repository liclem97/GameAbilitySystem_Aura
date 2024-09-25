

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * UWaitCooldownChange Ŭ����
 *
 * �� Ŭ������ �������Ʈ���� �񵿱������� ��ٿ� ��ȭ�� ��ٸ��� �׼��� ����.
 * Ư�� �ɷ��� ��ٿ��� ���۵ǰų� ����� �� �̸� �����ϰ�, �ش� ������ ���� �̺�Ʈ�� ��ε�ĳ��Ʈ.
 *
 * �ֿ� ����:
 * - �ɷ� �ý��� ������Ʈ���� ��ٿ� ���� ��ȭ�� ����.
 * - ��ٿ� ���۰� ���ῡ ���� �̺�Ʈ ��������Ʈ�� �������Ʈ�� ��ε�ĳ��Ʈ.
 *
 * @see UBlueprintAsyncActionBase
 */
UCLASS(BlueprintType, meta= (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
		
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta =(BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};
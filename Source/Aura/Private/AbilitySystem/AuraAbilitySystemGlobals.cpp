


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "AuraAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{	
	// Ŀ���ҵ� FAuraGameplayEffectContext�� �Ҵ��Ͽ� ��ȯ
	return new FAuraGameplayEffectContext();
}

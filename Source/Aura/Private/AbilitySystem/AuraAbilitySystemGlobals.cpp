


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "AuraAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{	
	// 커스텀된 FAuraGameplayEffectContext를 할당하여 반환
	return new FAuraGameplayEffectContext();
}

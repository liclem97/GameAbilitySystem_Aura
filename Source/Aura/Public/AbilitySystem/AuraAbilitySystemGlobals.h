

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * UAuraAbilitySystemGlobals 클래스
 *
 * 이 클래스는 UAbilitySystemGlobals를 상속받아 Aura 프로젝트에 맞게 커스터마이즈된 능력 시스템의 전역 설정을 관리합니다.
 * 주로 게임 내에서 발생하는 GameplayEffect의 컨텍스트를 할당하는 역할을 하며,
 * 기본 FGameplayEffectContext 대신 커스텀된 컨텍스트를 생성하여 프로젝트의 요구 사항에 맞는 추가적인 데이터를 처리할 수 있게 합니다.
 *
 * 주요 역할:
 * - AllocGameplayEffectContext() 함수를 오버라이드하여 게임 내에서 맞춤형 FGameplayEffectContext를 생성합니다.
 * - Aura 프로젝트의 능력 시스템에 맞는 글로벌 설정 및 기능을 관리합니다.
 *
 * @see UAbilitySystemGlobals
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;	
};

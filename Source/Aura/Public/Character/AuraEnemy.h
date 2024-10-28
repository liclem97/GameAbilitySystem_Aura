

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

/**
 * AAuraEnemy 클래스
 *
 * 적 캐릭터를 나타내는 클래스. AAuraCharacterBase를 상속받아 적 캐릭터의 기본 동작을 제공하며,
 * IEnemyInterface를 구현하여 인터페이스 기반의 적 관련 동작(하이라이트, 언하이라이트 등)을 정의.
 *
 * 주요 역할:
 * - 적 AI의 행동과 상태를 관리하며, 전투 중 적의 레벨과 사망 동작을 처리.
 * - AIController와 Behavior Tree를 사용하여 적 AI의 인공지능을 제어.
 * - 건강 상태와 관련된 속성 변화를 처리하고 UI 위젯을 통해 적의 체력 상태를 표시.
 *
 * 제공 기능:
 * - EnemyInterface를 구현하여 적 캐릭터와의 상호작용을 관리. (예: 하이라이트 효과)
 * - 전투 관련 기능을 제공하고, 적 캐릭터의 사망 및 레벨을 처리.
 * - Hit Reaction(공격 반응)과 관련된 상태를 관리하고 표시.
 * - HealthBar 위젯과 관련된 기능을 제공하여 적 캐릭터의 체력 상태를 시각화.
 * - AIController와 Behavior Tree를 통해 AI 동작을 제어.
 *
 * @see AAuraCharacterBase, IEnemyInterface
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;	

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};

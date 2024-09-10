

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * AAuraAIController 클래스
 *
 * AI 캐릭터를 제어하는 AIController 클래스.
 * Behavior Tree와 Blackboard와 같은 AI 시스템을 활용하여 AI의 의사결정을 제어하는 역할.
 *
 * 주요 역할:
 * - AI 캐릭터의 행동을 관리하기 위해 Behavior Tree를 실행하고 제어.
 * - Blackboard와 상호작용하여 AI 캐릭터의 상태 및 목표를 추적하고 관리.
 *
 * 제공 기능:
 * - AI 캐릭터를 위한 기본 AIController 생성.
 * - Behavior Tree와 관련된 컴포넌트를 보유하여 AI의 행동을 관리.
 *
 * @see AAIController
 */

UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAuraAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};

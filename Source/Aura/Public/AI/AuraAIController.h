

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * AAuraAIController Ŭ����
 *
 * AI ĳ���͸� �����ϴ� AIController Ŭ����.
 * Behavior Tree�� Blackboard�� ���� AI �ý����� Ȱ���Ͽ� AI�� �ǻ������ �����ϴ� ����.
 *
 * �ֿ� ����:
 * - AI ĳ������ �ൿ�� �����ϱ� ���� Behavior Tree�� �����ϰ� ����.
 * - Blackboard�� ��ȣ�ۿ��Ͽ� AI ĳ������ ���� �� ��ǥ�� �����ϰ� ����.
 *
 * ���� ���:
 * - AI ĳ���͸� ���� �⺻ AIController ����.
 * - Behavior Tree�� ���õ� ������Ʈ�� �����Ͽ� AI�� �ൿ�� ����.
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

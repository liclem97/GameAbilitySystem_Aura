

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * UAttributeMenuWidgetController Ŭ����
 *
 * �� Ŭ������ �𸮾� ������ ���� ��Ʈ�ѷ��� ��ӹ޾� ��Ʈ����Ʈ �޴� ������ ���õ� UI ��ҵ��� �����ϴ� ������ ���.
 * �ַ� �÷��̾��� ���� Ư��(��, ����, ü��, ����) �� ���� Ư��(�����, ũ��Ƽ�� ��)�� ���� UI ������Ʈ�� �����ϰ�, �̸� �������Ʈ���� ���� ����� �� �ֵ��� ��.
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;	

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

};

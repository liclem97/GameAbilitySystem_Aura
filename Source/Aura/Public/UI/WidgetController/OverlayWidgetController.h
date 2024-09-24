

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

struct FOnAttributeChangeData;
class UAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * UOverlayWidgetController Ŭ����
 *
 * �� Ŭ������ �𸮾� ������ ���� ��Ʈ�ѷ��� ��ӹ޾� �������� ������ ���õ� UI ��ҵ��� �����ϴ� ������ ���.
 * �ַ� �÷��̾��� ����(ü��, ���� ��) �� �޽��� ������ ���� UI ������Ʈ�� �����ϰ�, �̸� �������Ʈ���� ���� ����� �� �ֵ��� ��.
 *
 * �ֿ� ���:
 * - �ʱⰪ�� ��ε�ĳ��Ʈ�Ͽ� UI�� �⺻ ���¸� ����.
 * - �Ӽ�(Attributes) ��ȭ�� ���� UI ��ҵ��� ������Ʈ�ϰ�, �Ӽ� ��ȭ �̺�Ʈ�� ��������Ʈ�� ����.
 * - �޽��� ������ ���� �����͸� ó���ϰ� �޽����� UI�� ǥ��.
 *
 * ���� ���:
 * - OnHealthChanged, OnMaxHealthChanged, OnManaChanged �� �÷��̾� �Ӽ� ��ȭ�� ���� ��������Ʈ.
 * - MessageWidgetRowDelegate�� ���� UI�� �޽����� ���õ� �����͸� ǥ��.
 *
 * ���� �޼���:
 * - BroadcastInitialValues(): ������ �ʱⰪ�� ��ε�ĳ��Ʈ�ϴ� �Լ�.
 * - BindCallbacksToDependencies(): UI ��ҿ� �ʿ��� �ݹ��� ���ε��ϴ� �Լ�.
 * - GetDataTableRowByTag(): ������ ���̺��� �±׸� �������� �����͸� �������� ���ø� �Լ�.
 *
 * @see UAuraWidgetController
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	// �ʱ� �Ӽ� ���� ��ε�ĳ��Ʈ�ϴ� �Լ�
	virtual void BroadcastInitialValues() override;

	// �Ӽ� ��ȭ�� ���� �ݹ��� ���ε��ϴ� �Լ�
	virtual void BindCallbacksToDependencies() override;

	// �÷��̾��� �Ӽ� ��ȭ �̺�Ʈ (ü��, ���� ��)
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:
	// �޽��� ���� ���� �����͸� �����ϴ� ������ ���̺�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	// �ɷ� ���� �����͸� �����ϴ� ��ü
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// �±׸� �������� ������ ���̺��� ���� �������� ���ø� �Լ�
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));	
}



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
class UAuraAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * UOverlayWidgetController 클래스
 *
 * 이 클래스는 언리얼 엔진의 위젯 컨트롤러를 상속받아 오버레이 위젯과 관련된 UI 요소들을 관리하는 역할을 담당.
 * 주로 플레이어의 상태(체력, 마나 등) 및 메시지 위젯과 같은 UI 컴포넌트를 갱신하고, 이를 블루프린트에서 쉽게 사용할 수 있도록 함.
 *
 * 주요 기능:
 * - 초기값을 브로드캐스트하여 UI의 기본 상태를 설정.
 * - 속성(Attributes) 변화에 따라 UI 요소들을 업데이트하고, 속성 변화 이벤트를 델리게이트로 전달.
 * - 메시지 위젯에 대한 데이터를 처리하고 메시지를 UI에 표시.
 *
 * 제공 기능:
 * - OnHealthChanged, OnMaxHealthChanged, OnManaChanged 등 플레이어 속성 변화에 따른 델리게이트.
 * - MessageWidgetRowDelegate를 통해 UI에 메시지와 관련된 데이터를 표시.
 *
 * 제공 메서드:
 * - BroadcastInitialValues(): 위젯의 초기값을 브로드캐스트하는 함수.
 * - BindCallbacksToDependencies(): UI 요소에 필요한 콜백을 바인딩하는 함수.
 * - GetDataTableRowByTag(): 데이터 테이블에서 태그를 기준으로 데이터를 가져오는 템플릿 함수.
 *
 * @see UAuraWidgetController
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	// 초기 속성 값을 브로드캐스트하는 함수
	virtual void BroadcastInitialValues() override;

	// 속성 변화에 따라 콜백을 바인딩하는 함수
	virtual void BindCallbacksToDependencies() override;

	// 플레이어의 속성 변화 이벤트 (체력, 마나 등)
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
	// 메시지 위젯 관련 데이터를 저장하는 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	// 능력 정보 데이터를 저장하는 객체
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// 태그를 기준으로 데이터 테이블의 행을 가져오는 템플릿 함수
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent);
};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));	
}

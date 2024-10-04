

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
 * UAttributeMenuWidgetController 클래스
 *
 * 이 클래스는 언리얼 엔진의 위젯 컨트롤러를 상속받아 애트리뷰트 메뉴 위젯과 관련된 UI 요소들을 관리하는 역할을 담당.
 * 주로 플레이어의 메인 특성(힘, 지능, 체력, 방어력) 및 서브 특성(관통력, 크리티컬 등)과 같은 UI 컴포넌트를 갱신하고, 이를 블루프린트에서 쉽게 사용할 수 있도록 함.
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

	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

};

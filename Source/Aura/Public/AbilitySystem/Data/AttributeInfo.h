

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * UAttributeInfo 클래스
 *
 * 이 클래스는 언리얼 엔진의 Data Asset을 상속받아 캐릭터의 속성(Attribute) 정보와 관련된 데이터를 관리하는 역할을 담당.
 * 각 속성에 대한 태그, 이름, 설명, 값 등을 정의하며, 이를 통해 속성 UI 및 기타 정보 처리를 지원.
 *
 * 주요 역할:
 * - 캐릭터 속성(Attribute)에 대한 정보(태그, 이름, 설명, 값 등)를 관리.
 * - 특정 속성 태그를 기반으로 해당 속성 정보를 검색하는 기능을 제공.
 *
 * 제공 기능:
 * - `AttributeInformation`: 여러 속성 정보를 저장하는 배열.
 * - `FindAttributeInfoForTag`: 주어진 속성 태그를 기반으로 해당 속성 정보를 검색하는 함수.
 *
 * 제공 메서드:
 * - `FindAttributeInfoForTag()`: 속성 태그를 입력받아 해당 속성 정보를 반환. 필요 시, 찾을 수 없는 경우 로그를 출력할 수 있는 옵션 제공.
 *
 * @see UDataAsset
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};

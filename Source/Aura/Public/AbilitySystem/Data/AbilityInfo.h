

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

/**
 * UAbilityInfo 클래스
 *
 * 이 클래스는 언리얼 엔진의 Data Asset을 상속받아 능력(Ability) 정보와 관련된 데이터를 관리하는 역할을 담당.
 * 각 능력에 대한 태그, 아이콘, 입력 태그 등의 정보를 정의하며, 이를 통해 능력 UI 및 기타 정보 처리를 지원.
 *
 * 주요 역할:
 * - 능력(Ability)에 대한 정보(태그, 아이콘, 배경 소재 등)를 관리.
 * - 특정 능력 태그를 기반으로 해당 능력 정보를 검색하는 기능을 제공.
 *
 * 제공 기능:
 * - `AbilityInformation`: 여러 능력의 정보를 저장하는 배열.
 * - `FindAbilityInfoForTag`: 주어진 능력 태그를 기반으로 해당 능력 정보를 검색하는 함수.
 *
 * 제공 메서드:
 * - `FindAbilityInfoForTag()`: 능력 태그를 입력받아 해당 능력 정보를 반환. 필요 시, 찾을 수 없는 경우 로그를 출력할 수 있는 옵션 제공.
 *
 * @see UDataAsset
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;

	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};



#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};

/**
 * UCharacterClassInfo 클래스
 *
 * 이 클래스는 언리얼 엔진의 Data Asset을 기반으로 하여 캐릭터 클래스와 관련된 기본 정보 및 속성들을 관리하는 역할을 담당.
 * 각 캐릭터 클래스(예: Elementalist, Warrior, Ranger)의 기본 속성 정보, 공통 속성 정보 및 게임 내 능력(Ability)과 관련된 데이터를 저장.
 *
 * 주요 역할:
 * - 캐릭터 클래스별 기본 속성(Primary Attributes)을 관리.
 * - 공통 속성(Secondary Attributes, Vital Attributes) 및 공통 능력(Common Abilities)을 정의.
 * - 데미지 계산에 사용되는 커브 테이블을 관리하여 각 클래스의 데미지 계산을 보조.
 *
 * 제공 기능:
 * - `CharacterClassInformation`: 각 캐릭터 클래스에 대한 기본 정보를 담고 있는 맵.
 * - `SecondaryAttributes`: 모든 캐릭터 클래스에 공통적으로 적용되는 보조 속성에 대한 GameplayEffect 정의.
 * - `VitalAttributes`: 모든 캐릭터 클래스에 적용되는 주요 속성(예: 체력, 마나)에 대한 GameplayEffect 정의.
 * - `CommonAbilities`: 모든 캐릭터가 사용할 수 있는 공통 능력을 정의하는 배열.
 * - `DamageCalculationCoefficients`: 데미지 계산에 필요한 곡선 데이터를 담은 커브 테이블.
 *
 * 제공 메서드:
 * - `GetClassDefaultInfo`: 캐릭터 클래스에 대한 기본 정보를 반환하는 함수.
 *
 * @see UDataAsset
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults | Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};

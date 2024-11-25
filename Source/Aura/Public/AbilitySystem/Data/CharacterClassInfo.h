

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * UCharacterClassInfo Ŭ����
 *
 * �� Ŭ������ �𸮾� ������ Data Asset�� ������� �Ͽ� ĳ���� Ŭ������ ���õ� �⺻ ���� �� �Ӽ����� �����ϴ� ������ ���.
 * �� ĳ���� Ŭ����(��: Elementalist, Warrior, Ranger)�� �⺻ �Ӽ� ����, ���� �Ӽ� ���� �� ���� �� �ɷ�(Ability)�� ���õ� �����͸� ����.
 *
 * �ֿ� ����:
 * - ĳ���� Ŭ������ �⺻ �Ӽ�(Primary Attributes)�� ����.
 * - ���� �Ӽ�(Secondary Attributes, Vital Attributes) �� ���� �ɷ�(Common Abilities)�� ����.
 * - ������ ��꿡 ���Ǵ� Ŀ�� ���̺��� �����Ͽ� �� Ŭ������ ������ ����� ����.
 *
 * ���� ���:
 * - `CharacterClassInformation`: �� ĳ���� Ŭ������ ���� �⺻ ������ ��� �ִ� ��.
 * - `SecondaryAttributes`: ��� ĳ���� Ŭ������ ���������� ����Ǵ� ���� �Ӽ��� ���� GameplayEffect ����.
 * - `VitalAttributes`: ��� ĳ���� Ŭ������ ����Ǵ� �ֿ� �Ӽ�(��: ü��, ����)�� ���� GameplayEffect ����.
 * - `CommonAbilities`: ��� ĳ���Ͱ� ����� �� �ִ� ���� �ɷ��� �����ϴ� �迭.
 * - `DamageCalculationCoefficients`: ������ ��꿡 �ʿ��� � �����͸� ���� Ŀ�� ���̺�.
 *
 * ���� �޼���:
 * - `GetClassDefaultInfo`: ĳ���� Ŭ������ ���� �⺻ ������ ��ȯ�ϴ� �Լ�.
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
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

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

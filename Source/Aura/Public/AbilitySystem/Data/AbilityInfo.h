

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
 * UAbilityInfo Ŭ����
 *
 * �� Ŭ������ �𸮾� ������ Data Asset�� ��ӹ޾� �ɷ�(Ability) ������ ���õ� �����͸� �����ϴ� ������ ���.
 * �� �ɷ¿� ���� �±�, ������, �Է� �±� ���� ������ �����ϸ�, �̸� ���� �ɷ� UI �� ��Ÿ ���� ó���� ����.
 *
 * �ֿ� ����:
 * - �ɷ�(Ability)�� ���� ����(�±�, ������, ��� ���� ��)�� ����.
 * - Ư�� �ɷ� �±׸� ������� �ش� �ɷ� ������ �˻��ϴ� ����� ����.
 *
 * ���� ���:
 * - `AbilityInformation`: ���� �ɷ��� ������ �����ϴ� �迭.
 * - `FindAbilityInfoForTag`: �־��� �ɷ� �±׸� ������� �ش� �ɷ� ������ �˻��ϴ� �Լ�.
 *
 * ���� �޼���:
 * - `FindAbilityInfoForTag()`: �ɷ� �±׸� �Է¹޾� �ش� �ɷ� ������ ��ȯ. �ʿ� ��, ã�� �� ���� ��� �α׸� ����� �� �ִ� �ɼ� ����.
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

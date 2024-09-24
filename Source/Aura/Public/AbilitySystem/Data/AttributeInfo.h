

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
 * UAttributeInfo Ŭ����
 *
 * �� Ŭ������ �𸮾� ������ Data Asset�� ��ӹ޾� ĳ������ �Ӽ�(Attribute) ������ ���õ� �����͸� �����ϴ� ������ ���.
 * �� �Ӽ��� ���� �±�, �̸�, ����, �� ���� �����ϸ�, �̸� ���� �Ӽ� UI �� ��Ÿ ���� ó���� ����.
 *
 * �ֿ� ����:
 * - ĳ���� �Ӽ�(Attribute)�� ���� ����(�±�, �̸�, ����, �� ��)�� ����.
 * - Ư�� �Ӽ� �±׸� ������� �ش� �Ӽ� ������ �˻��ϴ� ����� ����.
 *
 * ���� ���:
 * - `AttributeInformation`: ���� �Ӽ� ������ �����ϴ� �迭.
 * - `FindAttributeInfoForTag`: �־��� �Ӽ� �±׸� ������� �ش� �Ӽ� ������ �˻��ϴ� �Լ�.
 *
 * ���� �޼���:
 * - `FindAttributeInfoForTag()`: �Ӽ� �±׸� �Է¹޾� �ش� �Ӽ� ������ ��ȯ. �ʿ� ��, ã�� �� ���� ��� �α׸� ����� �� �ִ� �ɼ� ����.
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

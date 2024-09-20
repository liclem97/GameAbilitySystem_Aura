

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * UAuraSummonAbility Ŭ����
 *
 * �� Ŭ������ Ư�� ����(�̴Ͼ� ��)�� ��ȯ�ϴ� ����� ����ϴ� Ability Ŭ����.
 * ��ȯ�� �̴Ͼ��� ��, �̴Ͼ� Ŭ����, ���� ��ġ�� �ּ�/�ִ� �Ÿ�, ��ȯ ����(Spread) ���� ������ �� ����.
 *
 * �ֿ� ���:
 * - ��ȯ�� �̴Ͼ��� ���� ���� (NumMinions)
 * - ��ȯ�� �̴Ͼ��� Ŭ������ �迭�� ���� (MinionClasses)
 * - ��ȯ ��ġ�� �ּ�/�ִ� �Ÿ��� ���� (MinSpawnDistance, MaxSpawnDistance)
 * - ��ȯ ��ġ�� Ȯ�� ����(Spread)�� ���� (SpawnSpread)
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
};

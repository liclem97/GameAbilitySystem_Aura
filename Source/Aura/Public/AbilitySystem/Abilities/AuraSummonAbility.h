

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * UAuraSummonAbility 클래스
 *
 * 이 클래스는 특정 액터(미니언 등)를 소환하는 기능을 담당하는 Ability 클래스.
 * 소환할 미니언의 수, 미니언 클래스, 스폰 위치의 최소/최대 거리, 소환 각도(Spread) 등을 설정할 수 있음.
 *
 * 주요 기능:
 * - 소환할 미니언의 수를 설정 (NumMinions)
 * - 소환할 미니언의 클래스를 배열로 설정 (MinionClasses)
 * - 소환 위치의 최소/최대 거리를 설정 (MinSpawnDistance, MaxSpawnDistance)
 * - 소환 위치의 확산 각도(Spread)를 설정 (SpawnSpread)
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

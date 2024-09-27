

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 * ULevelUpInfo 클래스
 *
 * 이 클래스는 캐릭터 레벨업 정보를 관리하는 Data Asset.
 * 각 레벨에 필요한 경험치 요구사항과 레벨업 시 획득하는 속성 포인트 및 스펠 포인트에 대한 정보를 제공.
 *
 * 주요 역할:
 * - 레벨업에 필요한 정보를 저장하고 관리.
 * - 주어진 경험치를 기반으로 캐릭터의 현재 레벨을 찾는 기능을 제공.
 *
 * @see UDataAsset
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(int32 XP);

};

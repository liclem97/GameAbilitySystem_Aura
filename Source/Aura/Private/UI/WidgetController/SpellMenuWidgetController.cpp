


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints) 
		{
			SpellPointsChanged.Broadcast(SpellPoints);
		});
}

// 선택된 능력 태그의 상태를 확인하고,
// 해당 태그에 맞춰 버튼 활성화 여부를 결정한 후 델리게이트를 통해 브로드캐스트하는 함수.
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{	
	// 현재 선택된 AbilityTag에 따라 능력 상태를 결정.
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	// 태그가 유효하지 않거나, 'None'이거나, 해당하는 능력 사양이 없으면 잠금 상태로 설정.
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{	
		// 능력 사양에서 상태를 가져옴.
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	// 버튼 활성화 여부 결정.
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnabledButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);

	// 선택된 글로브의 버튼 상태를 브로드캐스트.
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

// 능력 상태와 스펠 포인트에 따라 장비 및 스펠 포인트 버튼을 활성화할지 결정하는 함수.
void USpellMenuWidgetController::ShouldEnabledButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnabledSpellPointsButton, bool& bShouldEnabledEquipButton)
{	
	// 능력 상태와 스펠 포인트에 따라 버튼을 활성화할지 여부를 결정.
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnabledSpellPointsButton = false;
	bShouldEnabledEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped)) // 능력이 장착된 경우
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible)) // 능력이 사용 가능 상태인 경우
	{
		if (SpellPoints > 0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked)) // 능력이 잠금 해제된 경우
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
}

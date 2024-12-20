


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
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{	
			// 어빌리티 버튼 클릭 중 스탯이 변경되면 버튼이 바로 활성화.
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				// 버튼 활성화 여부 결정.
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnabledButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

				// 선택된 글로브의 버튼 상태를 브로드캐스트.
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints) 
		{
			SpellPointsChanged.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;

			// 버튼 활성화 여부 결정.
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnabledButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			// 선택된 글로브의 버튼 상태를 브로드캐스트.
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		});
}

// 선택된 능력 태그의 상태를 확인하고,
// 해당 태그에 맞춰 버튼 활성화 여부를 결정한 후 델리게이트를 통해 브로드캐스트하는 함수.
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{	
	if (bWaitingForEquipSelection)
	{
		// 셀렉팅 애니메이션 멈추기 델리게이트
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}	

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
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	// 버튼 활성화 여부 결정.
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnabledButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);

	// 선택된 글로브의 버튼 상태를 브로드캐스트.
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{	
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{	
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

// 장비 버튼이 눌렸을 때 선택된 능력의 상태와 타입을 처리하고, 선택된 능력이 이미 장착된 경우 슬롯을 설정
void USpellMenuWidgetController::EquipButtonPressed()
{	
	// 선택된 능력의 타입을 가져옴.
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	// 장착 대기 상태를 알리는 델리게이트 브로드캐스트.
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	// 선택된 능력이 이미 장착되어 있는지 확인하고, 장착된 슬롯을 설정.
	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{	
	// 장비 대기 상태가 아니면 함수 종료.
	if (!bWaitingForEquipSelection) return;

	// 선택된 능력의 타입이 슬롯의 타입과 일치하는지 확인.
	// (공격 스킬을 패시브 슬롯에 장착하거나 그 반대의 경우를 방지)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	// 서버에 선택된 능력을 해당 슬롯에 장착하도록 요청.
	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// 이전 슬롯을 클리어.
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// 이전 슬롯이 유효한 경우 빈 정보 브로드캐스트. (이미 장착된 스펠을 다시 장착할 때만)
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// 새로운 슬롯에 능력 정보 설정.
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	// 장비 대기 상태 종료를 알리는 델리게이트 브로드캐스트.
	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
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




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
			// �����Ƽ ��ư Ŭ�� �� ������ ����Ǹ� ��ư�� �ٷ� Ȱ��ȭ.
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				// ��ư Ȱ��ȭ ���� ����.
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnabledButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

				// ���õ� �۷κ��� ��ư ���¸� ��ε�ĳ��Ʈ.
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

			// ��ư Ȱ��ȭ ���� ����.
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnabledButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			// ���õ� �۷κ��� ��ư ���¸� ��ε�ĳ��Ʈ.
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		});
}

// ���õ� �ɷ� �±��� ���¸� Ȯ���ϰ�,
// �ش� �±׿� ���� ��ư Ȱ��ȭ ���θ� ������ �� ��������Ʈ�� ���� ��ε�ĳ��Ʈ�ϴ� �Լ�.
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{	
	if (bWaitingForEquipSelection)
	{
		// ������ �ִϸ��̼� ���߱� ��������Ʈ
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}	

	// ���� ���õ� AbilityTag�� ���� �ɷ� ���¸� ����.
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	// �±װ� ��ȿ���� �ʰų�, 'None'�̰ų�, �ش��ϴ� �ɷ� ����� ������ ��� ���·� ����.
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
		// �ɷ� ��翡�� ���¸� ������.
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	// ��ư Ȱ��ȭ ���� ����.
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnabledButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);

	// ���õ� �۷κ��� ��ư ���¸� ��ε�ĳ��Ʈ.
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

// ��� ��ư�� ������ �� ���õ� �ɷ��� ���¿� Ÿ���� ó���ϰ�, ���õ� �ɷ��� �̹� ������ ��� ������ ����
void USpellMenuWidgetController::EquipButtonPressed()
{	
	// ���õ� �ɷ��� Ÿ���� ������.
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	// ���� ��� ���¸� �˸��� ��������Ʈ ��ε�ĳ��Ʈ.
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	// ���õ� �ɷ��� �̹� �����Ǿ� �ִ��� Ȯ���ϰ�, ������ ������ ����.
	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{	
	// ��� ��� ���°� �ƴϸ� �Լ� ����.
	if (!bWaitingForEquipSelection) return;

	// ���õ� �ɷ��� Ÿ���� ������ Ÿ�԰� ��ġ�ϴ��� Ȯ��.
	// (���� ��ų�� �нú� ���Կ� �����ϰų� �� �ݴ��� ��츦 ����)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	// ������ ���õ� �ɷ��� �ش� ���Կ� �����ϵ��� ��û.
	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// ���� ������ Ŭ����.
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// ���� ������ ��ȿ�� ��� �� ���� ��ε�ĳ��Ʈ. (�̹� ������ ������ �ٽ� ������ ����)
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// ���ο� ���Կ� �ɷ� ���� ����.
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	// ��� ��� ���� ���Ḧ �˸��� ��������Ʈ ��ε�ĳ��Ʈ.
	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

// �ɷ� ���¿� ���� ����Ʈ�� ���� ��� �� ���� ����Ʈ ��ư�� Ȱ��ȭ���� �����ϴ� �Լ�.
void USpellMenuWidgetController::ShouldEnabledButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnabledSpellPointsButton, bool& bShouldEnabledEquipButton)
{	
	// �ɷ� ���¿� ���� ����Ʈ�� ���� ��ư�� Ȱ��ȭ���� ���θ� ����.
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnabledSpellPointsButton = false;
	bShouldEnabledEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped)) // �ɷ��� ������ ���
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible)) // �ɷ��� ��� ���� ������ ���
	{
		if (SpellPoints > 0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked)) // �ɷ��� ��� ������ ���
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
}

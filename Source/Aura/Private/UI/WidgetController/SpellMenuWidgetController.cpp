


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

// ���õ� �ɷ� �±��� ���¸� Ȯ���ϰ�,
// �ش� �±׿� ���� ��ư Ȱ��ȭ ���θ� ������ �� ��������Ʈ�� ���� ��ε�ĳ��Ʈ�ϴ� �Լ�.
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{	
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

	// ��ư Ȱ��ȭ ���� ����.
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnabledButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);

	// ���õ� �۷κ��� ��ư ���¸� ��ε�ĳ��Ʈ.
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
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




#include "AbilitySystem/ASyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{	
	// ���ο� UWaitCooldownChange ��ü ����
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// �ɷ� �ý��� ������Ʈ �Ǵ� ��ٿ� �±װ� ��ȿ���� ������ �۾� ����
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// ��ٿ� �±װ� �߰��ǰų� ������ �� �̺�Ʈ ��� (��ٿ��� ���� �������� ����)
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged);

	// ��ٿ� ����Ʈ�� �߰��� �� �̺�Ʈ ���
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{	
	// �ɷ� �ý��� ������Ʈ�� ��ȿ���� ������ ����
	if (!IsValid(ASC)) return;

	// ��ϵ� ��ٿ� �±� �̺�Ʈ�� ����
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	// ��ü �Ҹ� �غ�
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{	
	// ��ٿ� �±��� ī��Ʈ�� 0�̸� ��ٿ� ���� �̺�Ʈ �߻�
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{	
	// ��ٿ� ���� �±׸� ����
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	// ��ٿ� �±װ� ����Ǿ����� Ȯ��
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{	
		// ��ٿ� ȿ���� �˻��ϰ� ���� �ð��� ���
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{	
			// ���� �� ��ٿ� �ð��� ã��
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}

			// ��ٿ� ���� �̺�Ʈ �߻�
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
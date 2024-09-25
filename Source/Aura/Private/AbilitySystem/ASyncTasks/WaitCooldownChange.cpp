


#include "AbilitySystem/ASyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{	
	// 새로운 UWaitCooldownChange 객체 생성
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// 능력 시스템 컴포넌트 또는 쿨다운 태그가 유효하지 않으면 작업 종료
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// 쿨다운 태그가 추가되거나 삭제될 때 이벤트 등록 (쿨다운이 언제 끝나는지 감지)
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged);

	// 쿨다운 이펙트가 추가될 때 이벤트 등록
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{	
	// 능력 시스템 컴포넌트가 유효하지 않으면 종료
	if (!IsValid(ASC)) return;

	// 등록된 쿨다운 태그 이벤트를 제거
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	// 객체 소멸 준비
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{	
	// 쿨다운 태그의 카운트가 0이면 쿨다운 종료 이벤트 발생
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{	
	// 쿨다운 관련 태그를 감지
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	// 쿨다운 태그가 적용되었는지 확인
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{	
		// 쿨다운 효과를 검색하고 남은 시간을 계산
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{	
			// 가장 긴 쿨다운 시간을 찾음
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}

			// 쿨다운 시작 이벤트 발생
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
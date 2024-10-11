


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{	
		// ���� Ability Ŭ������ ����� Level 1�� �����Ͽ� FGameplayAbilitySpec ��ü�� ����.
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}				
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{	
			// �ش� �ɷ��� �Է��� ���� ���·� ó���˴ϴ�.
			AbilitySpecInputPressed(AbilitySpec);

			// �ɷ��� ���� Ȱ��ȭ���� �ʾҴٸ�, Ȱ��ȭ�� �õ��մϴ�.
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{	
			// �ش� �ɷ��� �Է��� ������ ���·� ó���˴ϴ�.
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

// �� Ȱ��ȭ ������ �ɷ¿� ���� ������ ��������Ʈ�� �����ϴ� �Լ�.
// ���� ��������Ʈ ���࿡ �����ϸ� ���� �α׸� ���.
void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{	
	// Ȱ��ȭ ������ �ɷ� ����� �����ϰ� ��װ� �����ϱ� ���� ���� ���.
	FScopedAbilityListLock ActiveScopeLock(*this);

	// �� �ɷ¿� ���� ��������Ʈ�� ����.
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{	
		// ��������Ʈ�� ���ε��Ǿ� �ִٸ� ����. ���� �� �α� ���.
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

// �־��� �ɷ� ��翡�� "Abilities" �±׸� ��ȯ�ϴ� �Լ�.
// �ɷ� ��翡 ���� �±� �� "Abilities" �±װ� �ִ��� Ȯ���Ͽ� ��ȯ.
FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{	
	// �ɷ� ��翡 �ɷ��� �����ϴ��� Ȯ��.
	if (AbilitySpec.Ability)
	{	
		// �ɷ� �±� ����� ��ȸ�ϸ� "Abilities" �±׸� ã��.
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag; // "Abilities" �±׸� ã���� ��ȯ.
			}
		}
	}
	return FGameplayTag();
}

// �־��� �ɷ� ��翡�� "InputTag" �±׸� ��ȯ�ϴ� �Լ�.
// ���� �ɷ� �±� ��Ͽ��� "InputTag" �±׸� Ȯ���Ͽ� ��ȯ.
FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{	
	// ���� �ɷ� �±� ����� ��ȸ�ϸ� "InputTag" �±׸� ã��.
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag; // "InputTag" �±׸� ã���� ��ȯ.
		}
	}
	return FGameplayTag();
}

// �־��� �ɷ� ��翡�� "Abilities.Status" �±׸� ��ȯ�ϴ� �Լ�.
// ���� �ɷ� �±� ��Ͽ��� "Abilities.Status" �±׸� Ȯ���Ͽ� ��ȯ.
FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{	
	// ���� �ɷ� �±� ����� ��ȸ�ϸ� "Abilities.Status" �±׸� ã��.
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttributes(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttributes_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	// ���� �ƹ�Ÿ�� �ɷ� ����(AbilityInfo)�� ������.
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	// �ɷ� ���� �迭�� ��ȸ�ϸ鼭 �� �ɷ��� ���¸� ������Ʈ.
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;	// �ɷ� �±װ� ��ȿ���� ������ �ǳʶ�.
		if (Level < Info.LevelRequirement) continue;	// �÷��̾� ������ �䱸 �������� ������ �ǳʶ�.
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)	// �ش� �±��� �ɷ��� ������ ���� �߰�.
		{
			// �ɷ� ����� ����� ���� �±׸� �߰�.
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);

			// �ɷ��� �ο��ϰ� ����� ������(����).
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible);
		}
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}	
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveffectHandle)
{	
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

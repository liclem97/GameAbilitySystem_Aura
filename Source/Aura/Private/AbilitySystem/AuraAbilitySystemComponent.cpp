


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
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
			GiveAbility(AbilitySpec);
		}				
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
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

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}	
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveffectHandle)
{	
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

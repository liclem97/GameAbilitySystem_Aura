


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AuraLogChannels.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{	
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	/* Resistance Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);

}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);	
}

// ������ Attribute ���� �ٲ�� �� �����ϴ� �Լ�.
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

//�־��� ȿ�� �����ͷκ��� �ҽ��� Ÿ�ٿ� ���� ������ ����ü 'FEffectProperties'�� �����Ѵ�.
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = effect�� ����, Target = effect�� ���

	// 1. EffectSpec���� Context�� ������ EffectContextHandle�� ����
	Props.EffectContextHandle = Data.EffectSpec.GetContext();

	// 2. Context���� OriginalInstigator�� ASC�� ������ SourceASC�� ����
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// 3. SourceASC�� ��ȿ�� ���, �ҽ��� ���õ� �Ӽ����� ����
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// �ҽ� �ƹ�Ÿ ���� ����
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();

		// �ҽ� ��Ʈ�ѷ� ����
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		// �ҽ� ��Ʈ�ѷ��� ����, �ҽ� �ƹ�Ÿ ���Ͱ� ��ȿ�� ���
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{	
				// �ҽ� �ƹ�Ÿ ���Ͱ� Pawn�� ���, ��Ʈ�ѷ��� ������
				Props.SourceController = Pawn->GetController();
			}
		}
		// �ҽ� ĳ���� ���� (��Ʈ�ѷ��� ��ȿ�� ���)
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	// 4. Target�� AbilityActorInfo�� ��ȿ�� ���, Ÿ���� ���õ� �Ӽ����� ����
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{	
		// Ÿ�� �ƹ�Ÿ ���� ����
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

		// Ÿ�� ��Ʈ�ѷ� ����
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

		// Ÿ�� ĳ���� ����
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);

		// Ÿ���� ASC�� ������ ����
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}

}

// GameplayEffect�� ���������� ����Ǳ� �� ȣ��
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// Health�� Mana�� ����� �� ����.
	// GameplayEffec�� ����� �� Health�� Mana�� Max�� ���� �ʵ��� ����.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	// �������� ���� �� ����.
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{	
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}	
				SendXPEvent(Props);
			}
			else // ���� �ʾ��� �� ����.
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);

				// Ÿ�� ASC�� Effect_HitReact�� ������ Gameplay Ability Ȱ��ȭ. GA_HitReact
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			
			const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticaldHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		}
	}

	// ����ġ�� ���� �� ����.
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);

		//TODO : �������� �ؾ��� ��
		if (Props.SourceCharacter->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
		}		
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{	
	// Ÿ�� ĳ���Ͱ� CombatInterface�� �����ϰ� �ִ��� Ȯ���ϰ� ĳ����.
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetCharacter))
	{	
		// Ÿ�� ĳ������ ������ Ŭ������ ������.
		const int32 TargetLevel = CombatInterface->GetPlayerLevel();
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);

		// Ÿ�� ĳ������ ������ ������� XPReward�� ������.
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);
		
		// GameplayTags���� XP ���� �±׸� ������.
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

		// Gameplay Event�� ���� ������(Payload)�� ����.
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;

		// �ҽ� ĳ���Ϳ��� XP �̺�Ʈ ����.
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	// ������ != ������
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		{
			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
			{
				// ���� ���� ������ ��.
				PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
				return;
			}
			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
			{	
				// ���� ���� ������ ��.
				PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			}
		}
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

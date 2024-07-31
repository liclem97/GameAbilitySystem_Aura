


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(25.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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

// GameplayEffect�� ���������� ����� �Ŀ� ȣ��
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}



#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

// MMC_Max~ Ŭ���� �������� ��ũ�� ���.
struct AuraDamageStatics
{	
	// �ֿ� �Ӽ������� ĸó�ϱ� ���� ����. ������ �Ӽ��� ĸó�� �Ӽ��� �̸��� ������ ������.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	AuraDamageStatics()
	{	
		// �� �Ӽ��� ���� ���ǵ� ���� ĸó�� �غ��Ѵ�.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);	

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

// ������: ���� ��꿡�� ����� �Ӽ������� �����մϴ�.
UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

// �ֿ� ���� ��� �Լ�: �����÷��� ȿ���� ������� ��� ����� �������� ���.
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutPut) const
{	
	// �±׿� �Ӽ� ĸó ���Ǹ� ����.
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	// �������� ���� ���� �±� ����.
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);

	// �ҽ��� Ÿ���� AbilitySystemComponent �� �ƹ�Ÿ ���� ��������.
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	// �ҽ��� Ÿ���� �÷��̾� ���� Ȯ��.
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	// GameplayEffect�� ����� �±׵��� ������
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvalutationParamters;
	EvalutationParamters.SourceTags = SourceTag;
	EvalutationParamters.TargetTags = TargetTag;

	// ����� ó��.
	DetermineDebuff(ExecutionParams, Spec, EvalutationParamters, TagsToCaptureDefs);

	// ������ ���.
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{	
		// �������� ���� �±� ���� �� �Ӽ� ��������.
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(TagsToCaptureDefs.Contains(ResistanceTag),
			TEXT("TagsToCaptureDefs doesn't contain Tag : [%s] in ExecCalc_Damage."), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		// SetByCaller�� ������ ������ Ÿ���� ���� ������
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}

		// Ÿ���� ���� �Ӽ� ���� ĸó�ϰ� ���� ���� 0~100 ���̷� Ŭ����
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvalutationParamters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		// ���׿� ���� ������ ����
		DamageTypeValue *= (100.f - Resistance) / 100.f;

		// ����� ������ ��� ����
		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount) 
					{
						DamageTypeValue = DamageAmount;
					});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}

		Damage += DamageTypeValue;
	}

	// ��� Ȯ�� Ȯ�� �� ó��.
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvalutationParamters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	// ��� ���θ� EffectContextHandle�� ����
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	// ����� �������� ��� ������ �������� ����
	Damage = bBlocked ? Damage / 2.f : Damage;

	// ���� �� ���� ó��.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvalutationParamters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvalutationParamters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	// DA_CharacterClassInfo�� �ִ� DamageCalculationCoefficients Ŀ�����̺��� �����ͼ� �� ������ �´� ����� �����.
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	// ArmorPenetration�� Ÿ���� Armor�� ���� �ۼ�Ʈ�� ����.
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	// Armor�� ������ �������� ���� �ۼ�Ʈ�� ����.
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	// �ҽ��� CriticalHitChance, CriticalHitDamage�� Ÿ���� CriticalHitResistance ���� ������
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvalutationParamters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvalutationParamters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvalutationParamters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	// ũ��Ƽ�� ���׿� ���� ũ��Ƽ�� ��Ʈ Ȯ�� ����
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	
	// ũ��Ƽ�� ��Ʈ ���� ����
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// ũ��Ƽ�� ��Ʈ ���� �� ������ 2�� ���� �� ũ��Ƽ�� ������ �߰�
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	// ���� ���� �������� OutExecutionOutput�� �߰�
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutPut.AddOutputModifier(EvaluatedData);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters& EvalutationParamters,const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{	
	// �����÷��� �±׸� ������.
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// �� ���� ������ �ش� ����� ������ ��ȸ.
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;

		// �ش� ���� ������ �������� ������.
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);

		// �������� ����� ũ�� ����� ���� ���θ� ����.
		if (TypeDamage > -.5f) // �ε� �Ҽ��� ���� ����
		{
			// ����� ���� Ȯ�� ���.
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

			// Ÿ���� ����� ���� ���.
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvalutationParamters, TargetDebuffResistance);
			
			// ���� �� ���� �� ����� Ȯ�� ���.
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			
			// ����� ���� ���θ� �����ϰ� ����.
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

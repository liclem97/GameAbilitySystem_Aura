#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	/** ����ȭ�� ���� ���� ������ ��ȯ�մϴ�. ���� Ŭ������ �� ������ �������ؾ� �մϴ�! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** �� ���ؽ�Ʈ�� ���纻�� �����Ͽ� ���߿� ������ �� �ֵ��� �մϴ�. */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** ����� ���� ����ȭ, ���� Ŭ������ �̸� �������ؾ� �մϴ�. */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};

/** �� ���ø��� Ư�� ����ü�� ���� ��Ʈ��ũ ����ȭ�� ���� ���� ���ο� ���� Ư������ ����մϴ�.
* 1. ��Ʈ��ũ ����ȭ ���� (WithNetSerializer = true): �� ����ü�� ��Ʈ��ũ�� ���� ����ȭ �� ������ �����մϴ�.
* 2. ���� ���� (WithCopy = true): �� ����ü�� ���� �����ڸ� ���� ������ �� �ֽ��ϴ�.
*/
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		
	};
};
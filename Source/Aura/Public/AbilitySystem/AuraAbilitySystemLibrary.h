

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UCharacterClassInfo;
/**
 * UAuraAbilitySystemLibrary Ŭ����
 * 
 * �� Ŭ������ UBlueprintFunctionLibrary�� ��ӹ޾� Aura ������Ʈ���� Ability System�� ���õ� �پ��� ����� �����ϴ� ��ƿ��Ƽ �Լ����� �����մϴ�.
 * �ַ� ���� ��Ʈ�ѷ�, ĳ���� Ŭ���� �ʱ�ȭ, GameplayEffect�� ���õ� ��ɵ��� �������Ʈ���� ���� ����� �� �ֵ��� ����� �����մϴ�.
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	/** �����Ǵ� Pawn�� ĳ���� Ŭ������ �����Ѵ�. */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsCriticaldHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// UPARAM(ref) : const�� �ƴ� �Ű������� �����Ϳ��� InPut������ �������.
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
};


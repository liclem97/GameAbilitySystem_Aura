


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{	
	check(GEngine);

	UAuraAssetManager* AuraAssetManeger = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManeger;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// 타겟 데이터를 사용하기 위해 요구됨.
	UAbilitySystemGlobals::Get().InitGlobalData();
}
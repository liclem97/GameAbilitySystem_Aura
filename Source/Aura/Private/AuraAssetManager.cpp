


#include "AuraAssetManager.h"
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
}
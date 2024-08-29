


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{	
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		// ���ÿ��� ���� ���̶�� ���콺 Ŀ�� �����͸� ������ �����Ѵ�.
		SendMouseCursorData();
	}
	else // �������� ���� ���� ��
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		// Ŭ���̾�Ʈ -> ������ Ÿ�� �����Ͱ� ������ �� ȣ��� ��������Ʈ ���ε�
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		// Ÿ�� �����Ͱ� �̹� �����Ǿ��ִ��� Ȯ��, ������ �����Ͱ� �ִٸ� ��������Ʈ ��� ȣ��
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{	
			// Ÿ�� �����Ͱ� ���� �������� �ʾҴٸ�, ���� �÷��̾��� �����͸� ��ٸ��� ���·� ��ȯ.
			SetWaitingOnRemotePlayerData();
		}
	}
}

/* Ŭ���̾�Ʈ�� ��� Ÿ�� �����͸� ������ ������. */
void UTargetDataUnderMouse::SendMouseCursorData()
{	
	// ���� ������ ������ �����Ͽ� ������ Ŭ���̾�Ʈ ���� ������ ����ȭ ������ �����Ѵ�.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// ���� �÷��̾� ��Ʈ�ѷ��� ������.
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	// ���콺 Ŀ�� �Ʒ��� �浹 ������ �����´�.
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// Ÿ�� �����͸� �����ϱ� ���� �ڵ� �� ������ ��ü�� ����.
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit; // ���콺 Ŀ�� ��ġ�� ��Ʈ ����� ����.
	DataHandle.Add(Data);  // �ڵ鿡 �����͸� �߰�.

	// ������ ������ Ÿ�� �����͸� �����ϵ��� ��û.
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),          // ���� AbilitySpec �ڵ�
		GetActivationPredictionKey(),    // Ȱ��ȭ ���� Ű
		DataHandle,                      // Ÿ�� ������ �ڵ�
		FGameplayTag(),                  // �߰����� �±� (�� �±� ����)
		AbilitySystemComponent->ScopedPredictionKey);  // ���� Ű�� ������ ����

	// AbilityTaskDelegates�� ��ε�ĳ��Ʈ�ؾ� �ϴ� ��� ��ȿ�� �����͸� ��ε�ĳ��Ʈ.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{	
	// Ŭ���̾�Ʈ���� ������ Ÿ�� �����͸� �Һ��Ѵ�.
	// Ŭ���̾�Ʈ�� ������ ���� Ÿ�� �����͸� �������� ó���ϰ�, �� �̻� ������ �ʵ��� �����Ѵ�.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}




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
		// 로컬에서 제어 중이라면 마우스 커서 데이터를 서버로 전송한다.
		SendMouseCursorData();
	}
	else
	{
		// TODO : We are on the server, so listen for target data.
	}


}

/* 클라이언트인 경우 타겟 데이터를 서버로 보낸다. */
void UTargetDataUnderMouse::SendMouseCursorData()
{	
	// 예측 윈도우 범위를 설정하여 서버와 클라이언트 간의 데이터 동기화 문제를 방지한다.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// 현재 플레이어 컨트롤러를 가져옴.
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	// 마우스 커서 아래의 충돌 정보를 가져온다.
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// 타겟 데이터를 설정하기 위한 핸들 및 데이터 객체를 생성.
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit; // 마우스 커서 위치의 히트 결과를 저장.
	DataHandle.Add(Data);  // 핸들에 데이터를 추가.

	// 서버에 복제된 타겟 데이터를 설정하도록 요청.
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),          // 현재 AbilitySpec 핸들
		GetActivationPredictionKey(),    // 활성화 예측 키
		DataHandle,                      // 타겟 데이터 핸들
		FGameplayTag(),                  // 추가적인 태그 (빈 태그 전달)
		AbilitySystemComponent->ScopedPredictionKey);  // 예측 키를 범위에 설정

	// AbilityTaskDelegates를 브로드캐스트해야 하는 경우 유효한 데이터를 브로드캐스트.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
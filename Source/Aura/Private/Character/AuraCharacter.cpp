


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;	// 캐릭터의 이동을 평면으로 제한
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// 시작할 때 캐릭터의 위치가 평면을 벗어난 상태라면 가까운 평면에 붙여서 시작

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

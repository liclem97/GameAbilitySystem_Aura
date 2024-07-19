


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;	// ĳ������ �̵��� ������� ����
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// ������ �� ĳ������ ��ġ�� ����� ��� ���¶�� ����� ��鿡 �ٿ��� ����

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

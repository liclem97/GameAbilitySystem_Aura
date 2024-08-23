

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * UAuraInputComponent Ŭ������ UEnhancedInputComponent�� ��ӹ�����,
 * Aura ���� �ý��ۿ��� �Է� �׼��� ó���ϱ� ���� �߰����� ����� �����մϴ�.
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * Ability ���� �׼��� ���ε��ϴ� ���ø� �Լ��Դϴ�.
	 *
	 * @param InputConfig - �׼ǰ� �±װ� ���ǵ� �Է� ���� ��ü�Դϴ�.
	 * @param Object - ���ε��� ��� ��ü�Դϴ�.
	 * @param PressedFunc - Ű�� ������ �� ȣ���� �Լ� �������Դϴ�.
	 * @param ReleasedFunc - Ű�� �������� �� ȣ���� �Լ� �������Դϴ�.
	 * @param HeldFunc - Ű�� ���� ���¿��� ���������� ȣ���� �Լ� �������Դϴ�.
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(
		const UAuraInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc,
		HeldFuncType HeldFunc
	);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& Action : InputConfig->AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{	
			// Ư�� Func�� ��ȿ�� ���, ETriggerEvent �̺�Ʈ�� ���ε��մϴ�.

			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}

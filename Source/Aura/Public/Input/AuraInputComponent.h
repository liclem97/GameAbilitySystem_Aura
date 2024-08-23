

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * UAuraInputComponent 클래스는 UEnhancedInputComponent를 상속받으며,
 * Aura 게임 시스템에서 입력 액션을 처리하기 위한 추가적인 기능을 제공합니다.
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * Ability 관련 액션을 바인딩하는 템플릿 함수입니다.
	 *
	 * @param InputConfig - 액션과 태그가 정의된 입력 구성 객체입니다.
	 * @param Object - 바인딩할 대상 객체입니다.
	 * @param PressedFunc - 키가 눌렸을 때 호출할 함수 포인터입니다.
	 * @param ReleasedFunc - 키가 떼어졌을 때 호출할 함수 포인터입니다.
	 * @param HeldFunc - 키가 눌린 상태에서 지속적으로 호출할 함수 포인터입니다.
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
			// 특정 Func가 유효할 경우, ETriggerEvent 이벤트에 바인딩합니다.

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

// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtentionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADE_API UPawnExtentionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<class T>
	T* GetOwningPawn() const {
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template Paramerter to GetPawn must be drived from APawn");
		return CastChecked<T>(GetOwner());
	}
	APawn* GetOwningPawn() const {
		return GetOwningPawn<APawn>();
	}
	template <class T>
	T* GetOwningController() const {
		//static_assert는 컴파일 타임에 조건을 검사하여, 그 조건이 참이 아닐 경우 컴파일 오류를 발생시키는 C++의 기능
		//TPointerIsConvertibleFromTo 템플릿 함수에서 특정 타입 T가 다른 타입 U로 안전하게 변환될 수 있는지를 확인할 때 사용
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Paramerter to GetPawn must be drived from AController");
		return GetOwningPawn()->GetController<T>();
	}


		
};

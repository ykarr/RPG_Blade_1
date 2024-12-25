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
		//static_assert�� ������ Ÿ�ӿ� ������ �˻��Ͽ�, �� ������ ���� �ƴ� ��� ������ ������ �߻���Ű�� C++�� ���
		//TPointerIsConvertibleFromTo ���ø� �Լ����� Ư�� Ÿ�� T�� �ٸ� Ÿ�� U�� �����ϰ� ��ȯ�� �� �ִ����� Ȯ���� �� ���
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Paramerter to GetPawn must be drived from AController");
		return GetOwningPawn()->GetController<T>();
	}


		
};

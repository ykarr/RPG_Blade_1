// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BladeAttributeSet.generated.h"

class IPawnUIInterface;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class BLADE_API UBladeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
    UBladeAttributeSet();
    //Gameplay Effect�� ���������� ����� �Ŀ� ȣ��Ǵ� �ݹ� �Լ��Դϴ�. �� �Լ��� Gameplay Effect�� ����� �� �� ����� ó���ϴ� �� ����.
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

    UPROPERTY(BlueprintReadOnly, Category="Attribute")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, MaxHealth);

    UPROPERTY(BlueprintReadOnly, Category="Attribute")
    FGameplayAttributeData CurrentHealth;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, CurrentHealth);

    UPROPERTY(BlueprintReadOnly, Category="Attribute")
    FGameplayAttributeData MaxRage;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, MaxRage);

    UPROPERTY(BlueprintReadOnly, Category="Attribute")
    FGameplayAttributeData CurrentRage;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, CurrentRage);

    UPROPERTY(BlueprintReadOnly, Category="Damage")
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, AttackPower);

    UPROPERTY(BlueprintReadOnly, Category="Damage")
    FGameplayAttributeData DefensePower;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, DefensePower);
    
    UPROPERTY(BlueprintReadOnly, Category="Damage")
    FGameplayAttributeData DamageTaken;
    ATTRIBUTE_ACCESSORS(UBladeAttributeSet, DamageTaken);

private:
    TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};

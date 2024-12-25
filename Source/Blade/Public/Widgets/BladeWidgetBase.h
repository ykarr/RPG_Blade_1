// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BladeWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBladeWidgetBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized()override;
	/*virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;*/
	
	UFUNCTION(BlueprintCallable, meta=(DisplayName="OnOwningPlayerUIComponentInitialized"))
	virtual void OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningUIComponent);
	UFUNCTION(BlueprintCallable, meta=(DisplayName="OnOwningPlayerUIComponentInitialized"))
	virtual void OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningUIComponent);
public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);
};

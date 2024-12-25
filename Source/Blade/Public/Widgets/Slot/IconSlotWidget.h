// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "IconSlotWidget.generated.h"

class USizeBox;
class UImage;
//class UNamedSlot;
/**
 * 
 */
UCLASS()
class BLADE_API UIconSlotWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	virtual void SetSlotImageToSoftTextureIcon(TSoftObjectPtr<UTexture2D> InTexture);
protected:
	//UPROPERTY(meta = (BindWidget))
	//UNamedSlot* BladeNamedSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> BladeSizeBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BladeImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float BladeSizeBoxWidth = 128;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float BladeSizeBoxHeight = 128;
	virtual void NativeConstruct()override;
	virtual void NativePreConstruct()override;
private:
	void SetOverrideSize(float Width, float Height);
};

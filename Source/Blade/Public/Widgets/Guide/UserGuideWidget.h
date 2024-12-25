// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "UserGuideWidget.generated.h"

class USizeBox;
class UTextBlock;
class UInputAction;
/**
 * 
 */
UCLASS()
class BLADE_API UUserGuideWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	void SetKeyGuideText(FText& GuideKey);
protected:
	virtual void NativePreConstruct()override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> BladeSizeBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float BladeSizeBoxWidth = 128;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float BladeSizeBoxHeight = 128;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GuideKeyText;

	UPROPERTY(EditDefaultsOnly, Category = "Key")
	UInputAction* Key_GuideAction;
private:
	void SetOverrideSize(float Width, float Height);
	
};

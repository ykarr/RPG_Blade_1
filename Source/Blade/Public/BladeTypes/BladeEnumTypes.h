#pragma once

UENUM(BlueprintType)
enum class EBladeConfirmType : uint8
{
	YES,
	NO
};

UENUM()
enum class EBladehValidType :uint8
{
	Valid,
	InValid
};
UENUM(BlueprintType)
enum class EBladeSuccessType :uint8
{
	Successful,
	Failed
};

UENUM() 
enum class EBladeCountDownActionInput : uint8 {
	Start,
	Cancel
};
UENUM(BlueprintType)
enum class EBladeCountDownActionOutput : uint8 {
	Updated,
	Completed,
	Cancelled
};
UENUM(BlueprintType)
enum class EBladeGameDifficulty : uint8 {
	Easy,
	Normal,
	Hard,
	VeryHard
};
UENUM()
enum class EBladeInputMode :uint8 {
	GameOnly,
	UIOnly
};
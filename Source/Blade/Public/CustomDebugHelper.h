#pragma once

namespace Debug {
	static void Print(const FString& Msg, const FColor& color = FColor::MakeRandomColor(), int32 InKey = -1) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, color, Msg, true);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& color = FColor::MakeRandomColor()) {
		if (GEngine) {
			const FString FinalMSG = FloatTitle + TEXT(" : ") + FString::SanitizeFloat(FloatValueToPrint);
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, color, FinalMSG, true);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMSG);
		}
	}
}
//'를 이용해 Ability를 확인할 수 있음.
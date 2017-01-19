// @Maxpro 2017

#pragma once

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Subtitle.h"
#include "Dialog.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialog : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/*The question's excerpt - this will be shown inside our UI*/
	UPROPERTY(EditAnywhere)
	FString QuestionExcerpt;

	/*The actual SFX that we're going to play*/
	UPROPERTY(EditAnywhere)
	USoundBase* SFX;

	/*An array of the associated subtitles*/
	UPROPERTY(EditAnywhere)
	TArray<FSubtitle> Subtitles;

	/*True if we want to wait for the AI to respond*/
	UPROPERTY(EditAnywhere)
	bool bShouldAIAnswer;
};
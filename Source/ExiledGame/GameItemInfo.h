// @Maxpro 2017

#pragma once

#include "UObject/NoExportTypes.h"
#include "GameItemInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemCategories : uint8
{
	ICE_Consumable 	UMETA(DisplayName = "Consumable"),
	ICE_Equipment 	UMETA(DisplayName = "Equipment"),
	ICE_QuestItems	UMETA(DisplayName = "Quest Items"),
	ICE_Readables	UMETA(DisplayName = "Readables")
};

USTRUCT()
struct FGameItemInfo
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UseText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategories Category;

	FGameItemInfo()
	{
		Name = FText::FromString("");
		Description = FText::FromString("");
		Icon = nullptr;
		bCanBeUsed = true;
		UseText = FText::FromString("Use");
		bCanBeStacked = false;
		Category = EItemCategories::ICE_Consumable;
	}
};
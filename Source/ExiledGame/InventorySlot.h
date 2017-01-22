// @Maxpro 2017

#pragma once

#include "UObject/NoExportTypes.h"
#include "MasterItem.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlot 
{
	GENERATED_USTRUCT_BODY()


	/** type of Item Class */
	UPROPERTY(EditDefaultsOnly, Category = "ItemSystem")
	class AMasterItem* ItemClass;


	UPROPERTY(EditAnywhere)
	int32 AmountInSlot;

	FInventorySlot()
	{
		ItemClass = ConstructObject<AMasterItem>(AMasterItem::StaticClass());
	}


	
};

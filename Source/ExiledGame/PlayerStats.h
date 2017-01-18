// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "PlayerStats.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/*The xp need for the next level*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XpToLevel;

	/*The health that the player has in the current level*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
};
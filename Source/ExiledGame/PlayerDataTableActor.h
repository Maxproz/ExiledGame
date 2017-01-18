// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PlayerDataTableActor.generated.h"

UCLASS()
class EXILEDGAME_API APlayerDataTableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerDataTableActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// The data table we are using for our player stats
	// This needs to be hooked up manually in the blueprint
	UPROPERTY(EditAnywhere)
	UDataTable* DataTable;
	
};

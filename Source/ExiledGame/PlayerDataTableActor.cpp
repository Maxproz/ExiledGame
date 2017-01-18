// Fill out your copyright notice in the Description page of Project Settings.

#include "ExiledGame.h"
#include "PlayerStats.h"
#include "PlayerDataTableActor.h"


// Sets default values
APlayerDataTableActor::APlayerDataTableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerDataTableActor::BeginPlay()
{
	Super::BeginPlay();
	

	//Retrieve the Row based on the Row Name
	//The first parameter is the row name
	//I have yet to determine the second one, however, I will open a ticket on ue4 answerhub
	FPlayerStats* Row = DataTable->FindRow<FPlayerStats>(TEXT("1"), TEXT(""));
	if (Row)
	{
		//Cast the XpToLevel to string so I can print it with GLog
		FString output = FString::FromInt((*Row).XpToLevel);
		GLog->Log(output);

		//Casting the Health to string...
		output = FString::SanitizeFloat((*Row).Health);
		GLog->Log(output);
	}

}

// Called every frame
void APlayerDataTableActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


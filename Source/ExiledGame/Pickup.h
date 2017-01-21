// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MasterItem.h"
#include "Pickup.generated.h"


UCLASS()
class EXILEDGAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Destroy the actor;
	virtual void K2_DestroyActor() override;

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OverlapTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//UFUNCTION()
	//void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//UPROPERTY(EditAnywhere)
	//class AMasterItem* ItemToAdd;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMasterItem> ItemToAdd;

	UPROPERTY(EditAnywhere)
	int32 Amount;

private:

	bool bDidItemAddSucessfully;

	int32 RestOfItem;

};

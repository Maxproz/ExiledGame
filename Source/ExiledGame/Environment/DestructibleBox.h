// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExiledGameCharacter.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Runtime/Engine/Classes/Components/DestructibleComponent.h"
#include "DestructibleBox.generated.h"

UCLASS()
class EXILEDGAME_API ADestructibleBox : public AActor
{
	GENERATED_BODY()

private:

	//-----------------------------------------------------------------------------------------
	//Timeline Properties
	//-----------------------------------------------------------------------------------------

	FTimeline LocationTimeline;

	FTimeline RotationTimeline;

	/*Stores the initial location and rotation of pickup in order to have a smooth transition via curves*/
	FVector AmmoSMInitialLocation;

	FRotator AmmoSMInitialRotation;

	/*Advances the Location timeline*/
	UFUNCTION()
	void AdvanceLocationTimeline(float Alpha);

	/*Advances the Rotation timeline*/
	UFUNCTION()
	void AdvanceRotationTimeline(float Alpha);

	/*True if the box has been destroyed*/
	bool bHasBeenDestroyed;

	/*True if the pickup is inside the pickup threshold of the character*/
	bool bShouldStopTimelines;

	/*Main character reference*/
	AExiledGameCharacter* MainCharacter;

	/*Calculates the ammo for pickup based on the Min and Max Ammo properties of the Blueprint*/
	void CalculatePickupAmmo();


public:
	// Sets default values for this actor's properties
	ADestructibleBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

protected:


	UPROPERTY(BlueprintReadOnly)
	int32 AmmoToPickup;

	UPROPERTY(VisibleAnywhere)
	UDestructibleComponent* DesctructibleSM;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	//These props are configurated so the destructible sm is going to
	//get destroyed on the first hit

	UPROPERTY(EditAnywhere, Category = "DamageValues")
	float BaseDamage;

	UPROPERTY(EditAnywhere, Category = "DamageValues")
	float DamageRadius;

	UPROPERTY(EditAnywhere, Category = "DamageValues")
	float ImpulseStrength;


	//-----------------------------------------------------------------------------------------
	//Pickup Properties
	//-----------------------------------------------------------------------------------------

	/*The minimum ammo that can be picked up*/
	UPROPERTY(EditAnywhere, Category = "Pickup")
	int32 MinPickupAmmo;

	/*The maximum ammo that can be picked up*/
	UPROPERTY(EditAnywhere, Category = "Pickup")
	int32 MaxPickupAmmo;

	//-----------------------------------------------------------------------------------------
	//Timelines
	//-----------------------------------------------------------------------------------------

	/*The location float curve*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	UCurveFloat* LocationCurve;

	/*The rotation float curve*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	UCurveFloat* RotationCurve;

	/*The end rotation of the pickup's sm*/
	UPROPERTY(EditAnywhere, Category = "Pickup")
	FRotator EndRotation;

	/*The time in seconds after the pickup has reached the character and this actor will be destroyed*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float DestructionTime;

	/*The static mesh of the ammo pickup*/
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AmmoSM;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
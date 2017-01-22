// Fill out your copyright notice in the Description page of Project Settings.

#include "ExiledGame.h"
#include "ExiledGameCharacter.h"
#include "Inventory.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetSphereRadius(90.f);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OverlapTriggered);
	//Sphere->OnComponentEndOverlap.AddDynamic(this, &APickup::OverlapEnd);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickup::K2_DestroyActor()
{
	Super::K2_DestroyActor();

	
}

void APickup::OverlapTriggered(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AExiledGameCharacter* Char = Cast<AExiledGameCharacter>(OtherActor);

		AMasterItem* Item = ConstructObject<AMasterItem>(AMasterItem::StaticClass());

		if (Char->Inventory)
		{
			if (Item == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, "You need to assign an item to add", true);
				return;
			}
			else
			{
				
				Char->Inventory->AddItem(Item, Amount, bDidItemAddSucessfully, RestOfItem);

				// TODO: For some reason this code is not working
				if (bDidItemAddSucessfully == true)
				{
					if (RestOfItem > 0)
					{
						Amount = RestOfItem;
					}
					else
					{
					    K2_DestroyActor();
					}
				}
			}
			
		}
	}
}

//void APickup::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{

//}




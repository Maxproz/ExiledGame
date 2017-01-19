// @Maxpro 2017

#pragma once

#include "GameFramework/Actor.h"
#include "GameItemInfo.h"
#include "MasterItem.generated.h"

UCLASS()
class EXILEDGAME_API AMasterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameItemInfo ItemInfo;

	/*Adds the widget to our viewport and populates the buttons with the given questions*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Items")
	void EventOnUsed();
	
};

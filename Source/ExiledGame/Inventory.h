// @Maxpro 2017

#pragma once

#include "GameFramework/Actor.h"
#include "ExiledGameCharacter.h"
#include "InventorySlot.h"
#include "GameItemInfo.h"
#include "MasterItem.h"
#include "Inventory.generated.h"

UCLASS()
class EXILEDGAME_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	/* NEW BEGIN */
private:

	int32 MaxStackSize;

	TArray<FInventorySlot> Slots;


	void IsSlotEmpty(int32 Index, bool &IsEmpty) const;

	void GetItemInfoAtIndex(int32 Index, bool &IsEmptySlot, FGameItemInfo &ItemInfo, int32 &Amount) const;

	bool IsValidSlotClass(FInventorySlot InvSlot) const;

	void SearchEmptySlot(bool &Success, int32 &Index);

	void SearchFreeStack(const AMasterItem *ItemClass, bool &Success, int32 &Index);

	void AddItem(const AMasterItem *ItemClass, const int32 Amount, bool &Success);

public:


	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 AmountOfSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	AExiledGameCharacter* TopDownCharacterRef;

	TArray<FInventorySlot> GetSlots() const { return Slots; }
	
	FInventorySlot GetSlotAtIndex(int32 Index) const { return Slots[Index]; }

	/* NEW END */
};

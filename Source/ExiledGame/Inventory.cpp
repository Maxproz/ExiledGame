// @Maxpro 2017

#include "ExiledGame.h"
#include "Inventory.h"


// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Slots.SetNum(AmountOfSlots);
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	

	

}

// Called every frame
void AInventory::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AInventory::GetItemInfoAtIndex(int32 Index, bool &IsEmptySlot, FGameItemInfo &ItemInfo, int32 &Amount) const
{
	FInventorySlot DesiredSlot = Slots[Index];

	if (IsValidSlotClass(DesiredSlot))
	{
		FGameItemInfo DesiredItemInfo = DesiredSlot.ItemClass->ItemInfo;

		ItemInfo = DesiredItemInfo;
		IsEmptySlot = false;
		Amount = DesiredSlot.AmountInSlot;
		return;
	}
	else
	{
		// TODO: Will leaving ItemInfo and Amount blank here have any bad side effects?
		IsEmptySlot = true;

		return;
	}


}

bool AInventory::IsValidSlotClass(FInventorySlot InvSlot) const
{
	if (InvSlot.ItemClass != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AInventory::SearchEmptySlot(bool &Success, int32 &Index)
{
	TArray<FInventorySlot> CurrentSlots = GetSlots();

	// Local variables that I will set my output params to
	bool bResult = false;
	int32 bResultIndex = 0;


	for (int32 i = 0; i < CurrentSlots.Num(); i++)
	{
		FInventorySlot Slot = CurrentSlots[i];

		if (IsValidSlotClass(Slot))
		{
			// do nothing
		}
		else
		{
			bResult = true;
			bResultIndex = i;
			break;
		}
	}

	Success = bResult;
	Index = bResultIndex;
}

void AInventory::SearchFreeStack(const AMasterItem *ItemClass, bool &Success, int32 &Index)
{
	TArray<FInventorySlot> CurrentSlots = GetSlots();

	// Local variables that I will set my output params to
	bool bResult = false;
	int32 bResultIndex = 0;


	for (int32 i = 0; i < CurrentSlots.Num(); i++)
	{
		FInventorySlot Slot = CurrentSlots[i];

		if (IsValidSlotClass(Slot))
		{
			// Check weather the slot class is the same one we are searching for and that our stack is not full
			if ((Slot.ItemClass->StaticClass == ItemClass->StaticClass) && Slot.AmountInSlot < MaxStackSize) 
			{
				bResult = true;
				bResultIndex = i;
				break;
			}
			else
			{

			}
		}
		else
		{

		}
	}

	// Finished querying now set our output params
	Success = bResult;
	Index = bResultIndex;

}

void AInventory::AddItem(class AMasterItem *ItemClass, const int32 Amount, bool &Success, int32 &Rest)
{
	AMasterItem* LocalItemClass = NewObject<AMasterItem>(AMasterItem::StaticClass());
	LocalItemClass->ItemInfo = ItemClass->ItemInfo;
	int32 LocalAmount = Amount;
	int32 FoundIndex;

	if (ItemClass->ItemInfo.bCanBeStacked)
	{
		bool bIsFreeStack;
		int32 FreeStackIndex;

		SearchFreeStack(LocalItemClass, bIsFreeStack, FreeStackIndex);
		if (bIsFreeStack == true)
		{
			FoundIndex = FreeStackIndex;
			if (GetSlots()[FoundIndex].AmountInSlot + LocalAmount > MaxStackSize)
			{
				LocalAmount = GetSlots()[FoundIndex].AmountInSlot + LocalAmount - MaxStackSize;

				GetSlots()[FoundIndex].ItemClass = LocalItemClass;
				GetSlots()[FoundIndex].AmountInSlot = MaxStackSize;

				int32 UnusedRest3;
				bool UnusedSuccess3;
				AddItem(LocalItemClass, LocalAmount, UnusedSuccess3, UnusedRest3);
				Rest = UnusedRest3;
				Success = true;
				return;
			}
			else
			{
				GetSlots()[FoundIndex].ItemClass = LocalItemClass;
				GetSlots()[FoundIndex].AmountInSlot = GetSlots()[FoundIndex].AmountInSlot + LocalAmount;
				Rest = 0;
				Success = true;
				return;
			}
		}
		else
		{
			bool bIsEmptySlot;
			int32 EmptySlotIndex;
			SearchEmptySlot(bIsEmptySlot, EmptySlotIndex);

			if (bIsEmptySlot == true)
			{
				FoundIndex = EmptySlotIndex;	
				if (LocalAmount > MaxStackSize)
				{
					GetSlots()[FoundIndex].ItemClass = LocalItemClass;
					GetSlots()[FoundIndex].AmountInSlot = MaxStackSize;

					int32 UnusedRest2;
					bool UnusedSuccess1;
					AddItem(LocalItemClass, LocalAmount - MaxStackSize, UnusedSuccess1, UnusedRest2);
					Rest = UnusedRest2;
					Success = true;
					return;
				}
				else
				{
					GetSlots()[FoundIndex].ItemClass = LocalItemClass;
					GetSlots()[FoundIndex].AmountInSlot = LocalAmount;
					Rest = 0;
					Success = true;
					return;
				}
			}
			else
			{

				Rest = LocalAmount;
				Success = false;
				return;
			}
		}
	}
	else
	{
		bool bEmptySlotResult;
		int32 bEmptySlotIndex;
		SearchEmptySlot(bEmptySlotResult, bEmptySlotIndex);
		if (bEmptySlotResult == true)
		{
			FoundIndex = bEmptySlotIndex;
			
			GetSlots()[FoundIndex].ItemClass = LocalItemClass;
			GetSlots()[FoundIndex].AmountInSlot = 1;

			if (LocalAmount > 1)
			{
				int32 LocalRest1;
				bool UnusedSuccess2;
				AddItem(LocalItemClass, LocalAmount - 1, UnusedSuccess2, LocalRest1);
				Rest = LocalRest1;
				Success = true;
				return;
			}
			else
			{
				Rest = 0;
				Success = true;
				return;
			}
		}
		else
		{
			Rest = LocalAmount;
			Success = false;
			return;
		}
	}
}

void AInventory::GetAmountAtIndex(const int32 Index, int32 & Amount)
{
	TArray<FInventorySlot> CurrentSlots = GetSlots();

	FInventorySlot &DesiredSlot = CurrentSlots[Index];

	Amount = DesiredSlot.AmountInSlot;
	return;
}

void AInventory::IsSlotEmpty(int32 Index, bool &IsEmpty)
{
	TArray<FInventorySlot> CurrentSlots = GetSlots();

	FInventorySlot DesiredSlot = CurrentSlots[Index];
	
	if (DesiredSlot.ItemClass == nullptr)
	{
		IsEmpty = true;
	}
	else
	{
		IsEmpty = false;
	}

}


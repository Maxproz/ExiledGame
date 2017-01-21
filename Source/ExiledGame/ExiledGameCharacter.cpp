// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExiledGame.h"
#include "ExiledGameCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

AExiledGameCharacter::AExiledGameCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bIsTalking = false;
	bIsInTalkRange = false;
	AssociatedPawn = nullptr;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);


	// Setup basic stuff for the inventory spawning
	InventorySpawnTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	InventorySpawnTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));
	InventorySpawnTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	// Can't do this here because its not created yet, need to figure out how to do it on construction as a parameter.
	//TheInventory->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


}

void AExiledGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params;
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void AExiledGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Talk", IE_Pressed, this, &AExiledGameCharacter::ToggleTalking);
}

void AExiledGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Inventory = NewObject<AInventory>(AInventory::StaticClass());

	if (Inventory)
	{
		Inventory->SetActorTransform(InventorySpawnTransform);
		Inventory->AmountOfSlots = 20;
		Inventory->TopDownCharacterRef = this;
	}

	
	

}

FDialog* AExiledGameCharacter::RetrieveDialog(UDataTable * TableToSearch, FName RowName)
{
	if (!TableToSearch) return nullptr;

	// The table is valid, retrieve the given row if possible
	FString ContextString;
	return TableToSearch->FindRow<FDialog>(RowName, ContextString);
}

void AExiledGameCharacter::GeneratePlayerLines(UDataTable& PlayerLines)
{
	//Get all the row names of the table
	TArray<FName> PlayerOptions = PlayerLines.GetRowNames();

	//For each row name, try to retrieve the contents of the table 
	for (auto It : PlayerOptions)
	{
		//Retrieve the contents of the table
		FDialog* Dialog = RetrieveDialog(&PlayerLines, It);

		if (Dialog)
		{
			// We retrieved a valid row, populate the questions array with our excerpts
			Questions.Add(Dialog->QuestionExcerpt);
		}
	}

	// Make sure to create a reference of the available line for later use
	AvailableLines = &PlayerLines;

}

void AExiledGameCharacter::Talk(FString Excerpt, TArray<FSubtitle>& Subtitles)
{
	// Get all the row names based on our stored lines
	TArray<FName> PlayerOptions = AvailableLines->GetRowNames();
	
	for (auto It : PlayerOptions)
	{
		// Search inside the available lines table to find the pressed Excerpt from the UI
		FDialog* Dialog = RetrieveDialog(AvailableLines, It);

		if (Dialog && Dialog->QuestionExcerpt == Excerpt)
		{
			// We've found the pressed excerpt, assign our sfx to the audio comp and play it
			AudioComp->SetSound(Dialog->SFX);
			AudioComp->Play();

			// Update the corresponding subtitles
			Subtitles = Dialog->Subtitles;

			if (UI && AssociatedPawn && Dialog->bShouldAIAnswer)
			{
				// Calculate the total displayed time for our subtitles
				// When the subtitles end, the associated pawn will be able to talk to our character

				TArray<FSubtitle> SubtitlesToDisplay;

				float TotalSubsTime = 0.f;

				for (int32 i = 0; i < Subtitles.Num(); i++)
				{
					TotalSubsTime += Subtitles[i].AssociatedTime;
				}


				//Just a hardcoded value in order for the AI not to answer right after our subs.
				//It would be better if we expose that to our editor? Sure!
				TotalSubsTime += 1.f;

				//Tell the associated pawn to answer to our character after the specified time!
				AssociatedPawn->AnswerToCharacter(It, SubtitlesToDisplay, TotalSubsTime);
			}
			else if (!Dialog->bShouldAIAnswer) ToggleTalking();
			break;
		}
	}
}



void AExiledGameCharacter::ToggleTalking()
{
	if (bIsInTalkRange)
	{
		//If we are in talk range handle the talk status and the UI
		bIsTalking = !bIsTalking;
		ToggleUI();

		if (bIsTalking && AssociatedPawn)
		{
			//The associated pawn is polite enough to face us when we talk to him!
			FVector Location = AssociatedPawn->GetActorLocation();
			FVector TargetLocation = GetActorLocation();

			// TODO: make sure you understand how this function works visually
			AssociatedPawn->SetActorRotation((TargetLocation - Location).Rotation());
		}

	}
}
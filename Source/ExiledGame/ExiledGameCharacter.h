// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "AICharacter.h"
#include "DialogUI.h"
#include "ExiledGameCharacter.generated.h"

UCLASS(Blueprintable)
class AExiledGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AExiledGameCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;



	/*Interaction with Dialog AI Code START*/
private:

	/*True if the player is currently talking with any pawn*/
	bool bIsTalking;

	/*True if the player is inside a valid range to start talking to a pawn*/
	bool bIsInTalkRange;

	/*Initiates or terminates a conversation*/
	void ToggleTalking();

	/*The Pawn that the player is currently talking to*/
	AAICharacter* AssociatedPawn;

	/*A Reference to our lines - retrieved from the associated pawn*/
	UDataTable* AvailableLines;

	/*Searches in the given row inside the specified table*/
	FDialog* RetrieveDialog(UDataTable* TableToSearch, FName RowName);

public:

	/*Generates the player lines*/
	void GeneratePlayerLines(UDataTable& PlayerLines);

	/*This array is essentially an Array of Excerpts from our dialogs!*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Questions;

	/*Performs the actual talking - informs the associated pawn if necessary in order to answer
	The subtitles array contain all the subtitles for this talk - it should be passed to our UI*/
	UFUNCTION(BlueprintCallable, Category = "DialogSystem")
	void Talk(FString Excerpt, TArray<FSubtitle>& Subtitles);

	/*Enables / disables our talk ability. The player can't talk if he's not in a valid range*/
	void SetTalkRangeStatus(bool Status) { bIsInTalkRange = Status; }

	/*Sets a new associated pawn*/
	void SetAssociatedPawn(AAICharacter* Pawn) { AssociatedPawn = Pawn; }

	/*Retrieves the UI reference*/
	UDialogUI* GetUI() { return UI; }

protected:

	/*The component responsible for playing our SFX*/
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	/*Opens or closes the conversation UI*/
	UFUNCTION(BlueprintImplementableEvent, Category = "DialogSystem")
	void ToggleUI();

	/*UI Reference*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDialogUI* UI;


	/*Interaction with Dialog AI Code END*/

};


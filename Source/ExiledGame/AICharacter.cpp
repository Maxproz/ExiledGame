// @Maxpro 2017

#include "ExiledGame.h"
#include "ExiledGameCharacter.h"
#include "AICharacter.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Init the box and audio comps
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	BoxComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Register the begin and end overlap functions
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnBoxOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnBoxEndOverlap);

}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::OnBoxOverlap(UPrimitiveComponent* ThisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA<AExiledGameCharacter>())
	{
		AExiledGameCharacter* Char = Cast<AExiledGameCharacter>(OtherActor);
		Char->SetTalkRangeStatus(true);
		Char->GeneratePlayerLines(*PlayerLines);
		Char->SetAssociatedPawn(this);
	}
}

void AAICharacter::OnBoxEndOverlap(UPrimitiveComponent* ThisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex)
{
	if (OtherActor->IsA<AExiledGameCharacter>())
	{
		AExiledGameCharacter* Char = Cast<AExiledGameCharacter>(OtherActor);
		Char->SetTalkRangeStatus(false);
		Char->SetAssociatedPawn(nullptr);
	}
}

void AAICharacter::Talk(USoundBase * SFX, TArray<FSubtitle> Subs)
{
	AExiledGameCharacter* Char = Cast<AExiledGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Play the corresponding sfx
	AudioComp->SetSound(SFX);
	AudioComp->Play();

	// Tell the UI to update with the new subtitles
	Char->GetUI()->UpdateSubtitles(Subs);
}

void AAICharacter::AnswerToCharacter(FName PlayerLine, TArray<FSubtitle>& SubtitlesToDisplay, float delay)
{
	if (!AILines) return;

	// Retrieve the corresponding line
	FString ContextString;
	FDialog* Dialog = AILines->FindRow<FDialog>(PlayerLine, ContextString);

	AExiledGameCharacter* MainChar = Cast<AExiledGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Dialog && MainChar)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;

		TimerDel.BindUFunction(this, FName("Talk"), Dialog->SFX, Dialog->Subtitles);
		
		// Talk to the player after the delay time has passed
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, delay, false);
	}
}


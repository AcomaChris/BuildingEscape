// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Declare the owner on initialization
	Owner = GetOwner();

	// Check to see if Owner is a null pointer
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get owner of door: %s"), *this->GetName());
	}

	// Check to see if PressurePlate is null pointer
	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate attached to %s is null."), *this->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO make into a parameter
	{
		// If yes, open the door
		OnOpen.Broadcast();
	}
	else
	{
		// If not, close the door
		OnClose.Broadcast();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	// Protect against PressurePlate null pointer
	if (PressurePlate == nullptr)
	{
		return 0.0f;
	}

	float TotalMass = 0.0f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them an add their masses together
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate."), *Actor->GetName());
	}

	return TotalMass;
}
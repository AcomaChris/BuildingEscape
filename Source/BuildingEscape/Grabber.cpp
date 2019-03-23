// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"))

	GrabberPlayerController = GetWorld()->GetFirstPlayerController();

	// ...

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//GrabberPlayerPosition = GrabberPlayerController->GetPlayerViewPoint();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	// Log out to test

	// Get player viewpoint this tick
	//UE_LOG(LogTemp, Warning, TEXT("Position: %s | Rotation: %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotator.ToString()
	//)

	// Debug line trace variables
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;

	// Draw a red trace in the world to visualize
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f);

	// Setup query parameters for collision
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	ActorHit = LineTraceHit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit: %s"), *(ActorHit->GetName()))
	}
}


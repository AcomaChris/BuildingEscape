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

	InitializeGrabberVariables();

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

void UGrabber::InitializeGrabberVariables()
{
	// Get the player controller
	GrabberPlayerController = GetWorld()->GetFirstPlayerController();

	// Initialize the components
	InizializePhysicsHandle();
	InizializeInputComponent();

}

// Gets the physics handle and links to PhysicsHandle pointer
void UGrabber::InizializePhysicsHandle()
{
	// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	// Error out if we cannot find a physics handle
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing PhysicsHandle on actor %s"), *(GetOwner()->GetName()))
	}
}

// Gets the physics handle and links to InputComponent pointer
void UGrabber::InizializeInputComponent()
{
	// Look for attached input component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	// Error out if we cannot find an input component
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing InputComponent on actor %s"), *(GetOwner()->GetName()))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input components have been found"))

		//Binding the input actions
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Raycast and grab what's in reach
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))
}

// Release what's in reach
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release pressed"))
}
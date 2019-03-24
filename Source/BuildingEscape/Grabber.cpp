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

	FindPhysicsHandleComponent();

	SetupInputComponent();

	// ...

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GetGrabbedComponent())
	{
		// Declare variables for use in player controller location and rotation
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotator;

		// Get the player controller location and rotators and store in variables
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotator
		);

		// Debug line trace variables
		FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;

		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

// Gets the physics handle and links to PhysicsHandle pointer
void UGrabber::FindPhysicsHandleComponent()
{
	// Get the player controller
	GrabberPlayerController = GetWorld()->GetFirstPlayerController();

	// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	// Error out if we cannot find a physics handle
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing PhysicsHandle on actor %s"), *(GetOwner()->GetName()))
	}
}

// Setup the (assumed) attached input component
void UGrabber::SetupInputComponent()
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

	// LINE TRACK and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we reach something then attach a phsyics handle
	if (ActorHit)
	{
		// Attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(), // ERROR HERE "Pointer to incomplete class type not allowed"
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

// Release what's in reach
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release pressed"))

		// TODO release physics handle
		PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Declare variables for use in player controller location and rotation
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	// Get the player controller location and rotators and store in variables
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	// Debug line trace variables
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;

	if (DebugGrabberLine)
	{
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
	}

	// Setup query parameters for collision
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Create a hit result
	FHitResult Hit;

	// Line-trace (AKA ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	AActor * ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}
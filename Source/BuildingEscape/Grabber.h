// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Classes/Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// Find (assumed) attache physics handle
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach();

	/// Stores the player controller
	APlayerController * GrabberPlayerController = nullptr;
	FString GrabberPlayerPosition = "";

	/// How far each of the player can we see ahead of them
	UPROPERTY(EditAnywhere)
		float Reach = 100.0f;

	UPROPERTY(EditAnywhere)
		bool DebugGrabberLine = false;

	/// Return vector for start or end of line using Reach variable
	FVector GetReachLineEnd();
	FVector GetReachLineStart();

	// Physics handler
	UPhysicsHandleComponent * PhysicsHandle = nullptr;

	// Input component
	UInputComponent * InputComponent = nullptr;

	/// Raycast and grab or release what's in reach
	void Grab();
	void Release();

};
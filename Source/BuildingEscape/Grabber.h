// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	/// Stores the player controller
	APlayerController * GrabberPlayerController;
	FString GrabberPlayerPosition = "";

	/// Variables for storing location and rotation of player controller
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	/// Debug line trace and collision variables
	FVector LineTraceEnd = FVector(0.0f, 0.0f, 0.0f);
	FHitResult LineTraceHit;
	AActor * ActorHit;

	/// How far each of the player can we see ahead of them
	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;

};

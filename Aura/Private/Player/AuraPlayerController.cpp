// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/*
	* Line trace from cursor. There are several senarios:
	* A. LastActor == null && ThisActor == Null
	*    -Do nothing
	* B. LastActor ==null && ThisActor == valid
	*	-Highlight ThisActor
	* C. LastActor == Valid && ThisActor == null
	*	-UnHighLight LastActor
	* D. Both walid Last Actor != ThisActor
	*	Unhiglight lastActor, highlight this actor
	* E. Both are valid and are same actor
	*	do nothing
	*/
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighLightActor();
		}
		//Case A - nothing
	}
	else
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHighLightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
			// Case E - Nothing
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem) 
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::EyeDropper;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControledPawn = GetPawn<APawn>())
	{
		ControledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UCharacterMovementComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

UCLASS(config=Game)
class BOUNCE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh for first person (arms seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* Mesh1P;

	/** Character movement component */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta=(AllowPrivateAccess="true"))
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	class UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	class UInputAction* SprintAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	UInputAction* PauseAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	class UInputAction* SlideAction;

// Called when the game starts or when spawned
public:
	APlayerCharacter();

	 UFUNCTION()
	 void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult &Hit) override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for default movement */
    void Walk();

	/** Called for holding sprint */
    void Sprint(const FInputActionValue& Value);

	/** Called for releasing sprint */
    void StopSprinting(const FInputActionValue& Value);

	/** Called for holding slide */
    void Slide(const FInputActionValue& Value);

	/** Called for releasing slide */
    void StopSliding(const FInputActionValue& Value);

	/** Called for default movement */
    void Bounce(const FInputActionValue& Value);

	/** Called for pause */
	void Pause(const FInputActionValue& Value);

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

	/** Maximum speed while walking */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveSpeedWalk = 600.f;

	/** Maximum speed while sprinting */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveSpeedSprint = 1300.f;

	/** Maximum speed while sliding */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveSpeedSlide = 2300.f;

	/** Applied friction while grounded */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveFrictionGround = 8.f;

	/** Applied friction while sliding */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveFrictionSlide = 1.f;

	/** Applied friction while airborne */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveFrictionAir = 1.2f;

	/** Applied acceleration while walking */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveAccelerationWalk = 1024.f;

	/** Applied acceleration while sprinting */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveAccelerationSprint = 2048.f;

	/** Applied acceleration while sliding */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveAccelerationSlide = 32.f;

	/** The player's maximum health. This is the highest value of their health can be. This value is a value of the player's health, which starts at when spawned.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float ProjDamageMultiplier = 5.f;

	/** The player's current health. When reduced to 0, they are considered dead.*/
	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	bool Sprinting;

	UPROPERTY()
	bool Sliding;

	UPROPERTY()
	bool Bounced;

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCameraComponent; }

	float CapsuleRadius = 15.f;
	float CapsuleHeight = 80.f;
	float CapsuleHalfed = 40.f;

	/** Getter for walking speed */
	UFUNCTION(BlueprintPure, Category="Movement")
	FORCEINLINE float GetSpeedWalk() const { return MoveSpeedWalk; }

	/** Getter for sprinting speed */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetSpeedSprint() const { return MoveSpeedSprint; }

	/** Getter for sliding speed */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetSpeedSlide() const { return MoveSpeedSlide; }

	/** Getter for grounded friction */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetFrictionGround() const { return MoveFrictionGround; }

	/** Getter for sliding friction */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetFrictionSlide() const { return MoveFrictionSlide; }

	/** Getter for airborne friction */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetFrictionAir() const { return MoveFrictionAir; }

	/** Getter for walking acceleration */
	UFUNCTION(BlueprintPure, Category="Movement")
	FORCEINLINE float GetAccelerationWalk() const { return MoveAccelerationWalk; }

	/** Getter for sprinting acceleration */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetAccelerationSprint() const { return MoveAccelerationSprint; }

	/** Getter for sliding acceleration */
	UFUNCTION(BlueprintCallable, Category="Movement")
	FORCEINLINE float GetAccelerationSlide() const { return MoveAccelerationSlide; }

	// Health code copied from BounceCharacter

	/** Getter for Max Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** Getter for Current Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, AActor* DamageCauser);
};

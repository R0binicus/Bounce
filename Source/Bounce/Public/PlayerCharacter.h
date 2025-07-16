// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/LogMacros.h"
#include "WeaponComponent.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UCharacterMovementComponent;
class UWeaponComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

UCLASS(config = Game)
class BOUNCE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Pawn mesh for first person (arms seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** Character movement component */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta=(AllowPrivateAccess="true"))
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UWeaponComponent* WeaponComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlideAction;

protected:
	/** Maximum speed while walking */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeedWalk = 600.f;

	/** Maximum speed while sprinting */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeedSprint = 1300.f;

	/** Maximum speed while sliding */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeedSlide = 2300.f;

	/** Applied friction while grounded */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveFrictionGround = 8.f;

	/** Applied friction while sliding */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveFrictionSlide = 1.f;

	/** Applied friction while airborne */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveFrictionAir = 1.2f;

	/** Applied acceleration while walking */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveAccelerationWalk = 1024.f;

	/** Applied acceleration while sprinting */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveAccelerationSprint = 2048.f;

	/** Applied acceleration while sliding */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveAccelerationSlide = 32.f;

	/** The player's maximum health. This is the highest value of their health can be. This value is a value of the player's health, which starts at when spawned.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float ProjDamageMultiplier = 5.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Respawn")
	FVector RespawnPosition;

	/** The player's current health. When reduced to 0, they are considered dead.*/
	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	bool Sprinting;

	UPROPERTY()
	bool Sliding;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Collider")
	float CapsuleRadius = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = "Collider")
	float CapsuleHeight = 80.f;
	UPROPERTY(EditDefaultsOnly, Category = "Collider")
	float CapsuleHalfed = 40.f;
	UPROPERTY(EditDefaultsOnly, Category = "Collider")
	UCapsuleComponent* CoyoteCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool Bounced;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool Coyote = true;

	/** Sound to play when we take damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* DamageSound;

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Called for input actions */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Walk();
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
	void StopSliding(const FInputActionValue& Value);
	void Bounce(const FInputActionValue& Value);
	void Pause(const FInputActionValue& Value);

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void OnCoyoteOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void OnCoyoteOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	/** Getter for Weapon Component */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE UWeaponComponent* GetWeaponComponent() { return WeaponComponent; }

	/** Setter for Weapon Component */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponComponent(UWeaponComponent* _weaponComponent) { WeaponComponent = _weaponComponent; }

	/** Getter for walking speed */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetSpeedWalk() const { return MoveSpeedWalk; }

	/** Getter for sprinting speed */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetSpeedSprint() const { return MoveSpeedSprint; }

	/** Getter for sliding speed */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetSpeedSlide() const { return MoveSpeedSlide; }

	/** Getter for grounded friction */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetFrictionGround() const { return MoveFrictionGround; }

	/** Getter for sliding friction */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetFrictionSlide() const { return MoveFrictionSlide; }

	/** Getter for airborne friction */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetFrictionAir() const { return MoveFrictionAir; }

	/** Getter for walking acceleration */
	UFUNCTION(BlueprintPure, Category = "Movement")
	FORCEINLINE float GetAccelerationWalk() const { return MoveAccelerationWalk; }

	/** Getter for sprinting acceleration */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetAccelerationSprint() const { return MoveAccelerationSprint; }

	/** Getter for sliding acceleration */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE float GetAccelerationSlide() const { return MoveAccelerationSlide; }

	/** Getter for Max Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** Getter for Current Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** Setter for Current Health*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** Setter for RespawnPosition*/
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void SetRespawnPosition(FVector respawnPos) { RespawnPosition = respawnPos; }

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Controls")
	bool IsPlayerUsingGamePad();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

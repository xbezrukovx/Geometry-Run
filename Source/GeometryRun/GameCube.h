// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameCube.generated.h"

UCLASS()
class GEOMETRYRUN_API AGameCube : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCube();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UStaticMeshComponent* StaticMeshCube = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UBoxComponent* CollisionBox = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class USpringArmComponent* SpringArm = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnyWhere)
		UParticleSystem* SlipFx = nullptr;
	UPROPERTY(EditAnyWhere)
		UNiagaraSystem* PoofFx = nullptr;
	UPROPERTY() // This is tail, behind cube;
		UParticleSystemComponent* CurrentSlip = nullptr; 

	UFUNCTION() // This Function is death event;
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void MovementTick();
	void JumpEnable();
	void JumpDisable();
	void FlipJump();
	void FinishFlipRotate(); // For alignment when cube is on the ground after jump;
	
	bool isIdle;
	bool isJumpPressed;
};
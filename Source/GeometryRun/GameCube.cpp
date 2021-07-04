// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCube.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGameCube::AGameCube()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshCube->SetGenerateOverlapEvents(false);
	StaticMeshCube->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshCube->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->AttachTo(RootComponent);
	CollisionBox->SetRelativeLocation(FVector(40,0,0));
	CollisionBox->SetBoxExtent(FVector(2, 32, 32));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGameCube::OnOverlap);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(135, 0, 0), FQuat(0,-25,300 ,0));
	SpringArm->TargetArmLength = 600;
	SpringArm->bEnableCameraLag = true;

	GetMesh()->DestroyComponent();
}

// Called when the game starts or when spawned
void AGameCube::BeginPlay()
{
	Super::BeginPlay();
	CurrentSlip = UGameplayStatics::SpawnEmitterAttached(SlipFx, RootComponent, FName(TEXT("None")), FVector(-30, 0, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset);
}

// Called every frame
void AGameCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovementTick();
	FlipJump();
	FinishFlipRotate();
	if (isJumpPressed) Jump();
}

// Called to bind functionality to input
void AGameCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGameCube::JumpEnable);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGameCube::JumpDisable);
}

void AGameCube::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentSlip && PoofFx)
	{
		FVector SpawnLocation = this->GetActorLocation();
		FRotator SpawnRotation = this->GetActorRotation();
		CurrentSlip->DestroyComponent();
		StaticMeshCube->DestroyComponent();
		UCharacterMovementComponent* Movement = this->GetCharacterMovement();
		Movement->DestroyComponent();
		UNiagaraComponent* DieFx = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PoofFx, SpawnLocation, SpawnRotation);
	}
}

void AGameCube::MovementTick()
{
	AddMovementInput(FVector(1, 0, 0));
}

void AGameCube::JumpEnable()
{
	isJumpPressed = true;
}

void AGameCube::JumpDisable()
{
	isJumpPressed = false;
}

void AGameCube::FlipJump()
{
	UCharacterMovementComponent* Movement = this->GetCharacterMovement();
	if (Movement != nullptr && Movement->IsFalling())
	{
		isIdle = false;
		int CurrentPitch = StaticMeshCube->GetRelativeRotation().Pitch;
		StaticMeshCube->AddLocalRotation(FRotator(1.5,0,0));
	}
}

void AGameCube::FinishFlipRotate()
{
	UCharacterMovementComponent* Movement = this->GetCharacterMovement();
	if (Movement != nullptr && !Movement->IsFalling() && !isIdle)
	{
		int CurrentPitch = StaticMeshCube->GetRelativeRotation().Pitch;
		if (CurrentPitch != 0 && CurrentPitch != 90 && CurrentPitch != -90) {
			StaticMeshCube->AddLocalRotation(FRotator(1.5, 0, 0));
		}
		else {
			isIdle = true;
		}
	}
}
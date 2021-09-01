// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalMechanics/OrbitalMovementComponent.h"
#include "OrbitalMechanics/Orbital.h"
#include "OrbitalMechanics/Universe.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UOrbitalMovementComponent::UOrbitalMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

AUniverse* UOrbitalMovementComponent::GetUniverse() const
{
	if (Universe != nullptr) return Universe;

	const auto World = GetWorld();
	if (World == nullptr) return nullptr;
	
	const auto Actor = UGameplayStatics::GetActorOfClass(World, AUniverse::StaticClass());
	return Cast<AUniverse>(Actor);
}

IOrbitalInterface* UOrbitalMovementComponent::GetSimulationOrbital(const float Timestep, const bool bUsePhysicsTimestep) const
{
	const auto LocalUniverse = GetUniverse();
	const auto UniverseConstants = LocalUniverse->GetConstants();
	const float UsedTimestep = bUsePhysicsTimestep ? UniverseConstants->GetPhysicsTimestep() : Timestep;

	const auto Constants = UUniversalConstants::Create(UsedTimestep, UniverseConstants->G());
	return CreateOrbital(Constants);
}

FVector UOrbitalMovementComponent::UpdateVelocity(const TArray<IOrbitalInterface*> Others)
{
	if (Orbital == nullptr) return GetVelocity();

	TArray<IOrbitalInterface*> LocalOthers;
	for (const auto Other : Others)
	{
		const auto OrbitalMovement = Cast<UOrbitalMovementComponent>(Other);
		if (OrbitalMovement != nullptr && OrbitalMovement != this)
		{
			LocalOthers.Add(OrbitalMovement);
		}
	}
	
	Velocity = Orbital->UpdateVelocity(LocalOthers);
	return Velocity;
}

FVector UOrbitalMovementComponent::UpdateLocation()
{
	if (Orbital == nullptr) return GetLocation();

	const auto Location = Orbital->UpdateLocation();
	GetOwner()->SetActorLocation(Location);

	return Location;
}

void UOrbitalMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Universe = GetUniverse();
	const auto Constants = Universe->GetConstants();
	Orbital = CreateOrbital(Constants);
	
	Universe->Register(this);
}

void UOrbitalMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Universe == nullptr) return;
	Universe->Unregister(this);
}

UOrbital* UOrbitalMovementComponent::CreateOrbital(UUniversalConstants* Constants) const
{
	const auto LocalOrbital = NewObject<UOrbital>();
	LocalOrbital->Init(Constants, GetLocation(), GetVelocity(), GetMass());

	return LocalOrbital;
}


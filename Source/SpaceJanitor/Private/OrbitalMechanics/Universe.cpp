// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalMechanics/Universe.h"
#include "OrbitalMechanics/Orbital.h"
#include "OrbitalMechanics/OrbitalMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

AUniverse::AUniverse()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	bSimulateInEditor = false;
	SimulationSteps = 1000;
	SimulationTimestep = 0.1f;
	bUsePhysicsTimestep = false;
}

void AUniverse::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldTickIfViewportsOnly())
	{
		PrimaryActorTick.TickInterval = SimulationTickInterval;
		EditorSimulate();
	}
	else
	{
		Simulate();
	}
}

bool AUniverse::ShouldTickIfViewportsOnly() const
{
	const auto World = GetWorld();
	if (World != nullptr && World->WorldType == EWorldType::Editor && bSimulateInEditor)
	{
		return true;
	}

	FlushPersistentDebugLines(World);
	return false;
}

void AUniverse::Register(IOrbitalInterface* Orbital)
{
	Orbitals.Add(Orbital);
}

void AUniverse::Unregister(IOrbitalInterface* Orbital)
{
	Orbitals.Remove(Orbital);
}

void AUniverse::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.TickInterval = 0;
}

void AUniverse::Simulate()
{
	for (const auto Orbital : Orbitals) Orbital->UpdateVelocity(Orbitals);
	for (const auto Orbital : Orbitals) Orbital->UpdateLocation();
}

void AUniverse::EditorSimulate()
{
	const auto World = GetWorld();
	if (World == nullptr) return;
	
	IOrbitalInterface* ReferenceFrameOrbital = nullptr;
	const auto EditorOrbitals = GetEditorOrbitals(ReferenceFrameOrbital);
	const FVector ReferenceFrameInitialLocation = ReferenceFrameOrbital != nullptr
			? ReferenceFrameOrbital->GetLocation()
			: FVector::ZeroVector;

	TMap<IOrbitalInterface*, TArray<FVector>> SimulationPoints;
	for (const auto Orbital : EditorOrbitals) SimulationPoints.Add(Orbital, TArray<FVector>());
	
	for (int i = 0; i < SimulationSteps; i++)
	{
		TArray<FVector> Locations;
		
		const FVector ReferenceFrameLocation = ReferenceFrameOrbital != nullptr
			? ReferenceFrameOrbital->GetLocation()
			: FVector::ZeroVector;
		
		for (const auto Orbital : EditorOrbitals) Orbital->UpdateVelocity(EditorOrbitals);
		for (const auto Orbital : EditorOrbitals)
		{
			auto Points = SimulationPoints[Orbital];
			auto Location = Orbital->UpdateLocation();

			if (ReferenceFrameOrbital != nullptr)
			{
				const FVector ReferenceFrameOffset = ReferenceFrameLocation - ReferenceFrameInitialLocation;
				Location -= ReferenceFrameOffset;
			}
			
			if (ReferenceFrameOrbital != nullptr && Orbital == ReferenceFrameOrbital)
			{
				Location = ReferenceFrameInitialLocation;
			}

			Points.Add(Location);
			SimulationPoints[Orbital] = Points;
		}
	}

	FlushPersistentDebugLines(World);
	for (const auto Orbital : EditorOrbitals)
	{
		const auto Points = SimulationPoints[Orbital];
		for (int i = 0; i < Points.Num(); i++)
		{
			const auto NextIndex = i + 1;
			if (NextIndex >= Points.Num()) continue;

			const auto Current = Points[i];
			const auto Next = Points[NextIndex];

			DrawDebugLine(
				World,
				Current,
				Next,
				FColor::Red,
				true,
				1,
				0,
				10
				);
		}
	}
}

TArray<IOrbitalInterface*> AUniverse::GetEditorOrbitals(IOrbitalInterface*& OutReferenceFrameOrbital) const
{
	TArray<IOrbitalInterface*> Result;
	
	const auto World = GetWorld();
	if (World == nullptr) return Result;

	const UOrbitalMovementComponent* ReferenceFrameOrbitalMovement = nullptr;
	if (DrawOrbitsRelativeTo != nullptr)
	{
		const auto Component = DrawOrbitsRelativeTo->GetComponentByClass(UOrbitalMovementComponent::StaticClass());
		if (Component != nullptr) ReferenceFrameOrbitalMovement = Cast<UOrbitalMovementComponent>(Component); 
	}
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), Actors);
	for (const auto Actor : Actors)
	{
		const auto Component = Actor->GetComponentByClass(UOrbitalMovementComponent::StaticClass());
		if (Component == nullptr) continue;;
		
		const auto OrbitalMovement = Cast<UOrbitalMovementComponent>(Component);
		if (OrbitalMovement != nullptr)
		{
			const auto Orbital = OrbitalMovement->GetSimulationOrbital(SimulationTimestep, bUsePhysicsTimestep);
			if (Orbital == nullptr) continue;

			if (OrbitalMovement == ReferenceFrameOrbitalMovement) OutReferenceFrameOrbital = Orbital;
			Result.Add(Orbital);
		}
	}

	return Result;
}


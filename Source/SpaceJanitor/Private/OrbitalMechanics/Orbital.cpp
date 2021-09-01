// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalMechanics/Orbital.h"
#include "OrbitalMechanics/UniversalConstants.h"

void UOrbital::Init(UUniversalConstants* UniversalConstants, const FVector InitialLocation, const FVector InitialVelocity, const float ObjectMass)
{
	this->Constants = UniversalConstants;
	this->Location = InitialLocation;
	this->Velocity = InitialVelocity;
	this->Mass = ObjectMass;
}

FVector UOrbital::UpdateVelocity(TArray<IOrbitalInterface*> Others)
{
	for (const auto Other : Others)
	{
		if (Other != this)
		{
			const float G = Constants->GetGravitationalConstant();
			const float TimeStep = Constants->GetPhysicsTimestep();
			const FVector OtherLocation = Other->GetLocation();
			const float OtherMass = Other->GetMass();
			
			const float SquareDistance = (OtherLocation - Location).SizeSquared();
			FVector ForceDirection = OtherLocation - Location;
			ForceDirection.Normalize();
			const FVector Force = (ForceDirection * (G * Mass * OtherMass)) / SquareDistance;
			const FVector Acceleration = Force / Mass;

			Velocity += Acceleration * TimeStep;
		}
	}

	return Velocity;
}

FVector UOrbital::UpdateLocation()
{
	const float TimeStep = Constants->GetPhysicsTimestep();
	
	Location += Velocity * TimeStep;
	return Location;
}

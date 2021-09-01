// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OrbitalMechanics/OrbitalInterface.h"
#include "Orbital.generated.h"

/**
 * @brief Humble object that encapsulated orbital movement computation.
 */
UCLASS()
class SPACEJANITOR_API UOrbital : public UObject, public IOrbitalInterface
{
	GENERATED_BODY()
	
protected:
	/**
	 * @brief Universal constants, these are used for the gravity computation. 
	 */
	UPROPERTY()
	class UUniversalConstants* Constants;
	
	/**
	 * @brief Current orbital location. 
	 */
	UPROPERTY()
	FVector Location;

	/**
	 * @brief Current orbital velocity. 
	 */
	UPROPERTY()
	FVector Velocity;

	/**
	 * @brief Mass of the orbital.
	 */
	UPROPERTY()
	float Mass;

public:
	/**
	 * @brief Initializes a orbital.
	 * @param UniversalConstants Universal constants to use for gravitational acceleration computation
	 * @param InitialLocation Initial location of the orbital
	 * @param InitialVelocity Initial velocity of the orbital
	 * @param ObjectMass Orbital objects mass
	 */
	void Init(UUniversalConstants* UniversalConstants, FVector InitialLocation, FVector InitialVelocity, float ObjectMass);
	
	/**
	 * @brief Returns the current orbital location.
	 * @return Current location
	 */
	virtual FVector GetLocation() const override
	{
		return Location;
	}

	/**
	 * @brief Returns the current orbital velocity
	 * @return Current velocity
	 */
	virtual FVector GetVelocity() const override
	{
		return Velocity;
	}

	/**
	 * @brief Returns the orbitals mass
	 * @return Orbitals mass
	 */
	virtual float GetMass() const override
	{
		return Mass; 
	}

	/**
	 * @brief Updates the velocity with a given set of other orbitals within the universe. 
	 * @param Others Other orbitals within the universe
	 * @return Updated velocity
	 */
	virtual FVector UpdateVelocity(TArray<IOrbitalInterface*> Others) override;

	/**
	 * @brief Updates the location based on the current velocity.
	 * @return Updated location
	 */
	virtual FVector UpdateLocation() override;
};

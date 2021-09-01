// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OrbitalInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOrbitalInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IOrbitalInterface
{
	GENERATED_BODY()

public:
	/**
	* @brief Returns the current orbital location.
	* @return Current location
	*/
	virtual FVector GetLocation() const = 0;

	/**
	* @brief Returns the current orbital velocity
	* @return Current velocity
	*/
	virtual FVector GetVelocity() const = 0;
	
	/**
	* @brief Returns the orbitals mass
	* @return Orbitals mass
	*/
	virtual float GetMass() const = 0;

	/**
	* @brief Updates the velocity with a given set of other orbitals within the universe. 
	* @param Others Other orbitals within the universe
	* @return Updated velocity
	*/
	virtual FVector UpdateVelocity(TArray<IOrbitalInterface*> Others) = 0;

	/**
	* @brief Updates the location based on the current velocity.
	* @return Updated location
	*/
	virtual FVector UpdateLocation() = 0;
};

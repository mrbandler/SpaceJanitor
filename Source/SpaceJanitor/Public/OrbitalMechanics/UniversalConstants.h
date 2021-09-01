// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UniversalConstants.generated.h"

/**
 * Data asset for universal constants.
 */
UCLASS()
class SPACEJANITOR_API UUniversalConstants : public UDataAsset
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Physics timestep. 
	 */
	UPROPERTY(EditAnywhere, Category="Constants")
	float PhysicsTimestep;

	/**
	 * @brief Gravitational constants also know as G. 
	 */
	UPROPERTY(EditAnywhere, Category="Constants")
	float GravitationalConstant;

public:
	/**
	 * @brief Creates new universal constants, can only be used in C++ code.
	 * @param Timestep Timestep to use
	 * @param G Gravitational constants to use
	 */
	static UUniversalConstants* Create(const float Timestep, const float G)
	{
		auto Result = NewObject<UUniversalConstants>();
		Result->PhysicsTimestep = Timestep;
		Result->GravitationalConstant = G;

		return Result;
	}

	/**
	 * @brief Returns the physics timestep.
	 * @return Physics timestep constant 
	 */
	float GetPhysicsTimestep() const
	{
		return PhysicsTimestep;
	}

	/**
	 * @brief Returns the gravitational constant also known as G.
	 * @return Gravitational constant, G
	 */
	float GetGravitationalConstant() const
	{
		return GravitationalConstant;
	}

	/**
	 * @brief Returns the gravitational constant also known as G.
	 * @return Gravitational constant, G
	 */
	float G() const
	{
		return GravitationalConstant;
	}
};

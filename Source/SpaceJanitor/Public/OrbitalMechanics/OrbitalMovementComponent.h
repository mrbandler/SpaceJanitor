// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitalMechanics/OrbitalInterface.h"
#include "OrbitalMovementComponent.generated.h"

/**
 * @brief Orbital movement component.
 */
UCLASS(ClassGroup=("Space Janitor"), meta=(BlueprintSpawnableComponent))
class SPACEJANITOR_API UOrbitalMovementComponent : public UActorComponent, public IOrbitalInterface
{
	GENERATED_BODY()

protected:
	/**
	* @brief Current orbital velocity. 
	*/
	UPROPERTY(EditAnywhere, Category="Orbital Movement")
	FVector Velocity;

	/**
	* @brief Mass of the orbital.
	*/
	UPROPERTY(EditAnywhere, Category="Orbital Movement")
	float Mass;

private:
	/**
	 * @brief Universe component is registered on.
	 */
	UPROPERTY()
	class AUniverse* Universe;

	/**
	 * @brief Underlying orbital that computes the gravitational acceleration.
	 */
	UPROPERTY()
	class UOrbital* Orbital;
	
public:
	/**
	 * @brief Default constructor.
	 */
	UOrbitalMovementComponent();

	/**
	* @brief Returns the current orbital location.
	* @return Current location
	*/
	virtual FVector GetLocation() const override
	{
		return GetOwner()->GetActorLocation();
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
	 * @brief Returns the universe the component is registered on.
	 * @return Universe component is registered on
	 */
	class AUniverse* GetUniverse() const;
	
	/**
	 * @brief Returns the simulation orbital, used for in editor N-body simulation.
	 * @param Timestep Timestep to use for the simulation orbital
	 * @param bUsePhysicsTimestep Flag, whether to use the universal physics timestep or not
	 * @return Created simulation orbital
	 */
	class IOrbitalInterface* GetSimulationOrbital(const float Timestep, const bool bUsePhysicsTimestep = false) const;

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
	
protected:
	/**
	 * @brief Will be called when the game starts or when the component is added to an actor.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Will be called when the game ends or when the component is removed from an actor.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Creates an orbital to use for the N-body simulation.
	 * @param Constants Universal constants to use to create the orbital
	 * @return Created orbital
	 */
	UOrbital* CreateOrbital(class UUniversalConstants* Constants) const;

};

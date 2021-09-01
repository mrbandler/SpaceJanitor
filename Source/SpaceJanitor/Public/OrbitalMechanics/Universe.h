// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UniversalConstants.h"
#include "GameFramework/Actor.h"
#include "Universe.generated.h"

/**
 * @brief Universe actor, runs the N-body simulation. 
 */
UCLASS(Blueprintable, Category="Space Janitor")
class SPACEJANITOR_API AUniverse : public AActor
{
	GENERATED_BODY()
	
	/**
	 * @brief Universal constants.
	 */
	UPROPERTY(EditAnywhere, Category="Universe")
	UUniversalConstants* Constants;
	
	/**
	 * @brief Flag, whether or not to simulate in editor.
	 */
	UPROPERTY(EditAnywhere, Category="Editor Universe")
	bool bSimulateInEditor;

	UPROPERTY(EditAnywhere, Category="Editor Universe")
	float SimulationTickInterval = 1;
	
	/**
	 * @brief Number of steps to simulate.
	 */
	UPROPERTY(EditAnywhere, Category="Editor Universe")
	int32 SimulationSteps;

	/**
	 * @brief Timestep to be used for the editor simulation.
	 */
	UPROPERTY(EditAnywhere, Category="Editor Universe")
	float SimulationTimestep;
	
	/**
	 * @brief Flag, whether to use the universe physics timestep for the editor simulation.
	 */
	UPROPERTY(EditAnywhere, Category="Editor Universe")
	bool bUsePhysicsTimestep;

	/**
	 * @brief Actor to draw the editor simulation orbits relative to.
	 */
	UPROPERTY(EditAnywhere, Category="Editor Universe")
	AActor* DrawOrbitsRelativeTo;

	float EditorSimulationTickTimer = 0;
	
	/**
	 * @brief Registered orbitals to simulate. 
	 */
	TArray<class IOrbitalInterface*> Orbitals;
	
public:	
	/**
	 * @brief Default constructor.
	 */
	AUniverse();
	
	/**
	 * @brief Will be called every frame.
	 * @param DeltaTime 
	 */
	virtual void Tick(float DeltaTime) override;

	
	/**
     * @brief Returns a flag that determines whether the actor ticks within the viewport.
     * @return Flag, whether the actor should tick within the viewport.
     */
    virtual bool ShouldTickIfViewportsOnly() const override;

	/**
	 * @brief Returns the universal constants.
	 * @return Universal constants
	 */
	UUniversalConstants* GetConstants() const
	{
		return Constants;
	}

	/**
	 * @brief Registers a orbital to simulate.
	 * @param Orbital Orbital to simulate
	 */
	void Register(class IOrbitalInterface* Orbital);

	
	/**
	 * @brief Unregisters a orbital to no longer simulate.
	 * @param Orbital Orbital to stop simulating 
	 */
	void Unregister(class IOrbitalInterface* Orbital);

protected:
	/**
	 * @brief Will be called when the game starts.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Runs the N-body simulation during the game.
	 */
	virtual void Simulate();

	/**
	 * @brief Runs the N-body simulation in the editor.
	 */
	virtual void EditorSimulate();

private:
	/**
	 * @brief Returns all orbitals to be used within the editor simulation.
	 * @param OutReferenceFrameOrbital Reference frame orbital, this is the orbital the simulation should be drawn relative to
	 * @return All orbitals in the current map
	 */
	TArray<IOrbitalInterface*> GetEditorOrbitals(IOrbitalInterface*& OutReferenceFrameOrbital) const;
};

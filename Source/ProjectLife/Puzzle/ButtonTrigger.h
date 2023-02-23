// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "PuzzleTrigger.h"
#include "ButtonTrigger.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class PROJECTLIFE_API AButtonTrigger : public APuzzleTrigger
{
	GENERATED_BODY()
	
public:

	AButtonTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr<UStaticMeshComponent> Button;


	FTimeline TimelineHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		TObjectPtr<UCurveFloat> CurveForTimeline;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnButtonBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Door Open And Close*/
	void InitializeForTimeline();
	UFUNCTION()
		void TimelineFloatFunction(float Value);


	virtual void ButtonUp(); // virtual for ResetButton. See ResetButtonTrigger.h
	void ButtonDown();

	void ButtonMeshUp();
	void ButtonMeshDown();

	virtual void Trigger();
};

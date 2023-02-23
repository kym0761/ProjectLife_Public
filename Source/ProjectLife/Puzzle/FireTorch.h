// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsHold.h"
#include "FireTorch.generated.h"

class UNiagaraComponent;

/**
 *  ��ü���� ������ AFireStand�� ������. APhysicsHold�� ��ӹ޾ұ� ������ AFireStand�� ��ӹ��� ���Ͽ� ���� ����.
 */
UCLASS()
class PROJECTLIFE_API AFireTorch : public APhysicsHold, public ICombustible
{
	GENERATED_BODY()
	
public:

	AFireTorch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		TObjectPtr<UBoxComponent> FireOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		TObjectPtr<UNiagaraComponent> FireEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		bool bFireOn;

	void FireOn();

	void FireOff();

	virtual void Combust_Implementation();

	FTimerHandle CombustTimer;
	UFUNCTION()
		void OverlapCombust();
};

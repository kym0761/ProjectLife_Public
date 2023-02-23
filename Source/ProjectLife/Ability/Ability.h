// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityStruct.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

UCLASS()
class PROJECTLIFE_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		FAbilityDataStruct AbilityData;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability", Meta = (AllowPrivateAccess = "true"))
		float CurrentDurationTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability", Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> AbilityOwner;

	//�����Ƽ �� Target�� �� �Ѹ� -> ����) �� ������ ���� ���� ���� �ߵ� �����Ƽ�� �Ѹ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability", Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> AbilityTarget;

	FTimerHandle AbilityCountdownTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//temp to set owner in blueprint test
	UFUNCTION(BlueprintCallable)
	void SetAbilityOwner(AActor* Value);

	//�����Ƽ�� ���� ���� �κ�. ���� �׽�Ʈ�� ���ؼ� blueprintcallable ����
	UFUNCTION(BlueprintCallable)
	void StartAbility();

	/*�����Ƽ�� ActiveEffect, PassiveEffect, End , EndEfect�� �������Ʈ���� �����ǵ��� ��.*/
#pragma region Effect
	
	/*	�����Ƽ ó���� �ߵ��� ȿ���� ����
	*	���� : ó�� �ߵ��� ���� ������ �������� ��.
	*/
	UFUNCTION(BlueprintNativeEvent)
	void ActivateActiveEffect();
	virtual void ActivateActiveEffect_Implementation();

	/*	�����Ƽ�� �нú� ȿ���� ����
	*	���� : �����Ƽ�� �ִ� ���� ���� ��?
	*/
	UFUNCTION(BlueprintNativeEvent)
	void ActivatePassiveEffect();
	virtual void ActivatePassiveEffect_Implementation();

	/*	Actor�� Destroy�� �� EndPlay()���� �ߵ���.
	*	���� : �����Ƽ�� ����Ǹ� ���� �� ���� ȿ���� ������
	*/
	UFUNCTION(BlueprintNativeEvent)
	void EndAbility();
	virtual void EndAbility_Implementation();

	/*	�����Ƽ�� ����� �� �ߵ��� ȿ���� ����
	*	���� : �����Ƽ�� ����� �� ���� ������ �������� ��.
	*/
	UFUNCTION(BlueprintNativeEvent)
	void ActivateEndEffect();
	virtual void ActivateEndEffect_Implementation();
#pragma endregion

	//���� ���߰ų�, �ùٸ��� �ʰ� ȿ���� ����� �� �ߵ�
	UFUNCTION(BlueprintCallable)
	void DestroyAbilityWithoutEndEffect();
	
	//���������� ����� �� �ߵ�.
	UFUNCTION(BlueprintCallable)
	void DestroyAbilityWithEndEffect();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void AbilityEndTimerFunction();
};

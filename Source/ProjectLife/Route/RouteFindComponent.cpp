// Fill out your copyright notice in the Description page of Project Settings.


#include "RouteFindComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RouteNode.h"

// Sets default values for this component's properties
URouteFindComponent::URouteFindComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URouteFindComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URouteFindComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ARouteNode* URouteFindComponent::FindNearestRouteNode()
{
	AActor* owner = GetOwner();

	if (!IsValid(owner))
	{
		//Failed
		return nullptr;
	}

	TArray<AActor*> arr;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARouteNode::StaticClass(), arr);

	if (arr.Num() == 0)
	{
		//Failed
		return nullptr;
	}

	//owner�� ���� �Ÿ��� ����ؼ� ���� ����� ������ �迭�� ������.
	arr.Sort([owner](const AActor& a, const AActor& b)
		{
			float toA = FMath::Abs(owner->GetActorLocation().X - a.GetActorLocation().X)
				+ FMath::Abs(owner->GetActorLocation().Y - a.GetActorLocation().Y);
			float toB = FMath::Abs(owner->GetActorLocation().X - b.GetActorLocation().X)
				+ FMath::Abs(owner->GetActorLocation().Y - b.GetActorLocation().Y);
			return toA < toB;
		});

	ARouteNode* result = Cast<ARouteNode>(arr[0]);

	//result�� null�̵� ���� ����� Node�� return ��.
	return result;
}

TArray<ARouteNode*> URouteFindComponent::FindRoute(ARouteNode* Destination)
{
	//���� ����� ��带 �˻��ϰ�,�������� ã�������� �߰� ������ �˻�
	//����� - ��� - ��� -... - ������ ������ TArray<ARouteNode*> ��ȯ.


	//TODO : �Ǵ� ����
	//TODO : ���� ������ �� ��, �� - �� ���� �������� �����ϸ� ��� ������ �� ������ �߰��ؾ���.

	ARouteNode* first = FindNearestRouteNode();

	if (!IsValid(first) || !IsValid(Destination))
	{
		return TArray<ARouteNode*>();
	}

	//Ž���� ���� ť
	TArray<URouteInfo*> q;

	//�̹� Ž���� ��� üũ�뵵
	TArray<AActor*> visited;

	//���� ����
	URouteInfo* firstInfo = NewObject<URouteInfo>();
	firstInfo->RouteNode = first;
	firstInfo->RouteParentInfo = nullptr;
	firstInfo->F = 0;
	firstInfo->G = 0;

	q.Add(firstInfo);
	visited.Add(first);

	TArray<AActor*> nodeArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARouteNode::StaticClass(), nodeArr);

	URouteInfo* resultDest = nullptr;

	while (q.Num() > 0)
	{
		if (q.Num() > 1)
		{
			//���Ӱ� ���� ť�� �������� F�� ���� ���� �������� ������.
			q.HeapSort([](const URouteInfo& a, const URouteInfo& b)
				{
					return a.F < b.F;
				});
		}

		URouteInfo* nodeInfo = q.Pop();

		const float allowedDistance = 1000.0f;

		for (AActor* i : nodeArr)
		{
			//nodeinfo�� ���� ���� ����� ��ŵ.
			if (nodeInfo->RouteNode == i)
			{
				continue;
			}

			//�̹� �湮�ߴٸ� ��ŵ.
			if (visited.Find(i) != INDEX_NONE)
			{
				continue;
			}

			//�Ÿ� ����ؼ� ������ �Ÿ��� ���鸸 ������.
			float distance = FMath::Abs(nodeInfo->RouteNode->GetActorLocation().X - i->GetActorLocation().X)
				+ FMath::Abs(nodeInfo->RouteNode->GetActorLocation().Y - i->GetActorLocation().Y);

			if (distance > allowedDistance)
			{
				continue;
			}

			ARouteNode* current = Cast<ARouteNode>(i);
			// ���н� ��ŵ.
			if (!IsValid(current))
			{
				continue;
			}

			//��������� ������ ������ �����ϰ� ������ ����. q.Empty()�� �����ϸ� �ڵ����� while�� ����.
			if (current == Destination)
			{
				resultDest = NewObject<URouteInfo>();
				resultDest->G = distance;
				resultDest->F = nodeInfo->F + resultDest->G;
				resultDest->RouteNode = Destination;
				resultDest->RouteParentInfo = nodeInfo;
				q.Empty();
				break;
			}

			//�����ߴٸ� ť�� �ִ´�.
			URouteInfo* nextNodeInfo = NewObject<URouteInfo>();
			nextNodeInfo->G = distance;

			//F���� ��� ������ ������?
			nextNodeInfo->F = nodeInfo->F + nextNodeInfo->G;
			nextNodeInfo->RouteNode = current;
			nextNodeInfo->RouteParentInfo = nodeInfo;
			q.Add(nextNodeInfo);

			//���� ��带 �湮 üũ��.
			visited.Add(current);


		}

	}

	//��Ʈ �����
	TArray<ARouteNode*> routeResult;
	
	//Parent�� �������� ���������� ����Ž����
	URouteInfo* temp = resultDest;
	while (IsValid(temp))
	{
		routeResult.Add(temp->RouteNode);
		temp = temp->RouteParentInfo;
	}

	//routeResult ������ �������.
	Algo::Reverse(routeResult);
	
	return routeResult;
}


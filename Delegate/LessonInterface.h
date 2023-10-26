// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LessonInterface.generated.h"


// 딱히 뭘할게 없는 타입정보를 보관할 클래스
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULessonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALDELEGATE_API ILessonInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DoLesson()
	{
		UE_LOG(LogTemp, Log, TEXT("수업에 입장 합니다."));
	}
};
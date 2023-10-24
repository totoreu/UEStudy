// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

UMyGameInstance::UMyGameInstance()
{
    // CDO의 기본값을 설정하는 코드
    SchoolName = TEXT("기본학교");
}

void UMyGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("==========================="));
    TArray<UPerson*> Persons = {NewObject<UStudent>(), NewObject<UTeacher>(), NewObject<UStaff>()};
    for (const auto Person : Persons)
    {
        UE_LOG(LogTemp, Log, TEXT("구성원 이름 : %s"), *Person->GetName());
    }
    UE_LOG(LogTemp, Log, TEXT("==========================="));

    for (const auto Person : Persons)
    {
        // ILessonIterface를 상속 받았는지 안받았는지 캐스팅으로 확인 가능함. 상속받지않았으면 null값을 반환하기 때문
        ILessonInterface* LessonIterface = Cast<ILessonInterface>(Person);
        if (LessonIterface)
        {
            UE_LOG(LogTemp, Log, TEXT("%s님은 수업에 참여할 수 있습니다."), *Person->GetName());
            LessonIterface->DoLesson();
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("%s님은 수업에 참여할 수 없습니다."), *Person->GetName());
        }
    }
}

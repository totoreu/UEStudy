// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Student.h"
#include "Teacher.h"

UMyGameInstance::UMyGameInstance()
{
    // CDO의 기본값을 설정하는 코드
    SchoolName = TEXT("기본학교");
}

void UMyGameInstance::Init()
{
    Super::Init();

    UStudent* Student = NewObject<UStudent>();
    UTeacher* Teacher = NewObject<UTeacher>();
    Student->SetName(TEXT("신현승"));
    UE_LOG(LogTemp, Log, TEXT("새로운 학생 이름 %s"), *Student->GetName());

    FString CurrentTeacherName;
    FString NewTeacherName(TEXT("득쌤"));
    FProperty* NameProp = UTeacher::StaticClass()->FindPropertyByName(TEXT("Name"));
    if (NameProp)
    {
        NameProp->GetValue_InContainer(Teacher, &CurrentTeacherName);
        UE_LOG(LogTemp, Log, TEXT("현재 선생님 이름 %s"), *CurrentTeacherName);

        NameProp->SetValue_InContainer(Teacher, &NewTeacherName);
        UE_LOG(LogTemp, Log, TEXT("새로운 선생님 이름 % s"), *Teacher->GetName());
    }

    UE_LOG(LogTemp, Log, TEXT("=================================="));

    Student->DoLesson();
    UFunction* DoLessonFunc = Teacher->GetClass()->FindFunctionByName(TEXT("DoLesson"));
    if (DoLessonFunc)
    {
        Teacher->ProcessEvent(DoLessonFunc, nullptr);
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "JsonObjectConverter.h"

void PrintStudentInfo(const UStudent* InStudent, const FString& InTag)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] 이름 : %s, 순번 : %d"), *InTag, *InStudent->GetName(), InStudent->GetOrder());
}

UMyGameInstance::UMyGameInstance()
{
}

void UMyGameInstance::Init()
{
    Super::Init();

    FStudentData RawDataSrc(16, TEXT("신현승"));

    const FString SavedDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
    UE_LOG(LogTemp, Log, TEXT("저장할 파일 폴더 : %s"), *SavedDir);

    {
        const FString RawDataFileName(TEXT("RawData.bin"));
        FString RawDataAbosolutePath = FPaths::Combine(*SavedDir, *RawDataFileName);
        UE_LOG(LogTemp, Log, TEXT("저장할 파일 전체 경로 : %s"), *RawDataAbosolutePath);
        FPaths::MakeStandardFilename(RawDataAbosolutePath);
        UE_LOG(LogTemp, Log, TEXT("변경할 파일 전체 경로 : %s"), *RawDataAbosolutePath);

        FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*RawDataAbosolutePath);
        if (RawFileWriterAr)
        {
            *RawFileWriterAr << RawDataSrc;
            RawFileWriterAr->Close();
            delete RawFileWriterAr;
            RawFileWriterAr = nullptr;
            UE_LOG(LogTemp, Log, TEXT("쓰기 확인"));
        }

        FStudentData RawDataDest;
        FArchive* RawFileReaderAr = IFileManager::Get().CreateFileReader(*RawDataAbosolutePath);
        if (RawFileReaderAr)
        {
            *RawFileReaderAr << RawDataDest;
            RawFileReaderAr->Close();
            delete RawFileReaderAr;
            RawFileReaderAr = nullptr;
            UE_LOG(LogTemp, Log, TEXT("[RawData] 이름 : %s , 순번 : %d"), *RawDataDest.Name, RawDataDest.Order);
        }
    }

    StudentSrc = NewObject<UStudent>();
    StudentSrc->SetName(TEXT("현승맨"));
    StudentSrc->SetOrder(59);

    {
        const FString ObjectDataFileName(TEXT("ObjectData.bin"));
        FString ObjectDataAbsolutePath = FPaths::Combine(*SavedDir, *ObjectDataFileName);
        FPaths::MakeStandardFilename(ObjectDataAbsolutePath);

        TArray<uint8> BufferArray;
        FMemoryWriter MemoryWriterAr(BufferArray);
        StudentSrc->Serialize(MemoryWriterAr);
        TUniquePtr<FArchive> FileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*ObjectDataAbsolutePath));
        if (FileWriterAr)
        {
            *FileWriterAr << BufferArray;
            FileWriterAr->Close();
        }

        TArray<uint8> BufferArrayFromFile;
        TUniquePtr<FArchive> FileReaderAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*ObjectDataAbsolutePath));
        if (FileReaderAr)
        {
            *FileReaderAr << BufferArrayFromFile;
            FileReaderAr->Close();
        }

        FMemoryReader MemoryReaderAr(BufferArrayFromFile);
        UStudent* StudentDest = NewObject<UStudent>();
        StudentDest->Serialize(MemoryReaderAr);
        PrintStudentInfo(StudentDest, TEXT("ObjectData"));
    }

    {
        const FString JsonDataFileName(TEXT("StudentJsonData.txt"));
        FString JsonDataAbsolutePath = FPaths::Combine(*SavedDir, *JsonDataFileName);
        FPaths::MakeStandardFilename(JsonDataAbsolutePath);

        TSharedRef<FJsonObject> JsonObjectSrc = MakeShared<FJsonObject>();
        FJsonObjectConverter::UStructToJsonObject(StudentSrc->GetClass(), StudentSrc, JsonObjectSrc);

        FString JsonOutString;
        TSharedRef<TJsonWriter<TCHAR>> JsonWriterAr = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
        if (FJsonSerializer::Serialize(JsonObjectSrc, JsonWriterAr))
        {
            FFileHelper::SaveStringToFile(JsonOutString, *JsonDataAbsolutePath);
        }

        FString JsonInString;
        FFileHelper::LoadFileToString(JsonInString, *JsonDataAbsolutePath);

        TSharedRef<TJsonReader<TCHAR>> JsonReaderAr = TJsonReaderFactory<TCHAR>::Create(JsonInString);

        TSharedPtr<FJsonObject> JsonObjectDest;
        if (FJsonSerializer::Deserialize(JsonReaderAr, JsonObjectDest))
        {
            UStudent* JsonStudentDest = NewObject<UStudent>();
            if(FJsonObjectConverter::JsonObjectToUStruct(JsonObjectDest.ToSharedRef(), JsonStudentDest->GetClass(), JsonStudentDest))
            {
                PrintStudentInfo(JsonStudentDest, TEXT("JsonData"));
            }
        }
    }
}
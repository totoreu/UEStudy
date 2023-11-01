// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "JsonObjectConverter.h"
#include "UObject/SavePackage.h"

// 패키지 폴더 경로 및 파일 이름
const FString UMyGameInstance::PackageName = TEXT("/Game/Student");
// 페키지 asset 이름
const FString UMyGameInstance::AssetName = TEXT("TopStudent");

void PrintStudentInfo(const UStudent* InStudent, const FString& InTag)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] 이름 : %s, 순번 : %d"), *InTag, *InStudent->GetName(), InStudent->GetOrder());
}

UMyGameInstance::UMyGameInstance()
{
    const FString TopSoftObjectPath = FString::Printf(TEXT("%s.%s"), *PackageName, *AssetName);
    static ConstructorHelpers::FObjectFinder<UStudent> UASSET_TopStudent(*TopSoftObjectPath);
    if (UASSET_TopStudent.Succeeded())
    {
        PrintStudentInfo(UASSET_TopStudent.Object, TEXT("Constructor"));
    }
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

    // 패키지를 사용해 에셋 저장하기

    SaveStudentPackage();
    //LoadStudentPackage();
    //LoadStudentObject();

    const FString TopSoftObjectPath = FString::Printf(TEXT("%s.%s"), *PackageName, *AssetName);
    Handle = StreamableManager.RequestAsyncLoad(TopSoftObjectPath,
        [&]()
        {
            if (Handle.IsValid() && Handle->HasLoadCompleted())
            {
                UStudent* TopStudent = Cast<UStudent>(Handle->GetLoadedAsset());
                if (TopStudent)
                {
                    PrintStudentInfo(TopStudent, TEXT("AsyncLoad"));

                    Handle->ReleaseHandle();
                    Handle.Reset();
                }
            }
        });
}

void UMyGameInstance::SaveStudentPackage() const
{
    // 만약 패키지가 있다면 모두 로딩후 저장하도록
    UPackage* StudentPackage = ::LoadPackage(nullptr, *PackageName, LOAD_None);
    if (StudentPackage)
    {
        StudentPackage->FullyLoad();
    }

    // 패키지 생성
    StudentPackage = CreatePackage(*PackageName);
    // RF_Public 공개적으로 접근 가능 / RF_Standalone 다른 객체에 의존하지 않고 독립적으로 동작
    EObjectFlags ObjectFlag = RF_Public | RF_Standalone;
    // UStudent의 UClass를 반환하여 해당 클래스에 대한 정보와 동작을 조작가능할 수 있게 해준다.
    UStudent* TopStudent = NewObject<UStudent>(StudentPackage, UStudent::StaticClass(), *AssetName, ObjectFlag);
    TopStudent->SetName(TEXT("신현승"));
    TopStudent->SetOrder(36);

    const int32 NumofSubs = 10;
    for (int32 ix = 1; ix <= NumofSubs; ++ix)
    {
        FString SubObjectName = FString::Printf(TEXT("Student%d"), ix);
        UStudent* SubStudent = NewObject<UStudent>(TopStudent, UStudent::StaticClass(), *SubObjectName, ObjectFlag);
        SubStudent->SetName(FString::Printf(TEXT("학생%d"),ix));
        SubStudent->SetOrder(ix);
    }

    // 확장자를 부여하기위한 함수
    const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = ObjectFlag;

    if (UPackage::SavePackage(StudentPackage, nullptr, *PackageFileName, SaveArgs))
    {
        UE_LOG(LogTemp, Log, TEXT("패키지가 성공적으로 저장되었습니다."));
    }
}

void UMyGameInstance::LoadStudentPackage() const
{
                                // 패키지 정보 없음, 패키지 이름, 로드 기본옵션
    UPackage* StudentPackage = ::LoadPackage(nullptr, *PackageName, LOAD_None);
    if (nullptr == StudentPackage)
    {
        UE_LOG(LogTemp, Log, TEXT("패키지를 찾을 수 없습니다"));
        return;
    }

    StudentPackage->FullyLoad();

    UStudent* TopStudent = FindObject<UStudent>(StudentPackage, *AssetName);
    PrintStudentInfo(TopStudent, TEXT("FindObject Asset"));
}

void UMyGameInstance::LoadStudentObject() const
{
    const FString TopSoftObjectPath = FString::Printf(TEXT("%s.%s"), *PackageName, *AssetName);

    UStudent* TopStudent = LoadObject<UStudent>(nullptr, *TopSoftObjectPath);
    PrintStudentInfo(TopStudent, TEXT("LoadObjectAsset"));
}

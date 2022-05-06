#include "ZipFile.h"

#include "utils/stream_utils.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <functional>
namespace
{
std::string GetFilenameFromPath(const std::string& fullPath)
{
    std::string::size_type dirSeparatorPos;
    
    if ((dirSeparatorPos = fullPath.find_last_of('/')) != std::string::npos)
    {
        return fullPath.substr(dirSeparatorPos + 1);
    }
    else
    {
        return fullPath;
    }
}

std::string MakeTempFilename(const std::string& fileName)
{
    return fileName + ".tmp";
}
}

ZipArchive::Ptr ZipFile::Open(const std::string& zipPath)
{
    std::ifstream* zipFile = new std::ifstream();
    zipFile->open(zipPath, std::ios::binary);
    
    if (!zipFile->is_open())
    {
        // if file does not exist, try to create it
        std::ofstream tmpFile;
        tmpFile.open(zipPath, std::ios::binary);
        tmpFile.close();
        
        zipFile->open(zipPath, std::ios::binary);
        
        // if attempt to create file failed, return null
        if (!zipFile->is_open())
        {
            return nullptr;
        }
    }
    
    return ZipArchive::Create(zipFile, true);
}

void ZipFile::Save(ZipArchive::Ptr zipArchive, const std::string& zipPath)
{
    ZipFile::SaveAndClose(zipArchive, zipPath);
    
    zipArchive = ZipFile::Open(zipPath);
}

bool ZipFile::SaveAndClose(ZipArchive::Ptr zipArchive, const std::string& zipPath)
{
    // check if file exist
    std::string tempZipPath = MakeTempFilename(zipPath);
    std::ofstream outZipFile;
    outZipFile.open(tempZipPath, std::ios::binary | std::ios::trunc);
    
    if (!outZipFile.is_open())
    {
        return false;
    }
    
    zipArchive->WriteToStream(outZipFile);
    outZipFile.close();
    
    zipArchive->InternalDestroy();
    
    remove(zipPath.c_str());
    rename(tempZipPath.c_str(), zipPath.c_str());
    return true;
}

bool ZipFile::IsInArchive(const std::string& zipPath, const std::string& fileName)
{
    ZipArchive::Ptr zipArchive = ZipFile::Open(zipPath);
    if (!zipArchive)
        return false;
    return zipArchive->GetEntry(fileName) != nullptr;
}

zip_helper::ZIPResult ZipFile::AddFile(const std::string& zipPath, const std::string& fileName, ICompressionMethod::Ptr method)
{
    return AddFile(zipPath, fileName, GetFilenameFromPath(fileName), method);
}

zip_helper::ZIPResult ZipFile::AddFile(const std::string& zipPath, const std::string& fileName, const std::string& inArchiveName, ICompressionMethod::Ptr method)
{
    return AddEncryptedFile(zipPath, fileName, inArchiveName, std::string(), method);
}

zip_helper::ZIPResult ZipFile::AddEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& password, ICompressionMethod::Ptr method)
{
    return AddEncryptedFile(zipPath, fileName, GetFilenameFromPath(fileName), password, method);
}


zip_helper::ZIPResult ZipFile::AddEncryptedFiles(const std::string& zipPath, const std::vector<FileInfo>& fileInfos)
{
    std::string tmpName = MakeTempFilename(zipPath);
    
    {
        ZipArchive::Ptr zipArchive = ZipFile::Open(zipPath);
        if (!zipArchive)
            return zip_helper::ZIPResult::INPUT_FILE_OPEN_FAILED;
        
        std::vector<std::ifstream*> fileToAddVector;
        std::vector<std::istringstream*> stringToAddVector;
        
        std::unique_ptr<char, std::function<void(char*)>> _(new char,[&fileToAddVector, &stringToAddVector](char*p)->void{
            for (auto* pointer : fileToAddVector)
            {
                delete pointer;
            }
            fileToAddVector.clear();
            
            
            for (auto* pointer : stringToAddVector)
            {
                delete pointer;
            }
            stringToAddVector.clear();
            
            delete p;
        });
        
        //std::ifstream fileToAdd[fileInfos.size()];
        //std::istringstream stringToAdd[fileInfos.size()];
        
        int index = -1;
        for (const FileInfo& fileInfo : fileInfos)
        {
            index ++;
            std::ifstream* fileToAdd = new std::ifstream;// = fileToAdd[index];
            std::ifstream& fileToAdd_ = *fileToAdd;
            fileToAddVector.emplace_back(fileToAdd);
            
            std::istringstream* stringToAdd = new std::istringstream;// = stringToAdd[index];
            std::istringstream& stringToAdd_ = *stringToAdd;// = fileToAdd[index];
            stringToAddVector.emplace_back(stringToAdd);
            
            if (fileInfo.isFile)
            {
                fileToAdd_.open(fileInfo.content, std::ios::binary);
                
                if (!fileToAdd_.is_open())
                {
                    return zip_helper::ZIPResult::INPUT_FILE_OPEN_FAILED;
                }
            }
            else
            {
                stringToAdd_.str(fileInfo.content);
            }
            
            
            auto fileEntry = zipArchive->CreateEntry(fileInfo.inArchiveName);
            
            if (fileEntry == nullptr)
            {

                zipArchive->RemoveEntry(fileInfo.inArchiveName);
                fileEntry = zipArchive->CreateEntry(fileInfo.inArchiveName);
            }
            
            if (!fileInfo.password.empty())
            {
                fileEntry->SetPassword(fileInfo.password);
                fileEntry->UseDataDescriptor();
            }
            if (fileInfo.isFile)
                fileEntry->SetCompressionStream(fileToAdd_, fileInfo.method);
            else
                fileEntry->SetCompressionStream(stringToAdd_, fileInfo.method);
            
            //////////////////////////////////////////////////////////////////////////
            
        }
        
        std::ofstream outFile;
        outFile.open(tmpName, std::ios::binary);
        
        if (!outFile.is_open())
        {
            return zip_helper::ZIPResult::OUTPUT_FILE_OPEN_FAILED;
        }
        
        zipArchive->WriteToStream(outFile);
        outFile.close();
        
        // force closing the input zip stream
    }
    
    remove(zipPath.c_str());
    rename(tmpName.c_str(), zipPath.c_str());
    return zip_helper::ZIPResult::SUCCESS;
}

zip_helper::ZIPResult ZipFile::AddEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& inArchiveName, const std::string& password, ICompressionMethod::Ptr method)
{
    std::string tmpName = MakeTempFilename(zipPath);
    
    {
        ZipArchive::Ptr zipArchive = ZipFile::Open(zipPath);
        if (!zipArchive)
            return zip_helper::ZIPResult::INPUT_FILE_OPEN_FAILED;
        
        std::ifstream fileToAdd;
        fileToAdd.open(fileName, std::ios::binary);
        
        if (!fileToAdd.is_open())
        {
            return zip_helper::ZIPResult::INPUT_FILE_OPEN_FAILED;
        }
        
        auto fileEntry = zipArchive->CreateEntry(inArchiveName);
        
        if (fileEntry == nullptr)
        {

            zipArchive->RemoveEntry(inArchiveName);
            fileEntry = zipArchive->CreateEntry(inArchiveName);
        }
        
        if (!password.empty())
        {
            fileEntry->SetPassword(password);
            fileEntry->UseDataDescriptor();
        }
        
        fileEntry->SetCompressionStream(fileToAdd, method);
        
        //////////////////////////////////////////////////////////////////////////
        
        std::ofstream outFile;
        outFile.open(tmpName, std::ios::binary);
        
        if (!outFile.is_open())
        {
            return zip_helper::ZIPResult::OUTPUT_FILE_OPEN_FAILED;
        }
        
        zipArchive->WriteToStream(outFile);
        outFile.close();
        
        // force closing the input zip stream
    }
    
    remove(zipPath.c_str());
    rename(tmpName.c_str(), zipPath.c_str());
    return zip_helper::ZIPResult::SUCCESS;
}

zip_helper::ZIPResult ZipFile::ExtractFile(const std::string& zipPath, const std::string& fileName)
{
    return ExtractFile(zipPath, fileName, GetFilenameFromPath(fileName));
}

zip_helper::ZIPResult ZipFile::ExtractFile(const std::string& zipPath, const std::string& fileName, const std::string& destinationPath)
{
    return ExtractEncryptedFile(zipPath, fileName, destinationPath, std::string());
}

zip_helper::ZIPResult ZipFile::ExtractEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& password)
{
    return ExtractEncryptedFile(zipPath, fileName, GetFilenameFromPath(fileName), password);
}

zip_helper::ZIPResult ZipFile::ExtractEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& destinationPath, const std::string& password)
{
    ZipArchive::Ptr zipArchive = ZipFile::Open(zipPath);
    if (!zipArchive)
        return zip_helper::ZIPResult::INPUT_FILE_OPEN_FAILED;
    
    std::ofstream destFile;
    destFile.open(destinationPath, std::ios::binary | std::ios::trunc);
    
    if (!destFile.is_open())
    {
        return zip_helper::ZIPResult::OUTPUT_FILE_OPEN_FAILED;
    }
    
    auto entry = zipArchive->GetEntry(fileName);
    
    if (entry == nullptr)
    {
        return zip_helper::ZIPResult::NO_ENTRY;
    }
    
    if (!password.empty())
    {
        entry->SetPassword(password);
    }
    
    std::istream* dataStream = entry->GetDecompressionStream();
    
    if (dataStream == nullptr)
    {
        return zip_helper::ZIPResult::GET_ENTRY_STREAM_FAILED;
    }
    
    utils::stream::copy(*dataStream, destFile);
    
    
    
    destFile.flush();
    destFile.close();
    entry->CloseDecompressionStream();
    return zip_helper::ZIPResult::SUCCESS;
}


zip_helper::ZIPResult ZipFile::RemoveEntry(const std::string& zipPath, const std::string& fileName)
{
    std::string tmpName = MakeTempFilename(zipPath);
    
    {
        ZipArchive::Ptr zipArchive = ZipFile::Open(zipPath);
        if (!zipArchive)
            return zip_helper::ZIPResult::INPUT_FILE_OPEN_FAILED;
        zipArchive->RemoveEntry(fileName);
        
        //////////////////////////////////////////////////////////////////////////
        
        std::ofstream outFile;
        
        outFile.open(tmpName, std::ios::binary);
        
        if (!outFile.is_open())
        {
            return zip_helper::ZIPResult::OUTPUT_FILE_OPEN_FAILED;
        }
        
        zipArchive->WriteToStream(outFile);
        outFile.close();
        
        // force closing the input zip stream
    }
    
    remove(zipPath.c_str());
    rename(tmpName.c_str(), zipPath.c_str());
    return zip_helper::ZIPResult::SUCCESS;
}

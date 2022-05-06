#pragma once
#include "ZipArchive.h"

#include <string>
#include <memory>
#include "ZipEnums.h"
#include <vector>
/**
 * \brief Provides static methods for creating, extracting, and opening zip archives.
 */
class ZipFile
{

public:
    struct FileInfo
    {
        FileInfo(bool _isFile, const std::string& _content, const std::string& _inArchiveName, const std::string& _password, ICompressionMethod::Ptr _method = StoreMethod::Create())
        :isFile(_isFile), content(_content), inArchiveName(_inArchiveName), password(_password), method(_method)
        {
            
        }
        bool isFile = false;
        std::string content; //file or string
        std::string inArchiveName;
        std::string password;
        ICompressionMethod::Ptr method = DeflateMethod::Create();
    };
  // TODO:
  // CreateFromDirectory + compression level + opening to/from stream support
  // ExtractToDirectory

  public:
    /**
     * \brief Opens the zip archive file with the given filename.
     *
     * \param zipPath Full pathname of the zip file.
     *
     * \return The ZipArchive instance.
     */
    static ZipArchive::Ptr Open(const std::string& zipPath);

    /**
     * \brief Saves the zip archive file with the given filename.
     *        The ZipArchive class will stay open.
     *
     * \param zipArchive  The zip archive to save.
     * \param zipPath     Full pathname of the zip archive file.
     */
    static void Save(ZipArchive::Ptr zipArchive, const std::string& zipPath);

    /**
     * \brief Saves the zip archive file and close it.
     *        The ZipArchive class will be clear after this method call.
     *
     * \param zipArchive  The zip archive to save.
     * \param zipPath     Full pathname of the zip archive file.
     */
    static bool SaveAndClose(ZipArchive::Ptr zipArchive, const std::string& zipPath);

    /**
     * \brief Checks if file with the given path is contained in the archive.
     *
     * \param zipPath   Full pathname of the zip file.
     * \param fileName  Filename or the path of the file to check.
     *
     * \return  true if in archive, false if not.
     */
    static bool IsInArchive(const std::string& zipPath, const std::string& fileName);

    /**
     * \brief Adds a file to the zip archive.
     *        The name of the file in the archive will be the same as the added file name.
     *
     * \param zipPath   Full pathname of the zip file.
     * \param fileName  Filename of the file to add.
     * \param method     (Optional) The level of compression. Use StoreMethod::Create() for no compression.
     */
    static zip_helper::ZIPResult AddFile(const std::string& zipPath, const std::string& fileName, ICompressionMethod::Ptr method = DeflateMethod::Create());
    
    /**
     * \brief Adds a file to the zip archive.
     *
     * \param zipPath       Full pathname of the zip file.
     * \param fileName      Filename of the file to add.
     * \param inArchiveName Final name of the file in the archive.
     * \param method         (Optional) The level of compression. Use StoreMethod::Create() for no compression.
     */
    static zip_helper::ZIPResult AddFile(const std::string& zipPath, const std::string& fileName, const std::string& inArchiveName, ICompressionMethod::Ptr method = DeflateMethod::Create());

    /**
     * \brief Adds an encrypted file to the zip archive.
     *        The name of the file in the archive will be the same as the added file name.
     *
     * \param zipPath   Full pathname of the zip file.
     * \param fileName  Filename of the file to add.
     * \param password  The password.
     * \param method         (Optional) The level of compression. Use StoreMethod::Create() for no compression.
     */
    static zip_helper::ZIPResult AddEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& password, ICompressionMethod::Ptr method = DeflateMethod::Create());

    /**
     * \brief Adds an encrypted file to the zip archive.
     *
     * \param zipPath       Full pathname of the zip file.
     * \param fileName      Filename of the file to add.
     * \param inArchiveName Final name of the file in the archive.
     * \param password      The password.
     * \param method         (Optional) The level of compression. Use StoreMethod::Create() for no compression.
     */
    static zip_helper::ZIPResult AddEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& inArchiveName, const std::string& password, ICompressionMethod::Ptr method = DeflateMethod::Create());


    static zip_helper::ZIPResult AddEncryptedFiles(const std::string& zipPath, const std::vector<FileInfo>& fileInfos);

    /**
     * \brief Extracts the file from the zip archive.
     *        The extracted filename will have the same file name as the name of the file in the archive.
     *
     * \param zipPath   Full pathname of the zip file.
     * \param fileName  Filename of the file to extract.
     */
    static zip_helper::ZIPResult ExtractFile(const std::string& zipPath, const std::string& fileName);

    /**
     * \brief Extracts the file from the zip archive.
     *
     * \param zipPath         Full pathname of the zip file.
     * \param fileName        Filename of the file in the archive.
     * \param destinationPath Full pathname of the extracted file.
     */
    static zip_helper::ZIPResult ExtractFile(const std::string& zipPath, const std::string& fileName, const std::string& destinationPath);
    


    /**
     * \brief Extracts an encrypted file from the zip archive.
     *        The extracted filename will have the same file name as the name of the file in the archive.
     *
     * \param zipPath   Full pathname of the zip file.
     * \param fileName  Filename of the file to extract.
     * \param password  The password.
     */
    static zip_helper::ZIPResult ExtractEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& password);

    /**
     * \brief Extracts an encrypted file from the zip archive.
     *
     * \param zipPath         Full pathname of the zip file.
     * \param fileName        Filename of the file to extract.
     * \param destinationPath Full pathname of the extracted file.
     * \param password        The password.
     */
    static zip_helper::ZIPResult ExtractEncryptedFile(const std::string& zipPath, const std::string& fileName, const std::string& destinationPath, const std::string& password);

    /**
     * \brief Removes the file from the zip archive.
     *
     * \param zipPath   Full pathname of the zip file.
     * \param fileName  Filename of the file to remove.
     */
    static zip_helper::ZIPResult RemoveEntry(const std::string& zipPath, const std::string& fileName);
};

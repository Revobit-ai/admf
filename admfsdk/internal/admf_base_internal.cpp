//
//  admf_base_internal.cpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#include "admf_internal_header.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#if (defined _WIN32) || (defined __APPLE__)
#include <filesystem>
#else
#include <sys/stat.h> 
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#endif
#include <cstdio>
#include <string>
#include "bson.h"
#ifndef __APPLE__
#include <assert.h>
#endif

#include <stdio.h>
#include <stdlib.h>
using namespace admf_internal;
using namespace admf;

void String_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
    str_ = getBsonString(iter);
}

void String_internal::initMissed()
{
}
#ifdef ADMF_EDIT
void String_internal::save(bson_t *doc)
{
    //nothing to do
}
#endif
ADMF_UINT String_internal::getLength()
{
    return (ADMF_UINT)str_.length();
}

bool String_internal::isEmpty()
{
     return str_.length() == 0;
}

ADMF_UINT String_internal::getString(ADMF_CHAR *buff, ADMF_UINT len)
{
    if (buff == nullptr)
        return 0;

    ADMF_UINT len_ = std::min((ADMF_UINT)(len - 1), (ADMF_UINT)str_.length());
    strncpy(buff, str_.c_str(), len_);

    buff[len_] = 0;
    return len_ + 1;
}


void StringReadOnly_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
    str_ = getBsonString(iter);
}

void StringReadOnly_internal::initMissed()
{
}
#ifdef ADMF_EDIT
void StringReadOnly_internal::save(bson_t *doc)
{
    //nothing to do
}
#endif
ADMF_UINT StringReadOnly_internal::getLength()
{
    return (ADMF_UINT)str_.length();
}

bool StringReadOnly_internal::isEmpty()
{
    return str_.length() == 0;
}

ADMF_UINT StringReadOnly_internal::getString(ADMF_CHAR *buff, ADMF_UINT len)
{
    if (buff == nullptr)
        return 0;
    
    ADMF_UINT len_ = std::min((ADMF_UINT)(len - 1), (ADMF_UINT)str_.length());
    strncpy(buff, str_.c_str(), len_);
    
    buff[len_] = 0;
    return len_ + 1;
}

void Vec2_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string xKey = getNewKey("x");
    std::string yKey = getNewKey("y");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == xKey)
        {
            x_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == yKey)
        {
            y_ = (ADMF_FLOAT)bson_iter_as_double(&child);

        }
    }
}

void Vec2_internal::initMissed()
{
}
#ifdef ADMF_EDIT
void Vec2_internal::save(bson_t *doc)
{

    std::string xKey = getNewKey("x");
    std::string yKey = getNewKey("y");
    ADMF_BSON_APPEND_DOUBLE(doc, xKey, x_);
    ADMF_BSON_APPEND_DOUBLE(doc, yKey, y_);
}
#endif

ADMF_FLOAT Vec2_internal::getX()
{
    return x_;
}

ADMF_FLOAT Vec2_internal::getY()
{
    return y_;
}

void ColorRGB_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string rKey = getNewKey("r");
    std::string gKey = getNewKey("g");
    std::string bKey = getNewKey("b");
    std::string colorSpaceKey = getNewKey("colorSpace");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == rKey)
        {
            r_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == gKey)
        {
            g_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == bKey)
        {
            b_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == colorSpaceKey)
        {
            colorSpace_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
    }
}

void ColorRGB_internal::initMissed()
{
    if (!colorSpace_)
        colorSpace_ = std::make_shared<String_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void ColorRGB_internal::save(bson_t *doc)
{

    std::string rKey = getNewKey("r");
    std::string gKey = getNewKey("g");
    std::string bKey = getNewKey("b");
    std::string colorSpaceKey = getNewKey("colorSpace");

    ADMF_BSON_APPEND_DOUBLE(doc, rKey, r_);
    ADMF_BSON_APPEND_DOUBLE(doc, gKey, g_);
    ADMF_BSON_APPEND_DOUBLE(doc, bKey, b_);
    ADMF_BSON_APPEND_STRING(doc, colorSpaceKey, colorSpace_);
}
#endif
ADMF_FLOAT ColorRGB_internal::getR()
{
    return r_;
}

ADMF_FLOAT ColorRGB_internal::getG()
{
    return g_;
}

ADMF_FLOAT ColorRGB_internal::getB()
{
    return b_;
}

String ColorRGB_internal::getColorSpace()
{
    return String(colorSpace_);
}

void BinaryData_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    name_ = std::make_shared<String_internal>(admfIndex_, iter);
    if (name_->getInternalString().empty())
        return;

    ADMF_internal *admf_internal = ADMF_internal::getADMFInternalByIndex(admfIndex_);
    if (!admf_internal)
        return;

    auto zipEntry = admf_internal->getZipArchiveEntry(name_->getInternalString());
    if (!zipEntry)
        return;
    
    ADMF_internal::addZipEntry(admfIndex_, baseIndex_, zipEntry);

    originInfo_.originType = OriginType::ZipEntry;
}

void BinaryData_internal::initMissed()
{
    if (!name_ || name_->isEmpty())
        name_ = std::make_shared<String_internal>(admfIndex_);
    if (!assignedName_)
        assignedName_ = std::make_shared<String_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void BinaryData_internal::save(bson_t *doc)
{

    ADMF_internal *admf_internal = ADMF_internal::getADMFInternalByIndex(admfIndex_);
    assert(admf_internal != nullptr);
    if (!admf_internal)
        return;

    switch (originInfo_.originType)
    {
    case OriginType::ZipEntry:
    {
        auto zipEntry = getZipEntry();
        assert(zipEntry);
        if (!zipEntry)
            return;



#if (defined _WIN32) || (defined __APPLE__)
		char tempBuff[L_tmpnam + 1] = { 0 };
		const char* tmpFileName = std::tmpnam(tempBuff);
#else
		char tmpFileName[PATH_MAX + 1] = "/tmp/admf_XXXXXX";
		int fileHandle = mkstemp(tmpFileName);
		if (fileHandle < 0)
            return;
		close(fileHandle);
#endif
		assert(tmpFileName);
		if (tmpFileName == nullptr)
            return;

        ADMF_RESULT result = exportToFile(tmpFileName);
        assert(result == ADMF_SUCCESS);
        if (result != ADMF_SUCCESS)
            return;

        admf_internal->addBinaryFileEntry(tmpFileName, getNameString(), true);
    }
    break;

    case OriginType::File:
    {
        assert(!originInfo_.content.empty());
        if (originInfo_.content.empty())
            return;

        admf_internal->addBinaryFileEntry(originInfo_.content, getNameString(), originInfo_.needDeleteFile);
    }
    break;

    case OriginType::Data:
    {
        admf_internal->addDataEntry(originInfo_.content, getNameString());
    }
    break;

    default:
        return;
    }

    //BSON_APPEND_UTF8留给上一层结构， 因为在这里不知道key是什么
}
#endif

ADMF_UINT BinaryData_internal::getDataLength()
{

    switch (originInfo_.originType)
    {
    case OriginType::ZipEntry:
        {
            auto zipEntry = getZipEntry();
            if (!zipEntry)
                return 0;
            
            return (ADMF_UINT)zipEntry->GetSize();
        }

    case OriginType::File:
        if (originInfo_.content.empty())
            return 0;
        return (ADMF_UINT)filesize(originInfo_.content);

    case OriginType::Data:
        return (ADMF_UINT)originInfo_.content.length();

    default:
        return 0;
    }
}

std::istream*  BinaryData_internal::getCompressedStream()
{
    auto zipEntry = getZipEntry();
    if (!zipEntry)
        return nullptr;
    
    return zipEntry->GetRawStream();
}

ADMF_UINT BinaryData_internal::getData(const void *buff, admf::ADMF_UINT len)
{
    if (len == 0 || buff == nullptr)
        return 0;
    switch (originInfo_.originType)
    {
    case OriginType::ZipEntry:
        {
            auto zipEntry = getZipEntry();
            if (!zipEntry)
                return 0;
            size_t len_ = len;
            //zip_helper::ZIPResult result =
            zipEntry->ExtractData((ADMF_BYTE *)buff, len_);
            return (ADMF_UINT)len_;
        }
        break;
        
    case OriginType::File:
        {
            FILE *file = fopen(originInfo_.content.c_str(), "rb");
            if (file == nullptr)
                return 0;
            
            auto readNum = fread((void*)buff, len, 1, file);
            fclose(file);
            
            return (ADMF_UINT)readNum;
        }
        break;
        
    case OriginType::Data:
        {
            int len_ = std::min(len, getDataLength());
            if (len_ <= 0)
                return len_;
        
            memcpy((void*)buff, originInfo_.content.c_str(), len_);
            return len_;
        }
        break;
        
    default:
        return 0;
    }

    
    
}

String BinaryData_internal::getAssignedName()
{
    return String(assignedName_);
}

String BinaryData_internal::getName()
{
    if (!assignedName_->isEmpty())
        return getAssignedName();

    return getRawName();
}

String BinaryData_internal::getRawName()
{
    return String(name_);
}

ZipArchiveEntry::Ptr BinaryData_internal::getZipEntry()
{
    return ADMF_internal::getZipEntry(admfIndex_, baseIndex_);
}
admf::ADMF_RESULT BinaryData_internal::exportToFile(const char *filePath)
{

    switch (originInfo_.originType)
    {
    case OriginType::ZipEntry:
    {
        auto zipEntry = getZipEntry();
        if (!zipEntry)
            return ADMF_BINARY_NOT_FOUND;

        zip_helper::ZIPResult result = zipEntry->ExtractFile(filePath);
        if (result != zip_helper::ZIPResult::SUCCESS)
            return ADMF_WRITE_TO_FILE_FAILED;

        return ADMF_SUCCESS;
    }
    break;

    case OriginType::File:
    {
#if defined(__APPLE__) || defined(_WIN32)
#ifdef __APPLE__
        namespace fs = std::__fs::filesystem;
#else
        namespace fs = std::filesystem;
#endif
        const auto copyOptions = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
        std::error_code ec;
        fs::copy(originInfo_.content, filePath, copyOptions, ec);
        if (ec.value() != 0)
            return ADMF_WRITE_TO_FILE_FAILED;
        return ADMF_SUCCESS;
#else
        //linux
        if (admf_internal::copyFile(originInfo_.content.c_str(), filePath))
            return ADMF_WRITE_TO_FILE_FAILED;
        return ADMF_SUCCESS;
#endif
    }
    break;

    case OriginType::Data:
    {
        std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
        if (!file.is_open())
            return ADMF_OPEN_FILE_FAILED;
        file << originInfo_.content;
        return ADMF_SUCCESS;
    }
    break;

    default:
        return ADMF_UNKNOWN_ERROR;
    }
}

#ifdef ADMF_EDIT

admf::ADMF_RESULT BinaryData_internal::updateFromData(const void *buff, admf::ADMF_UINT len)
{
    if (buff == nullptr || len == 0)
        return ADMF_PARAM_ERROR;
    if (len > g_binarySizeLimit)
        return ADMF_BINARY_SIZE_EXCEED_LIMIT;

    originInfo_.clear();
    originInfo_.originType = OriginType::Data;
    originInfo_.content.append(reinterpret_cast<char const *>(buff), len);
    return ADMF_SUCCESS;
}

admf::ADMF_RESULT BinaryData_internal::updateFromFile(const char *filePath, bool needDelete)
{
    if (filePath == nullptr)
        return ADMF_PARAM_ERROR;

    originInfo_.clear();
    originInfo_.originType = OriginType::File;
    originInfo_.content = filePath;
    originInfo_.needDeleteFile = needDelete;
    return ADMF_SUCCESS;
}


void BinaryData_internal::setName(const char *name)
{
    name_->setString(name);
}
#endif
void Texture_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string pathKey = getNewKey("path");
    std::string colorSpaceKey = getNewKey("colorSpace");
    std::string dpiKey = getNewKey("dpi");
    std::string widthKey = getNewKey("width");
    std::string heightKey = getNewKey("height");
	std::string physicalWidthKey = getNewKey("physicalWidth");
	std::string physicalHeightKey = getNewKey("physicalHeight");
    std::string channelsKey = getNewKey("channels");
    std::string elementSizeKey = getNewKey("elementSize");
    //std::string typeKey = getNewKey("type");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == pathKey)
        {
            binaryData_ = std::make_shared<BinaryData_internal>(admfIndex_, &child);
        }
        else if (keyName == colorSpaceKey)
        {
            colorSpace_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == dpiKey)
        {
            dpi_ = std::make_shared<Vec2_internal>(admfIndex_, &child);
        }
        else if (keyName == widthKey)
        {
            width_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == heightKey)
        {
            height_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
		else if (keyName == physicalWidthKey)
		{
			physicalWidth_ = (ADMF_FLOAT)bson_iter_as_double(&child);
		}
		else if (keyName == physicalHeightKey)
		{
			physicalHeight_ = (ADMF_FLOAT)bson_iter_as_double(&child);
		}
        else if (keyName == channelsKey)
        {
            channels_ = (ADMF_UINT)bson_iter_as_int64(&child);
        }
        else if (keyName == elementSizeKey)
        {
            elementSize_ = (ADMF_UINT)bson_iter_as_int64(&child);
        }
        /* assigned by parent object
        else if (keyName == typeKey)
        {
            setType((TEX_TYPE)bson_iter_as_int64(&child));
        }
        */
    }
}

void Texture_internal::initMissed()
{
    if (!binaryData_)
        binaryData_ = std::make_shared<BinaryData_internal>(admfIndex_);
    if (!colorSpace_)
        colorSpace_ = std::make_shared<String_internal>(admfIndex_);
    if (!dpi_)
        dpi_ = std::make_shared<Vec2_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void Texture_internal::save(bson_t *doc)
{
    std::string pathKey = getNewKey("path");
    std::string colorSpaceKey = getNewKey("colorSpace");
    std::string dpiKey = getNewKey("dpi");
    std::string widthKey = getNewKey("width");
    std::string heightKey = getNewKey("height");
	std::string physicalWidthKey = getNewKey("physicalWidth");
	std::string physicalHeightKey = getNewKey("physicalHeight");
    std::string channelsKey = getNewKey("channels");
    std::string elementSizeKey = getNewKey("elementSize");

    //std::string typeKey = getNewKey("type");

    ADMF_BSON_APPEND_BINARY(doc, pathKey, binaryData_);
    ADMF_BSON_APPEND_STRING(doc, colorSpaceKey, colorSpace_);
    ADMF_BSON_APPEND_DOCUMENT(doc, dpiKey, dpi_);
    ADMF_BSON_APPEND_DOUBLE(doc, widthKey, width_);
    ADMF_BSON_APPEND_DOUBLE(doc, heightKey, height_);
	ADMF_BSON_APPEND_DOUBLE(doc, physicalWidthKey, physicalWidth_);
	ADMF_BSON_APPEND_DOUBLE(doc, physicalHeightKey, physicalHeight_);
    ADMF_BSON_APPEND_INT32(doc, channelsKey, channels_);
    ADMF_BSON_APPEND_INT32(doc, elementSizeKey, elementSize_);
}
#endif
BinaryData Texture_internal::getBinaryData() //content of "/normal.png""
{
    return BinaryData(binaryData_);
}
TEX_TYPE Texture_internal::getType()
{
    return type_;
}

String Texture_internal::getTypeName()
{
    return String(typeName_);
}
void Texture_internal::setType(admf::TEX_TYPE type) {
	type_ = type;
	typeName_ = getTypeName_internal();
	if (binaryData_)
	{
        auto textureBinaryType = this->getTypeByBinaryData();

        auto ext = this->getExtensionByTextureFileType(textureBinaryType);
		std::string str = typeName_->getInternalString() + "." + ext;
        auto string = binaryData_->getAssignedName();
        auto* string_internal = dynamic_cast<String_internal*>(string.get());
        assert(string_internal);
        string_internal->str_ = str;
	}
};
/*
 TEX_TYPE_BASE = 0,
 TEX_TYPE_NORMAL,
 TEX_TYPE_ALPHA,
 TEX_TYPE_METALNESS,
 TEX_TYPE_ROUGHNESS,
 TEX_TYPE_SPECULAR,
 TEX_TYPE_GLOSSINESS
 */
std::shared_ptr<String_internal> Texture_internal::getTypeName_internal()
{
    String_internal *str;
    switch (type_)
    {
    case TEX_TYPE_BASE:
        str = new String_internal("Base");
        break;
    case TEX_TYPE_NORMAL:
        str = new String_internal("Normal");
        break;
    case TEX_TYPE_ALPHA:
        str = new String_internal("Alpha");
        break;
    case TEX_TYPE_METALNESS:
        str = new String_internal("Metalness");
        break;
    case TEX_TYPE_ROUGHNESS:
        str = new String_internal("Roughness");
        break;
    case TEX_TYPE_SPECULAR:
        str = new String_internal("Specular");
        break;
    case TEX_TYPE_GLOSSINESS:
        str = new String_internal("Glossiness");
        break;
    case TEX_TYPE_ANISOTROPY:
        str = new String_internal("Anisotropy");
        break;
    case TEX_TYPE_ANISOTROPY_ROTATION:
        str = new String_internal("AnisotropyRotation");
        break;
    case TEX_TYPE_EMISSIVE:
        str = new String_internal("Emissive");
        break;
    case TEX_TYPE_AO:
        str = new String_internal("AO");
        break;
    case TEX_TYPE_HEIGHT:
        str = new String_internal("Height");
        break;
    case TEX_TYPE_CLEARCOAT_NORMAL:
        str = new String_internal("ClearCoatNormal");
        break;
    case TEX_TYPE_CLEARCOAT_ROUGHNESS:
        str = new String_internal("ClearCoatRoughness");
        break;
    case TEX_TYPE_CLEARCOAT_VALUE:
        str = new String_internal("ClearCoatValue");
        break;
    case TEX_TYPE_SHEEN_TINT:
        str = new String_internal("SheenTint");
        break;
    case TEX_TYPE_SHEEN_VALUE:
        str = new String_internal("SheenValue");
        break;
    case TEX_TYPE_SPECULAR_TINT:
        str = new String_internal("SpecularTint");
        break;
    case TEX_TYPE_SUBSURFACE_COLOR:
        str = new String_internal("SubSurfaceColor");
        break;
    case TEX_TYPE_SUBSURFACE_RADIUS:
        str = new String_internal("SubSurfaceRadius");
        break;
    case TEX_TYPE_SUBSURFACE_VALUE:
        str = new String_internal("SubSurfaceValue");
        break;
    case TEX_TYPE_TRANSMISSION:
        str = new String_internal("Tranmission");
        break;
    case TEX_TYPE_IOR:
        str = new String_internal("IOR");
        break;
    default:
        str = new String_internal("Unknown");
        break;
    }
    return std::shared_ptr<String_internal>(str);
}

String Texture_internal::getColorSpace()
{
    return colorSpace_;
}

ADMF_FLOAT Texture_internal::getWidth()
{
    return width_;
}

Vec2 Texture_internal::getDpi()
{
    return Vec2(dpi_);
}

ADMF_FLOAT Texture_internal::getHeight()
{
    return height_;
}

ADMF_FLOAT Texture_internal::getPhysicalWidth()
{
    return physicalWidth_;
}
ADMF_FLOAT Texture_internal::getPhysicalHeight()
{
    return physicalHeight_;
}



const std::vector<std::pair<admf::TextureFileType, std::vector<unsigned char>>> G_BinaryTypeData
{
    {admf::TextureFileType::JPG, {0xFF,0xD8,0xFF,0xE0}},
    {admf::TextureFileType::JPG, {0xFF,0xD8,0xFF,0xE1}},
    {admf::TextureFileType::JPG, {0xFF,0xD8,0xFF,0xE8}},
    {admf::TextureFileType::PNG, {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}},
    {admf::TextureFileType::GIF, {0x47,0x49,0x46,0x38}},
    {admf::TextureFileType::TIFF, {0x49,0x20,0x49}},
    {admf::TextureFileType::TIFF, {0x49,0x49,0x2A,0x00}},
    {admf::TextureFileType::TIFF, {0x4D,0x4D,0x00,0x2A}},
    {admf::TextureFileType::TIFF, {0x4D,0x4D,0x00,0x2B}},

};
admf::TextureFileType Texture_internal::getTypeByBinaryData(const unsigned char* data, admf::ADMF_UINT len)
{

    if (data == nullptr)
        return admf::TextureFileType::None;
    admf::TextureFileType result = admf::TextureFileType::RAW;
    
    for (auto it = G_BinaryTypeData.begin(); it != G_BinaryTypeData.end(); it ++)
    {
        auto v = it->second;
        if (len < v.size())
            continue;
        
        bool needContinue = false;
        for (int i = 0; i < v.size(); i++)
        {
            if (data[i] != v[i])
            {
                needContinue = true;
                break;
            }
        }
        
        if (needContinue)
            continue;
        
        result = it->first;
        break;
    }
    
    
    
    return result;
}

admf::TextureFileType Texture_internal::getTypeByBinaryData()
{
    if (!binaryData_)
        return admf::TextureFileType::RAW;
    
    
    auto dataLen = binaryData_->getDataLength();
    unsigned char* dataBuff = (unsigned char*)malloc(dataLen);
    memset(dataBuff, 0, dataLen);
    
    binaryData_->getData(dataBuff, dataLen);
    
    admf::TextureFileType result = Texture_internal::getTypeByBinaryData(dataBuff, (admf::ADMF_UINT)dataLen);
    
    free(dataBuff);
    dataBuff = NULL;
    
    return result;
    
    
}


std::string Texture_internal::getExtensionByTextureFileType(admf::TextureFileType type)
{
    switch (type) {
    case TextureFileType::JPG:
        return "jpg";
    case TextureFileType::TIFF:
        return "tiff";
    case TextureFileType::GIF:
        return "gif";
    case TextureFileType::PNG:
    default:
        return "png";
    }
}

//
//  admf_main_internal.cpp
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#include "admf_internal_header.h"
#include <chrono>
#include "bson.h"
#include <memory>
#include "render_helper.h" //encrypt
#include <random>
#include "base64.h"
#include <iostream>
#include <fstream>
#ifndef __APPLE__
#include <assert.h>
#endif
using namespace admf;
using namespace admf_internal;

const unsigned int BsonHeadLen = 16;

ADMFInternalIndexMap ADMF_internal::admfIndexMap;

constexpr const char* bsonZipName = "description";

static std::string __pwd__;

ADMF_UINT admf_internal::g_descriptionSizeLimit = 20 * 1024 * 1024;
ADMF_UINT admf_internal::g_binarySizeLimit = 100 * 1024 * 1024;

unsigned int admf_internal::filesize(const std::string &filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    int len = (int)in.tellg();
    if (len < 0)
        len = 0;
    return len;
}
std::string admf_internal::getNewKey(const std::string &origKey)
{

#ifdef ADMF_NEED_ENCRYPT
    size_t len;
    void *encryptKey = admfRender::makeRenderPipeline(origKey.c_str(), origKey.length(), __pwd__.c_str(), &len);

    std::string base64 = base64_encode((unsigned char const *)encryptKey, len);
    if (encryptKey)
        free(encryptKey);

    return base64;
#else
    return origKey;
#endif
}

char* admf_internal::getNewKey_(const char* origKey)
{
#ifdef ADMF_NEED_ENCRYPT
    std::string decode_base64 = base64_decode(origKey, strlen(origKey));
    //void * rasterizer(const void * data, size_t len, const void * key, size_t * out_len);
    size_t len;
    void* data = admfRender::rasterizer(decode_base64.data(), decode_base64.length(), __pwd__.c_str(), &len);

    
    return (char*) data;
#else
    return const_cast<char*>(origKey);
#endif
}

#ifndef WIN32
#define _open open
#define _read read
#define _write write
#define _close close
#endif

extern bool admf_internal::copyFile(const char* src, const char* dest)
{
    if (src == nullptr || dest == nullptr)
        return false;
	int in_fd = _open(src, O_RDONLY);
	assert(in_fd >= 0);
    if (in_fd < 0)
        return false;

	int out_fd = _open(dest, O_WRONLY);
	assert(out_fd >= 0);
	if (out_fd < 0)
		return false;

	char buf[8192];

	while (true) {
		ssize_t read_result = _read(in_fd, &buf[0], sizeof(buf));
		if (!read_result) break;
		ssize_t write_result = _write(out_fd, &buf[0], read_result);
		assert(write_result == read_result);
	}

    _close(in_fd);
    _close(out_fd);
    return true;
}



std::string admf_internal::getBsonString(const bson_iter_t *iter)
{
    uint32_t len;
    const char *str = bson_iter_utf8(iter, &len);
    if (str != nullptr)
        return std::string(str, len);

    return std::string();
}

void ADMF_::setSizeLimit(ADMF_UINT descriptionSizeLimit, ADMF_UINT binarySizeLimit)
{
    g_descriptionSizeLimit = descriptionSizeLimit;
    g_binarySizeLimit = binarySizeLimit;
}

void ADMF_::getSizeLimit(ADMF_UINT &descriptionSizeLimit, ADMF_UINT &binarySizeLimit)
{
    descriptionSizeLimit = g_descriptionSizeLimit;
    binarySizeLimit = g_binarySizeLimit;
}

ADMF_internal *ADMF_internal::getADMFInternalByIndex(int admfIndex)
{
    return admfIndexMap[admfIndex];
}

static unsigned int s_admfIndex = 1;

ADMF_internal::ADMF_internal(ZipArchive::Ptr admfZipEntry, ZipArchiveEntry::Ptr bsonZipEntry)
    : admfZipEntry_(admfZipEntry), bsonZipEntry_(bsonZipEntry), Base_internal(s_admfIndex++)
{
    _();
    admfRender::init();
    admfIndexMap[admfIndex_] = this;
}

ADMF_internal::~ADMF_internal()
{
    zipEntryMap_.clear();
    admfIndexMap.erase(admfIndex_);
    bsonZipEntry_.reset();
    admfZipEntry_.reset();
    

    
}

admf::StringReadOnly ADMF_internal::getSchema()
{
    return StringReadOnly(schema_);
}
admf::StringReadOnly ADMF_internal::getSDKVersion()
{
    return StringReadOnly(sdkVersion_);
}
admf::Material ADMF_internal::getMaterial()
{
    return Material(material_internal_);
}
admf::Physics ADMF_internal::getPhysics()
{
    return Physics(physics_internal_);
}
admf::Custom ADMF_internal::getCustom()
{
    return Custom(custom_internal_);
}
admf::Geometry ADMF_internal::getGeometry()
{
    return Geometry(geometry_internal_);
}

void ADMF_internal::_()
{
    static bool init = false;
    if (init)
        return;

    init = true;

#ifdef ADMF_NEED_ENCRYPT
    __pwd__.append("$");
    __pwd__.append("x");
    __pwd__.append("8");
    __pwd__.append("&");
    __pwd__.append(")");
    __pwd__.append("@");
    __pwd__.append("_");
    __pwd__.append("A");
    __pwd__.append(" ");
    __pwd__.append("~");
#endif
}

#ifdef ADMF_EDIT
ADMF admf::createADMF()
{
    ADMF_internal *admf_internal = new ADMF_internal();
    admf_internal->initMissed();
    return ADMF(admf_internal);
}
#endif

ADMF admf::loadFromFile(const char *filePath, ADMF_RESULT &result)
{
    return ADMF_internal::loadFromFile(filePath, result, nullptr);
}

ADMF ADMF_internal::loadFromFile(const char* filePath, ADMF_RESULT& result, ADMFJsons* jsons)
{
    ZipArchive::Ptr zipArchive = ZipFile::Open(filePath);
    if (!zipArchive)
    {
        result = ADMF_FILE_NOT_EXIST;
        return nullptr;
    }
    std::string bsonZipNameKey = getNewKey(bsonZipName);
    ZipArchiveEntry::Ptr zipArchiveEntry = zipArchive->GetEntry(bsonZipNameKey);
    if (!zipArchiveEntry)
    {
        result = ADMF_DESC_NOT_FOUND;
        return nullptr;
    }
    
    zipArchiveEntry->SetPassword(__pwd__);
    
    size_t uncompressSize = zipArchiveEntry->GetSize();
    if (uncompressSize > g_descriptionSizeLimit || uncompressSize <= BsonHeadLen)
    {
        result = ADMF_DESC_SIZE_ERROR;
        return nullptr;
    }
    
    unsigned char *bsonData = new unsigned char[uncompressSize];
    
    
    std::unique_ptr<char, std::function<void(char*)>> _(new char,[&bsonData](char*p)->void{
        delete[] bsonData;
        bsonData = nullptr;
        delete p;
    });
    
    zip_helper::ZIPResult extractResult = zipArchiveEntry->ExtractData(bsonData, uncompressSize);
    if (extractResult != zip_helper::ZIPResult::SUCCESS)
    {
        result = ADMF_GET_DESC_CONTENT_FAILED;
        return nullptr;
    }
    
    admfRender::____(bsonData, uncompressSize);
    
    ADMF_UINT rawSize = (ADMF_UINT)(uncompressSize - BsonHeadLen);
    if (!ADMF_internal::verifyBsonHead(bsonData, BsonHeadLen, rawSize))
    {
        result = ADMF_GET_DESC_CONTENT_FAILED;
        return nullptr;
    }
    
    ADMF_BYTE *bsonRawData = bsonData + BsonHeadLen;
    
    bson_reader_t *reader = bson_reader_new_from_data(bsonRawData, rawSize);
    if (reader == nullptr)
    {
        result = ADMF_GET_DESC_CONTENT_FAILED;
        return nullptr;
    }
    
    ADMF_internal *admf_internal = new ADMF_internal(zipArchive, zipArchiveEntry);
    auto admf_load_result = admf_internal->load(reader, jsons);
    
    bson_reader_destroy(reader);
    if (admf_load_result != ADMF_SUCCESS)
    {
        result = admf_load_result;
        return nullptr;
    }
    
    admf_internal->initMissed();
    
    result = ADMF_SUCCESS;
    return ADMF(admf_internal);
}

admf::ADMF_RESULT ADMF_internal::load(bson_reader_t *reader, ADMFJsons* jsons) //save
{

#define GetJsons(x) \
    if (jsons) \
    { \
        bson_t b; \
        uint32_t len; \
        const uint8_t *data; \
        bson_iter_document(&iter, &len, &data); \
        if (bson_init_static (&b, data, len)) { \
            g_convertKey = getNewKey_; \
            char *json = bson_as_relaxed_extended_json (&b, NULL); \
            g_convertKey = NULL;\
            if (json) \
            { \
                jsons->x = json; \
                bson_free(json); \
            }\
        } \
    }\
    
    if (reader == nullptr)
        return ADMF_PARAM_ERROR;

    if (reader)
    {
        bool reached_eof = false;

        const bson_t *bson_read_ = bson_reader_read(reader, &reached_eof);

      

        bson_iter_t iter;
        if (!bson_iter_init(&iter, bson_read_))
        {
            return ADMF_DESC_FORMAT_ERROR;
        }

        std::string schemaKey = getNewKey("schema");
        std::string materialKey = getNewKey("material");
        std::string physicsKey = getNewKey("physics");
        std::string customKey = getNewKey("custom");
        std::string geometryKey = getNewKey("geometry");
        std::string sdkVersionKey = getNewKey("sdkVersion");
        while (bson_iter_next(&iter))
        {
            std::string keyName = bson_iter_key(&iter);
            //printf("Found element key: \"%s\"\n", keyName.c_str());
            assert(bson_iter_value(&iter) != nullptr);
            if (keyName == schemaKey)
            {
                schema_ = std::make_shared<StringReadOnly_internal>(admfIndex_, &iter);
            }
            else if (keyName == materialKey)
            {
                GetJsons(material)
                material_internal_ = std::make_shared<Material_internal>(admfIndex_, &iter);
            }
            else if (keyName == physicsKey)
            {
                GetJsons(physics)
                physics_internal_ = std::make_shared<Physics_internal>(admfIndex_, &iter);
            }
            else if (keyName == customKey)
            {
                GetJsons(custom)
                custom_internal_ = std::make_shared<Custom_internal>(admfIndex_, &iter);
            }
            else if (keyName == geometryKey)
            {
                GetJsons(geometry)
                geometry_internal_ = std::make_shared<Geometry_internal>(admfIndex_, &iter);
            }
            else if (keyName == sdkVersionKey)
            {
                sdkVersion_ = std::make_shared<StringReadOnly_internal>(admfIndex_, &iter);
            }
        }

#ifdef DEBUG
        //auto *b1 = bson_reader_read(reader, &reached_eof);
        //assert(b1 == nullptr && reached_eof);
#endif
    }

    return ADMF_SUCCESS;
}

void ADMF_internal::initMissed()
{
    if (!schema_)
        schema_ = std::make_shared<StringReadOnly_internal>(admfIndex_);
    if (!sdkVersion_)
        sdkVersion_ = std::make_shared<StringReadOnly_internal>(admfIndex_);
    if (!material_internal_)
        material_internal_ = std::make_shared<Material_internal>(admfIndex_);
    if (!physics_internal_)
        physics_internal_ = std::make_shared<Physics_internal>(admfIndex_);
    if (!geometry_internal_)
        geometry_internal_ = std::make_shared<Geometry_internal>(admfIndex_);
    if (!custom_internal_)
        custom_internal_ = std::make_shared<Custom_internal>(admfIndex_);
}

#ifdef ADMF_EDIT

void ADMF_internal::save(bson_t *doc)
{

    std::string schemaKey = getNewKey("schema");
    std::string materialKey = getNewKey("material");
    std::string physicsKey = getNewKey("physics");
    std::string customKey = getNewKey("custom");
    std::string geometryKey = getNewKey("geometry");
    std::string sdkVersionKey = getNewKey("sdkVersion");

    schema_->str_ = ADMF_SCHEMA;
    sdkVersion_->str_ = ADMF_SDK_VERSION;
    

    
    ADMF_BSON_APPEND_STRING(doc, sdkVersionKey, sdkVersion_);
    ADMF_BSON_APPEND_STRING(doc, schemaKey, schema_);
    ADMF_BSON_APPEND_DOCUMENT(doc, materialKey, material_internal_);
    ADMF_BSON_APPEND_DOCUMENT(doc, physicsKey, physics_internal_);
    ADMF_BSON_APPEND_DOCUMENT(doc, geometryKey, geometry_internal_);
    ADMF_BSON_APPEND_DOCUMENT(doc, customKey, custom_internal_);
}

ADMF_RESULT ADMF_internal::saveToFile(const char *filePath)
{
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    fileInfos_.clear();
    needDeleteFiles_.clear();
    std::string tmpPath(filePath);
    tmpPath += ".tmp";
    remove(tmpPath.c_str());

    bson_writer_t *writer;
    bson_t *doc;
    uint8_t *bsonBuff = NULL;
    size_t bsonBuffLen = 0;
    bool r;

    writer = bson_writer_new(&bsonBuff, &bsonBuffLen, 0, bson_realloc_ctx, NULL);

    r = bson_writer_begin(writer, &doc);
    assert(r);

    //r = BSON_APPEND_INT32 (doc, "i", i);
    //assert (r);
    save(doc);

    bson_writer_end(writer);

    unsigned char bsonHead[BsonHeadLen];
    makeBsonHead(bsonHead, BsonHeadLen, (ADMF_UINT)bsonBuffLen);
    
    std::string bsonData(reinterpret_cast<char const *>(bsonHead), BsonHeadLen);
    bsonData.append(reinterpret_cast<char const *>(bsonBuff), bsonBuffLen);
    
    auto size = bsonData.length();
    unsigned char* buffer = new unsigned char[size];
    memcpy(buffer, bsonData.data(), size);
    
    admfRender::___(buffer, size);
    
    bsonData.clear();
    
    bsonData.append(reinterpret_cast<char const *>(buffer), size);
    delete [] buffer;
    /*
     bool isFile = false;
     std::string content; //file or string
     std::string inArchiveName;
     std::string password;
     ICompressionMethod::Ptr method = DeflateMethod::Create();
     */
    std::string bsonZipNameKey = getNewKey(bsonZipName);

    fileInfos_.emplace_back(false, bsonData, bsonZipNameKey, __pwd__);
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = t2-t1;
    printf("before AddEncryptedFiles:%f\n", time_span.count());

    zip_helper::ZIPResult result = ZipFile::AddEncryptedFiles(tmpPath, fileInfos_);
    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
    time_span = t3-t2;
    printf("after AddEncryptedFiles:%f\n", time_span.count());
    for (auto &filePath : needDeleteFiles_)
    {
        remove(filePath.c_str());
    }

    bson_free(bsonBuff);
    fileInfos_.clear();
    needDeleteFiles_.clear();

    if (result != zip_helper::ZIPResult::SUCCESS)
        return ADMF_WRITE_TO_FILE_FAILED;

    remove(filePath);
    rename(tmpPath.c_str(), filePath);
    
    std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
    time_span = t4-t3;
    printf("after delete all temp files:%f\n", time_span.count());

    return ADMF_SUCCESS;
}

void ADMF_internal::addBinaryFileEntry(const std::string &filePath, const std::string &inZipName, bool needDelete)
{
    fileInfos_.emplace_back(true, filePath, inZipName, __pwd__);
    if (needDelete)
        needDeleteFiles_.emplace_back(filePath);
}
void ADMF_internal::addDataEntry(const std::string &data, const std::string &inZipName)
{
    fileInfos_.emplace_back(false, data, inZipName, __pwd__);
}

#endif
bool ADMF_internal::makeBsonHead(admf::ADMF_BYTE *head, admf::ADMF_UINT headLen, admf::ADMF_UINT rawSize)
{
    if (head == nullptr || headLen != BsonHeadLen)
    {
        return false;
    }

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> distByte(0, 255);

    ADMF_UINT v1 = 0, v2 = 0;
    ADMF_BYTE value = 0;

    for (int i = 0; i < BsonHeadLen - 2; i++)
    {
        if (i < 4)
        {
            value = (rawSize >> i*8) & 0xFF;
        }
        else
        {
            value = distByte(rng);
        }

        v1 += value;

        if (i % 2 == 0)
            v2 += value;
        else
            v2 -= value;

        head[i] = value;
    }

    head[BsonHeadLen-2] = v1 & 0xFF;
    head[BsonHeadLen-1] = v2 & 0xFF;
    return true;
}

bool ADMF_internal::verifyBsonHead(admf::ADMF_BYTE *head, admf::ADMF_UINT headLen, admf::ADMF_UINT rawSize)
{
    if (head == nullptr || headLen != BsonHeadLen)
    {
        return false;
    }

    ADMF_UINT v1 = 0, v2 = 0;
    ADMF_BYTE value = 0;

    for (int i = 0; i < BsonHeadLen - 2; i++)
    {
        value = head[i];
        if (i < 4)
        {
            if (value != ((rawSize >> i*8) & 0xFF))
            {
                return false;
            }
        }

        v1 += value;

        if (i % 2 == 0)
            v2 += value;
        else
            v2 -= value;
    }

    if (head[BsonHeadLen-2] != (v1 & 0xFF))
        return false;
    if (head[BsonHeadLen-1] != (v2 & 0xFF))
        return false;

    return true;
}

ZipArchiveEntry::Ptr ADMF_internal::getZipArchiveEntry(const std::string &name)
{
    if (!admfZipEntry_)
        return nullptr;

    auto entry = admfZipEntry_->GetEntry(name);
    if (!entry)
        return nullptr;
    entry->SetPassword(__pwd__);
    return entry;
}



//bson helper code
//bson字段格式变化后进行兼容处理
#define BSON_ITER_TYPE(i) ((int) * ((i)->raw + (i)->type))
bool admf_internal::bson_iter_can_convert_to_double (const bson_iter_t *iter)
{
    if (iter == nullptr)
        return false;
    
    switch ((int) BSON_ITER_TYPE (iter)) {
    case BSON_TYPE_BOOL:
    case BSON_TYPE_DOUBLE:
    case BSON_TYPE_INT32:
    case BSON_TYPE_INT64:
        return true;
        
    default:
        return false;
    }
}

//
//  admf_main_internal.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//


#include "admf_internal.h"
#include "ZipFile.h"
#include <unordered_map>
#ifndef admf_main_internal_h
#define admf_main_internal_h

namespace admf_internal {
    
    class ADMF_internal;
    class Material_internal;
    class Physics_internal;
    class Custom_internal;
    class Geometry_internal;
    class String_internal;
    class StringReadOnly_internal;
    
    typedef std::unordered_map<int, ADMF_internal*> ADMFInternalIndexMap; //int means admfIndex_
    typedef std::unordered_map<int, ZipArchiveEntry::Ptr> ZipEntryMap; //int means baseIndex_
    
    
    class ADMF_internal : public admf::ADMF_, public Base_internal
    {
    public:
        struct ADMFJsons
        {
            std::string custom;
            std::string material;
            std::string geometry;
            std::string physics;
        };
        
        //manager zip and admf.bson
        //zip contains admf.bson and other binary resources

    private:
        static ADMFInternalIndexMap admfIndexMap;
        static void _();
        virtual void load(bson_iter_t *iter) override {};

        
        
    public:
        static admf::ADMF loadFromFile(const char* filePath, admf::ADMF_RESULT& result, ADMFJsons* jsons);
        
        static ADMF_internal* getADMFInternalByIndex(int admfIndex);
        static void addZipEntry(int admfIndex, int baseIndex, ZipArchiveEntry::Ptr ptr)
        {
            if (!ptr)
                return;
            
            ADMF_internal* admf_internal = getADMFInternalByIndex(admfIndex);
            if (!admf_internal)
                return;
            
            admf_internal->zipEntryMap_[baseIndex] = ptr;
        }
        
        static ZipArchiveEntry::Ptr getZipEntry(int admfIndex, int baseIndex)
        {
            ADMF_internal* admf_internal = getADMFInternalByIndex(admfIndex);
            if (!admf_internal)
                return nullptr;
            auto it = admf_internal->zipEntryMap_.find(baseIndex);
            if (it == admf_internal->zipEntryMap_.end())
                return nullptr;
            return it->second;
        }
        
        
       
        virtual admf::Material getMaterial() override;
        virtual admf::Physics getPhysics() override;
        virtual admf::Custom getCustom() override;
        virtual admf::Geometry getGeometry() override;
        virtual admf::StringReadOnly getSDKVersion() override; //上次编辑时的SDK版本
        virtual admf::StringReadOnly getSchema() override; //"1.0" 文件格式版本 文件格式不变， 只新增贴图之类的字段时，schema不变 sdkversion更新
        
    private:
        std::shared_ptr<StringReadOnly_internal> schema_;
        std::shared_ptr<Material_internal> material_internal_;
        std::shared_ptr<Physics_internal> physics_internal_;
        std::shared_ptr<Custom_internal> custom_internal_;
        std::shared_ptr<Geometry_internal> geometry_internal_;
        std::shared_ptr<StringReadOnly_internal> sdkVersion_; //文件里的sdk版本
        
    private:
        ZipEntryMap zipEntryMap_; //record all the entries used by binary data()
        
        ZipArchive::Ptr admfZipEntry_;
        ZipArchiveEntry::Ptr bsonZipEntry_;
        
    public:
        admf::ADMF_RESULT load(bson_reader_t* reader, ADMFJsons* jsons);
        virtual void initMissed() override;
        ADMF_internal():ADMF_internal(nullptr, nullptr) {};
        ADMF_internal(ZipArchive::Ptr admfZipEntry,ZipArchiveEntry::Ptr bsonZipEntry);
        ~ADMF_internal();
    
    public:

        static bool verifyBsonHead(admf::ADMF_BYTE* head, admf::ADMF_UINT headLen, admf::ADMF_UINT rawSize);
        static bool makeBsonHead(admf::ADMF_BYTE* head, admf::ADMF_UINT headLen, admf::ADMF_UINT rawSize);
        ZipArchiveEntry::Ptr getZipArchiveEntry(const std::string& name);
        
#ifdef ADMF_EDIT
    public:
        virtual admf::ADMF_RESULT saveToFile(const char* filePath) override;
        void addBinaryFileEntry(const std::string& filePath, const std::string& inZipName, bool needDelete = false);
        void addDataEntry(const std::string& data, const std::string& inZipName);
    private:
        virtual void save(bson_t* doc) override;
        std::vector<ZipFile::FileInfo> fileInfos_; //exclude bson;
        std::vector<std::string> needDeleteFiles_;
#endif
    };
}
#endif /* admf_structures_h */

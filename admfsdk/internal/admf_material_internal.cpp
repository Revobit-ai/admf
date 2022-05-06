//
//  admf_material_internal.cpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//


#include "admf_internal_header.h"
#include "bson.h"
using namespace admf_internal;
using namespace admf;
#ifndef __APPLE__
#include <assert.h>
#endif

void Material_internal::load(bson_iter_t *iter) //save
{
    //layerArray_ = std::make_shared<Array_internal<admf::MaterialLayer>>();
    layerArray_ = std::make_shared<Array_internal<MaterialLayer>>([this]() {
        return std::make_shared<MaterialLayer_internal>(admfIndex_, nullptr);
    });
    
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    
    bson_iter_t child;
    if (!bson_iter_recurse (iter, &child))
        return;

    std::string layersKey = getNewKey("layers");
    std::string sideKey = getNewKey("side");
    std::string deviceKey = getNewKey("device");
    std::string createdKey = getNewKey("created");
    std::string modifiedKey = getNewKey("modified");
    std::string idKey = getNewKey("id");
    std::string nameKey = getNewKey("name");
    std::string metadataKey = getNewKey("metadata");
    
    
    
    while (bson_iter_next (&child)) {
        std::string keyName = bson_iter_key (&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == layersKey)
        {
            //layers
            if (!BSON_ITER_HOLDS_ARRAY(&child))
                continue;
            bson_t b;
            uint32_t len;
            const uint8_t *data;
            bson_iter_array(&child, &len, &data);
            if (!bson_init_static (&b, data, len))
                continue;
            
            bson_iter_t iter_;
            if (!bson_iter_init (&iter_, &b))
                continue;
            
            
            while (bson_iter_next (&iter_)) {
                auto layer = std::make_shared<MaterialLayer_internal>(admfIndex_, &iter_);
                layerArray_->pushBack(layer);
            }
        }
        else if (keyName == sideKey)
        {
            sideLayer_ = std::make_shared<MaterialLayer_internal>(admfIndex_, &child);
        }
        else if (keyName == deviceKey)
        {
            device_ = std::make_shared<MaterialDevice_internal>(admfIndex_, &child);
        }
        else if (keyName == createdKey)
        {
            createTime_ = bson_iter_date_time(&child);
        }
        else if (keyName == modifiedKey)
        {
            modifiedTime_ = bson_iter_date_time(&child);
        }
        else if (keyName == idKey)
        {
            id_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == nameKey)
        {
            name_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == metadataKey)
        {
           //MaterialMetaData
            metaData_ = std::make_shared<MaterialMetaData_internal>(admfIndex_, &child);
        }

    }
}

void Material_internal::initMissed()
{
    if (!sideLayer_)
        sideLayer_ = std::make_shared<MaterialLayer_internal>(admfIndex_);
    if (!device_)
        device_ = std::make_shared<MaterialDevice_internal>(admfIndex_);
    if (!id_)
        id_ = std::make_shared<String_internal>(admfIndex_);
    if (!name_)
        name_ = std::make_shared<String_internal>(admfIndex_);
    if (!metaData_)
        metaData_ = std::make_shared<MaterialMetaData_internal>(admfIndex_);
    
    
}

#ifdef ADMF_EDIT
void Material_internal::save(bson_t* doc)
{
    std::string layersKey = getNewKey("layers");
    std::string sideKey = getNewKey("side");
    std::string deviceKey = getNewKey("device");
    std::string createdKey = getNewKey("created");
    std::string modifiedKey = getNewKey("modified");
    std::string idKey = getNewKey("id");
    std::string nameKey = getNewKey("name");
    std::string metadataKey = getNewKey("metadata");
    


    ADMF_BSON_APPEND_ARRAY(doc, layersKey, layerArray_, MaterialLayer_internal);
    ADMF_BSON_APPEND_DOCUMENT(doc, sideKey, sideLayer_);
    ADMF_BSON_APPEND_DOCUMENT(doc, deviceKey, device_);
    ADMF_BSON_APPEND_DATE_TIME(doc, createdKey, createTime_);
    ADMF_BSON_APPEND_DATE_TIME(doc, modifiedKey, modifiedTime_);
    ADMF_BSON_APPEND_STRING(doc, idKey, id_);
    ADMF_BSON_APPEND_STRING(doc, nameKey, name_);
    ADMF_BSON_APPEND_DOCUMENT(doc, metadataKey, metaData_);

   
}
#endif
Array<admf::MaterialLayer> Material_internal::getLayerArray()
{
    return Array<admf::MaterialLayer>(layerArray_);
}
MaterialLayer Material_internal::getSideLayer()
{
    return MaterialLayer(sideLayer_);
}

ADMF_DATE Material_internal::getCreatedTime()  //"2018-11-08T12:35:37",
{
    return createTime_;
}

ADMF_DATE Material_internal::getModifiedTime()  //"2020-11-03T07:59:55Z",
{
    return modifiedTime_;
}

MaterialDevice Material_internal::getDevice()
{
    return MaterialDevice(device_);
}

String Material_internal::getId()  //"0001"
{
    return String(id_);
}

String Material_internal::getName()  //"PANTONE 20-0060TPM",
{
    return String(name_);
}

MaterialMetaData Material_internal::getMetaData()
{
    return MaterialMetaData(metaData_);
}


void MaterialDevice_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse (iter, &child))
        return;


    std::string typeKey = getNewKey("type");
    std::string modelKey = getNewKey("model");
    std::string revisionKey = getNewKey("revision");
    std::string serialNumberKey = getNewKey("serialNumber");

    while (bson_iter_next (&child)) {
        std::string keyName = bson_iter_key (&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == typeKey)
        {
            type_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == modelKey)
        {
            model_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == revisionKey)
        {
            revision_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == serialNumberKey)
        {
            serialNumber_ = std::make_shared<String_internal>(admfIndex_, &child);
        }

    }
}

void MaterialDevice_internal::initMissed()
{
    if (!type_)
        type_ = std::make_shared<String_internal>(admfIndex_);
    if (!model_)
        model_ = std::make_shared<String_internal>(admfIndex_);
    if (!revision_)
        revision_ = std::make_shared<String_internal>(admfIndex_);
    if (!serialNumber_)
        serialNumber_ = std::make_shared<String_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void MaterialDevice_internal::save(bson_t* doc)
{
    std::string typeKey = getNewKey("type");
    std::string modelKey = getNewKey("model");
    std::string revisionKey = getNewKey("revision");
    std::string serialNumberKey = getNewKey("serialNumber");


    ADMF_BSON_APPEND_STRING(doc, typeKey, type_);
    ADMF_BSON_APPEND_STRING(doc, modelKey, model_);
    ADMF_BSON_APPEND_STRING(doc, revisionKey, revision_);
    ADMF_BSON_APPEND_STRING(doc, serialNumberKey, serialNumber_);
}
#endif

String MaterialDevice_internal::getType()
{
    return String(type_);
}
String MaterialDevice_internal::getModel()
{
    return String(model_);
}
String MaterialDevice_internal::getRevision()
{
    return String(revision_);
}
String MaterialDevice_internal::getSerialNumber()
{
    return String(serialNumber_);
}

void MaterialMetaData_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse (iter, &child))
        return;


    std::string sourceKey = getNewKey("source");
    std::string typeKey = getNewKey("type");
    std::string versionKey = getNewKey("version");
    
    while (bson_iter_next (&child)) {
        std::string keyName = bson_iter_key (&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == sourceKey)
        {
            source_ = std::make_shared<BinaryData_internal>(admfIndex_, &child);
            source_->name_->str_ = "original";
        }
        else if (keyName == typeKey)
        {
            type_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == versionKey)
        {
            version_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        
    }
    std::string origPath = source_->getNameString() + "." + type_->getInternalString();
    source_->name_->str_ = origPath;

}

void MaterialMetaData_internal::initMissed()
{
    if (!source_)
        source_ = std::make_shared<BinaryData_internal>(admfIndex_);
    if (!type_)
        type_ = std::make_shared<String_internal>(admfIndex_);
    if (!version_)
        version_ = std::make_shared<String_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void MaterialMetaData_internal::save(bson_t* doc)
{
    std::string sourceKey = getNewKey("source");
    std::string typeKey = getNewKey("type");
    std::string versionKey = getNewKey("version");

    ADMF_BSON_APPEND_BINARY(doc, sourceKey, source_);
    ADMF_BSON_APPEND_STRING(doc, typeKey, type_);
    ADMF_BSON_APPEND_STRING(doc, versionKey, version_);
}
#endif

BinaryData MaterialMetaData_internal::getSource()
{
    return BinaryData(source_);
}
admf::String MaterialMetaData_internal::getType()
{
    return String(type_);
}
admf::String MaterialMetaData_internal::getVersion()
{
    return String(version_);
}

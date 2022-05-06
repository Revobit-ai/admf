//
//  admf_custom_internal.cpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#include "admf_internal_header.h"
#include "bson.h"
#ifndef __APPLE__
#include <assert.h>
#endif
using namespace admf_internal;
using namespace admf;

void BaseColorData_internal::load(bson_iter_t *iter) //save
{

    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string typeKey = getNewKey("type");
    std::string solidKey = getNewKey("solid");
    std::string multiKey = getNewKey("multi");
    std::string indexKey = getNewKey("index");
    

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == typeKey)
        {
            type_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == solidKey)
        {
            solid_ = std::make_shared<BaseColorDataSolid_internal>(admfIndex_, &child);
        }
        else if (keyName == multiKey)
        {
            multi_ = std::make_shared<BaseColorDataMulti_internal>(admfIndex_, &child);
        }
        else if (keyName == indexKey)
        {
            index_ = (ADMF_INT)bson_iter_as_int64(&child);
        }

    }
}

void BaseColorData_internal::initMissed()
{
    if (!type_)
        type_ = std::make_shared<String_internal>(admfIndex_);
    if (!solid_)
        solid_ = std::make_shared<BaseColorDataSolid_internal>(admfIndex_);
    if (!multi_)
        multi_ = std::make_shared<BaseColorDataMulti_internal>(admfIndex_);

}
#ifdef ADMF_EDIT
void BaseColorData_internal::save(bson_t* doc)
{
	std::string typeKey = getNewKey("type");
	std::string solidKey = getNewKey("solid");
	std::string multiKey = getNewKey("multi");
    std::string indexKey = getNewKey("index");
 

	ADMF_BSON_APPEND_STRING(doc, typeKey, type_);
    ADMF_BSON_APPEND_INT32(doc, indexKey, index_);
	ADMF_BSON_APPEND_DOCUMENT(doc, solidKey, solid_);
	ADMF_BSON_APPEND_DOCUMENT(doc, multiKey, multi_);

    
}
#endif

String BaseColorData_internal::getType()
{
    return String(type_);
}

BaseColorDataSolid BaseColorData_internal::getSolid()
{
    return BaseColorDataSolid(solid_);
}

BaseColorDataMulti BaseColorData_internal::getMulti()
{
    return BaseColorDataMulti(multi_);
}


admf::ADMF_INT BaseColorData_internal::getIndex()
{
    return index_;
}



#ifdef ADMF_EDIT
void BaseColorData_internal::setIndex(ADMF_INT index)
{
    index_ = index;
}
#endif

void BaseColorDataSolid_internal::load(bson_iter_t *iter) //save
{

    blockArray_ = std::make_shared<Array_internal<admf::BaseColorDataSolidBlock>>([this]() {
        return std::make_shared<BaseColorDataSolidBlock_internal>(admfIndex_, nullptr);
    });

    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
/*
    std::string colorSpaceKey = getNewKey("colorSpace");
    std::string valueKey = getNewKey("value");
    std::string inputBlackKey = getNewKey("inputBlack");
    std::string inputWhiteKey = getNewKey("inputWhite");
    std::string gammaKey = getNewKey("gamma");
    std::string outputBlackKey = getNewKey("outputBlack");
    std::string outputWhiteKey = getNewKey("outputWhite");
 */
    std::string blockKey = getNewKey("block");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        /*
        if (keyName == colorSpaceKey)
        {
            colorSpace_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == valueKey)
        {
            value_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
        else if (keyName == inputBlackKey)
        {
            inputBlack_ = (ADMF_UINT)bson_iter_as_int64(&child);
        }
        else if (keyName == inputWhiteKey)
        {
            inputWhite_ = (ADMF_UINT)bson_iter_as_int64(&child);
        }
        else if (keyName == gammaKey)
        {
            gamma_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == outputBlackKey)
        {
            outputBlack_ = (ADMF_UINT)bson_iter_as_int64(&child);
        }
        else if (keyName == outputWhiteKey)
        {
            outputWhite_ = (ADMF_UINT)bson_iter_as_int64(&child);
        }
        else*/ if (keyName == blockKey)
        {
            if (!BSON_ITER_HOLDS_ARRAY(&child))
                continue;
            bson_t b;
            uint32_t len;
            const uint8_t *data;
            bson_iter_array(&child, &len, &data);
            if (!bson_init_static(&b, data, len))
                continue;

            bson_iter_t iter_;
            if (!bson_iter_init(&iter_, &b))
                continue;

            while (bson_iter_next(&iter_))
            {
                auto block = std::make_shared<BaseColorDataSolidBlock_internal>(admfIndex_, &iter_);
                blockArray_->pushBack(block);
            }
        }
    }
}

void BaseColorDataSolid_internal::initMissed()
{
    /*
    if (!colorSpace_)
        colorSpace_ = std::make_shared<String_internal>(admfIndex_);
    if (!value_)
        value_ = std::make_shared<ColorRGB_internal>(admfIndex_);
     */
}
#ifdef ADMF_EDIT
void BaseColorDataSolid_internal::save(bson_t *doc)
{
/*
    std::string colorSpaceKey = getNewKey("colorSpace");
    std::string valueKey = getNewKey("value");
    std::string inputBlackKey = getNewKey("inputBlack");
    std::string inputWhiteKey = getNewKey("inputWhite");
    std::string gammaKey = getNewKey("gamma");
    std::string outputBlackKey = getNewKey("outputBlack");
    std::string outputWhiteKey = getNewKey("outputWhite");
 */
    std::string blockKey = getNewKey("block");

    /*
    ADMF_BSON_APPEND_STRING(doc, colorSpaceKey, colorSpace_);
    ADMF_BSON_APPEND_DOCUMENT(doc, valueKey, value_);
    ADMF_BSON_APPEND_INT32(doc, inputBlackKey, inputBlack_);
    ADMF_BSON_APPEND_INT32(doc, inputWhiteKey, inputWhite_);
    ADMF_BSON_APPEND_DOUBLE(doc, gammaKey, gamma_);
    ADMF_BSON_APPEND_INT32(doc, outputBlackKey, outputBlack_);
    ADMF_BSON_APPEND_INT32(doc, outputWhiteKey, outputWhite_);
     */
    ADMF_BSON_APPEND_ARRAY(doc, blockKey, blockArray_, BaseColorDataSolidBlock_internal);
}
#endif
/*
String BaseColorDataSolid_internal::getColorSpace() //"srgb",
{
    return String(colorSpace_);
}

ColorRGB BaseColorDataSolid_internal::getValue() //
{
    return ColorRGB(value_);
}

ADMF_INT BaseColorDataSolid_internal::getInputBlack() //0,
{
    return inputBlack_;
}

ADMF_INT BaseColorDataSolid_internal::getInputWhite() //255,
{
    return inputWhite_;
}

ADMF_FLOAT BaseColorDataSolid_internal::getGamma() //1.0,
{
    return gamma_;
}

ADMF_INT BaseColorDataSolid_internal::getOutputBlack() //0,
{
    return outputBlack_;
}

ADMF_INT BaseColorDataSolid_internal::getOutputWhite() //0,
{
    return outputWhite_;
}
*/
Array<BaseColorDataSolidBlock> BaseColorDataSolid_internal::getBlockArray()
{
    return blockArray_;
}

void BaseColorDataSolidBlock_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string nameKey = getNewKey("name");
    std::string typeKey = getNewKey("type");
    std::string valueKey = getNewKey("value");
    std::string isOriginalKey = getNewKey("isOriginal");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == nameKey)
        {
            name_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == typeKey)
        {
            type_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == valueKey)
        {
            value_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == isOriginalKey)
        {
            isOriginal_ = (ADMF_BYTE)bson_iter_as_int64(&child);
        }
    }
}

void BaseColorDataSolidBlock_internal::initMissed()
{
    if (!name_)
        name_ = std::make_shared<String_internal>(admfIndex_);
    if (!type_)
        type_ = std::make_shared<String_internal>(admfIndex_);
    if (!value_)
        value_ = std::make_shared<String_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void BaseColorDataSolidBlock_internal::save(bson_t *doc)
{
    std::string nameKey = getNewKey("name");
    std::string typeKey = getNewKey("type");
    std::string valueKey = getNewKey("value");
    std::string isOriginalKey = getNewKey("isOriginal");

    ADMF_BSON_APPEND_STRING(doc, nameKey, name_);
    ADMF_BSON_APPEND_STRING(doc, typeKey, type_);
    ADMF_BSON_APPEND_STRING(doc, valueKey, value_);
    ADMF_BSON_APPEND_INT32(doc, isOriginalKey, isOriginal_);
}
#endif
String BaseColorDataSolidBlock_internal::getName() //"(166,202,240)",
{
    return name_;
}

String BaseColorDataSolidBlock_internal::getType() //"RGB",
{
    return type_;
}

String BaseColorDataSolidBlock_internal::getValue() //"166,202,240",
{
    return value_;
}

ADMF_BYTE BaseColorDataSolidBlock_internal::isOriginal()
{
    return isOriginal_;
}

#ifdef ADMF_EDIT
void BaseColorDataSolidBlock_internal::setOriginal(admf::ADMF_BYTE isOriginal)
{
    isOriginal_ = isOriginal;
}
#endif

void BaseColorDataMulti_internal::load(bson_iter_t *iter) //save
{
    blockArray_ = std::make_shared<Array_internal<admf::BaseColorDataMultiBlock>>([this]() {
        return std::make_shared<BaseColorDataMultiBlock_internal>(admfIndex_, nullptr);
    });

    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;


    std::string blockKey = getNewKey("block");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == blockKey)
        {
            if (!BSON_ITER_HOLDS_ARRAY(&child))
                continue;
            bson_t b;
            uint32_t len;
            const uint8_t *data;
            bson_iter_array(&child, &len, &data);
            if (!bson_init_static(&b, data, len))
                continue;

            bson_iter_t iter_;
            if (!bson_iter_init(&iter_, &b))
                continue;

            while (bson_iter_next(&iter_))
            {
                auto block = std::make_shared<BaseColorDataMultiBlock_internal>(admfIndex_, &iter_);
                blockArray_->pushBack(block);
            }
        }
    }
}

void BaseColorDataMulti_internal::initMissed()
{

}
#ifdef ADMF_EDIT
void BaseColorDataMulti_internal::save(bson_t *doc)
{
    std::string blockKey = getNewKey("block");
    ADMF_BSON_APPEND_ARRAY(doc, blockKey, blockArray_, BaseColorDataMultiBlock_internal);
}
#endif


Array<BaseColorDataMultiBlock> BaseColorDataMulti_internal::getBlockArray()
{
    return blockArray_;
}



void BaseColorDataMultiBlock_internal::load(bson_iter_t *iter) //save
{

    maskArray_ = std::make_shared<Array_internal<admf::BaseColorDataMultiBlockMask>>([this]() {
        return std::make_shared<BaseColorDataMultiBlockMask_internal>(admfIndex_, nullptr);
    });

    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string nameKey = getNewKey("name");
    std::string maskKey = getNewKey("mask");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == nameKey)
        {
            name_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        if (keyName == maskKey)
        {
            if (!BSON_ITER_HOLDS_ARRAY(&child))
                continue;
            bson_t b;
            uint32_t len;
            const uint8_t *data;
            bson_iter_array(&child, &len, &data);
            if (!bson_init_static(&b, data, len))
                continue;

            bson_iter_t iter_;
            if (!bson_iter_init(&iter_, &b))
                continue;

            while (bson_iter_next(&iter_))
            {
                auto mask = std::make_shared<BaseColorDataMultiBlockMask_internal>(admfIndex_, &iter_);
                maskArray_->pushBack(mask);
            }
        }
    }
}

void BaseColorDataMultiBlock_internal::initMissed()
{
    if (!name_)
        name_ = std::make_shared<String_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void BaseColorDataMultiBlock_internal::save(bson_t *doc)
{
    std::string nameKey = getNewKey("name");
    std::string maskKey = getNewKey("mask");

    ADMF_BSON_APPEND_STRING(doc, nameKey, name_);
    ADMF_BSON_APPEND_ARRAY(doc, maskKey, maskArray_, BaseColorDataMultiBlockMask_internal);
}
#endif


String BaseColorDataMultiBlock_internal::getName()
{
    return name_;
}

Array<BaseColorDataMultiBlockMask> BaseColorDataMultiBlock_internal::getMaskArray()
{
    return maskArray_;
}


void BaseColorDataMultiBlockMask_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string maskKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == maskKey)
        {
            mask_ = std::make_shared<Texture_internal>(admfIndex_, &child);
        }
        else if (keyName == valueKey)
        {
            value_ = std::make_shared<String_internal>(admfIndex_, &child);
        }

    }
}

void BaseColorDataMultiBlockMask_internal::initMissed()
{
    if (!mask_)
        mask_ = std::make_shared<Texture_internal>(admfIndex_);
    if (!value_)
        value_ = std::make_shared<String_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void BaseColorDataMultiBlockMask_internal::save(bson_t *doc)
{
    std::string maskKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");


    ADMF_BSON_APPEND_DOCUMENT(doc, maskKey, mask_);
    ADMF_BSON_APPEND_STRING(doc, valueKey, value_);

}
#endif

Texture BaseColorDataMultiBlockMask_internal::getMask()
{
    return mask_;
}

String BaseColorDataMultiBlockMask_internal::getValue()
{
    return value_;
}

/*

void BaseColorChangeColorData_internal::load(bson_iter_t *iter) //save
{
    
    if (iter == nullptr)
        return;
    
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    
    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
    
    std::string enabledKey = getNewKey("enabled");
    std::string bottomSKey = getNewKey("bottomS");
    std::string bottomVKey = getNewKey("bottomV");
    std::string meanSKey = getNewKey("meanS");
    std::string meanVKey = getNewKey("meanV");
    std::string kSKey = getNewKey("kS");
    std::string kVKey = getNewKey("kV");
    
    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == enabledKey)
        {
            enabled_ = (ADMF_BYTE)bson_iter_as_int64(&child);
        }
        else if (keyName == bottomSKey)
        {
            bottomS_ = (ADMF_DOUBLE)bson_iter_as_double(&child);
        }
        else if (keyName == bottomVKey)
        {
            bottomV_ = (ADMF_DOUBLE)bson_iter_as_double(&child);
        }
        else if (keyName == meanSKey)
        {
            meanS_ = (ADMF_DOUBLE)bson_iter_as_double(&child);
        }
        else if (keyName == meanVKey)
        {
            meanV_ = (ADMF_DOUBLE)bson_iter_as_double(&child);
        }
        else if (keyName == kSKey)
        {
            kS_ = (ADMF_DOUBLE)bson_iter_as_double(&child);
        }
        else if (keyName == kVKey)
        {
            kV_ = (ADMF_DOUBLE)bson_iter_as_double(&child);
        }
   
    }
}

void BaseColorChangeColorData_internal::initMissed()
{

}
#ifdef ADMF_EDIT
void BaseColorChangeColorData_internal::save(bson_t* doc)
{
    std::string enabledKey = getNewKey("enabled");
    std::string bottomSKey = getNewKey("bottomS");
    std::string bottomVKey = getNewKey("bottomV");
    std::string meanSKey = getNewKey("meanS");
    std::string meanVKey = getNewKey("meanV");
    std::string kSKey = getNewKey("kS");
    std::string kVKey = getNewKey("kV");
    

    ADMF_BSON_APPEND_INT64(doc, enabledKey, enabled_);
    ADMF_BSON_APPEND_DOUBLE(doc, bottomSKey, bottomS_);
    ADMF_BSON_APPEND_DOUBLE(doc, bottomVKey, bottomV_);
    ADMF_BSON_APPEND_DOUBLE(doc, meanSKey, meanS_);
    ADMF_BSON_APPEND_DOUBLE(doc, meanVKey, meanV_);
    ADMF_BSON_APPEND_DOUBLE(doc, kSKey, kS_);
    ADMF_BSON_APPEND_DOUBLE(doc, kVKey, kV_);
    
    
}
#endif
*/


//
//  admf_physics_internal.cpp
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


void PhysicsComposition_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string nameKey = getNewKey("name");
    std::string percentageKey = getNewKey("percentage");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == nameKey)
        {
            name_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == percentageKey)
        {
            percentage_ = (admf::ADMF_UINT)bson_iter_as_int64(&child);
        }
    }
}

void PhysicsComposition_internal::initMissed()
{
    if (!name_)
        name_ = std::make_shared<String_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void PhysicsComposition_internal::save(bson_t *doc)
{
    std::string nameKey = getNewKey("name");
    std::string percentageKey = getNewKey("percentage");

    ADMF_BSON_APPEND_STRING(doc, nameKey, name_);
    ADMF_BSON_APPEND_INT32(doc, percentageKey, percentage_);
}
#endif

admf::String PhysicsComposition_internal::getName() //"Cotton"
{
    return name_;
}
admf::ADMF_UINT PhysicsComposition_internal::getPercentage() //万分比的整数值 例如30.1% => 3010
{
    return percentage_;
}

void Physics_internal::load(bson_iter_t *iter) //save
{
    compositionArray_ = std::make_shared<Array_internal<admf::PhysicsComposition>>([this]() {
        return std::make_shared<PhysicsComposition_internal>(admfIndex_, nullptr);
    });

    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string deviceKey = getNewKey("device");
    std::string pathKey = getNewKey("path");
    std::string compositionKey = getNewKey("composition");
    std::string thicknessKey = getNewKey("thickness");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == deviceKey)
        {
            deviceString_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == pathKey)
        {
            binary_ = std::make_shared<BinaryData_internal>(admfIndex_, &child);
        }
        else if (keyName == compositionKey)
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
                auto composition = std::make_shared<PhysicsComposition_internal>(admfIndex_, &iter_);
                compositionArray_->pushBack(composition);
                //compositionVector_.emplace_back(composition);
            }
        }
        else if (keyName == thicknessKey)
        {
            thickness_ = (ADMF_FLOAT)bson_iter_as_double(iter);
        }
    }
}

void Physics_internal::initMissed()
{
    if (!deviceString_)
        deviceString_ = std::make_shared<String_internal>(admfIndex_);
    if (!binary_)
        binary_ = std::make_shared<BinaryData_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void Physics_internal::save(bson_t *doc)
{
    std::string deviceKey = getNewKey("device");
    std::string pathKey = getNewKey("path");
    std::string compositionKey = getNewKey("composition");
    std::string thicknessKey = getNewKey("thickness");

    ADMF_BSON_APPEND_STRING(doc, deviceKey, deviceString_);
    ADMF_BSON_APPEND_BINARY(doc, pathKey, binary_);
    ADMF_BSON_APPEND_ARRAY(doc, compositionKey, compositionArray_, PhysicsComposition_internal);
    ADMF_BSON_APPEND_DOUBLE(doc, thicknessKey, thickness_);
}
#endif
String Physics_internal::getDevice() //"fab"
{
    return String(deviceString_);
}

BinaryData Physics_internal::getBinaryData() //content of "/browzwear_physics_example.json"
{
    return BinaryData(binary_);
}

Array<PhysicsComposition> Physics_internal::getCompositionArray()
{
    //return std::make_shared<admf::Array_<admf::PhysicsComposition>>(compositionArray_);
    return Array<PhysicsComposition>(compositionArray_);
}

admf::ADMF_FLOAT Physics_internal::getThinkness() //0.20203900337219239
{
    return thickness_;
}

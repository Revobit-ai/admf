//
//  admf_custom_internal.cpp
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
void BaseColor_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");
    //std::string changeColorKey = getNewKey("changeColor");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == colorKey)
        {
            data_ = std::make_shared<BaseColorData_internal>(admfIndex_, &child);
        }
        /*
        else if (changeColorKey == changeColorKey)
        {
            changeColor_ = std::make_shared<BaseColorChangeColorData_internal>(admfIndex_, &child);
        }
        */
    }
}

void BaseColor_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }

    if (!data_)
        data_ = std::make_shared<BaseColorData_internal>(admfIndex_);
    /*
    if (!changeColor_)
        changeColor_ = std::make_shared<BaseColorChangeColorData_internal>(admfIndex_);
     */
}
#ifdef ADMF_EDIT
void BaseColor_internal::save(bson_t *doc)
{
    auto type = data_->getType();

    if (type->isEmpty())
    {
        auto solid = data_->getSolid();
        auto blockArray = solid->getBlockArray();
        if (blockArray->size() == 0)
        {
            auto solidBlock = blockArray->append();
            solidBlock->setOriginal(1);
            type->setString("solid");
        }
    
    }
    
    
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");
    std::string changeColorKey = getNewKey("changeColor");

    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, data_);
    //ADMF_BSON_APPEND_DOCUMENT(doc, changeColorKey, changeColor_);
}
#endif
BaseColorData BaseColor_internal::getData()
{
    return BaseColorData(data_);
}

Texture BaseColor_internal::getTexture()
{
    return Texture(texture_);
}
/*
BaseColorChangeColorData BaseColor_internal::getChangeColorData()
{
    return BaseColorChangeColorData(changeColor_);
    
};
*/
void Specular_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");
    std::string valueKey = getNewKey("value");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == colorKey)
        {
            color_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
        else if (keyName == valueKey)
        {
            value_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
    }
}

void Specular_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }

    if (!color_)
        color_ = std::make_shared<ColorRGB_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void Specular_internal::save(bson_t *doc)
{
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");
    std::string valueKey = getNewKey("value");

    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, color_);
    ADMF_BSON_APPEND_DOUBLE(doc, valueKey, value_);
}
#endif
Texture Specular_internal::getTexture()
{
    return Texture(texture_);
}

ColorRGB Specular_internal::getColor()
{
    return ColorRGB(color_);
}



void Emissive_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
   
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
  
    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
   
    std::string textureKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");
    std::string colorKey = getNewKey("color");
  
    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        /*printf("Found element key: \"%s\"\n", keyName.c_str()); */
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == valueKey)
        {
            value_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == colorKey)
        {
            color_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
    }
}

void Emissive_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }
    
    if (!color_)
        color_ = std::make_shared<ColorRGB_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void Emissive_internal::save(bson_t *doc)
{
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");
    std::string colorKey = getNewKey("color");
    
    
    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOUBLE(doc, valueKey, value_);
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, color_);

}
#endif
Texture Emissive_internal::getTexture()
{
    return Texture(texture_);
}

ColorRGB Emissive_internal::getColor()
{
    return ColorRGB(color_);
}



void SubSurfaceColor_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
    
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    
    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
    
    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");
    
    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        /*printf("Found element key: \"%s\"\n", keyName.c_str()); */
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == colorKey)
        {
            color_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
    }
}

void SubSurfaceColor_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }
    
    if (!color_)
        color_ = std::make_shared<ColorRGB_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void SubSurfaceColor_internal::save(bson_t *doc)
{
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");
    
    
    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, color_);
    
}
#endif
Texture SubSurfaceColor_internal::getTexture()
{
    return Texture(texture_);
}

ColorRGB SubSurfaceColor_internal::getColor()
{
    return ColorRGB(color_);
}

void ClearCoatColor_internal::load(bson_iter_t* iter)
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        /*printf("Found element key: \"%s\"\n", keyName.c_str()); */
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == colorKey)
        {
            color_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
    }
}

void ClearCoatColor_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }

    if (!color_)
        color_ = std::make_shared<ColorRGB_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void ClearCoatColor_internal::save(bson_t* doc)
{
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");


    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, color_);

}
#endif

Texture ClearCoatColor_internal::getTexture()
{
    return Texture(texture_);
}

ColorRGB ClearCoatColor_internal::getColor()
{
    return ColorRGB(color_);
}

void SheenColor_internal::load(bson_iter_t* iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        /*printf("Found element key: \"%s\"\n", keyName.c_str()); */
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == colorKey)
        {
            color_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
    }
}

void SheenColor_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }

    if (!color_)
        color_ = std::make_shared<ColorRGB_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void SheenColor_internal::save(bson_t* doc)
{
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string colorKey = getNewKey("color");


    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, color_);

}
#endif
Texture SheenColor_internal::getTexture()
{
    return Texture(texture_);
}

ColorRGB SheenColor_internal::getColor()
{
    return ColorRGB(color_);
}


void Height_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
    
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    
    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
    
    std::string textureKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");
    std::string levelKey = getNewKey("level");
    
    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        
        if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == valueKey)
        {
            value_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        if (keyName == levelKey)
        {
            level_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        
    }
}

void Height_internal::initMissed()
{
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }
}

#ifdef ADMF_EDIT
void Height_internal::save(bson_t *doc)
{
    texture_->setType(getTextureType());
    std::string textureKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");
    std::string levelKey = getNewKey("level");
    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOUBLE(doc, valueKey, value_);
    ADMF_BSON_APPEND_DOUBLE(doc, levelKey, level_);
}
#endif

Texture Height_internal::getTexture()
{
    return Texture(texture_);
}

#ifdef ADMF_EDIT
#define TextureAndValue_Save_Implementation(classname)  \
void classname::save(bson_t* doc)                                             \
{                                                                             \
    texture_->setType(getTextureType());                                           \
    std::string textureKey = getNewKey("texture");                            \
    std::string valueKey = getNewKey("value");                                \
    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);                      \
    ADMF_BSON_APPEND_DOUBLE(doc, valueKey, value_);                            \
} 
#else
#define TextureAndValue_Save_Implementation(x)
#endif

#define TextureAndValueContainer_Internal_Implementation(classname)               \
    void classname::load(bson_iter_t *iter)                                       \
    {                                                                             \
        if (iter == nullptr)                                                      \
            return;                                                               \
                                                                                  \
        if (!BSON_ITER_HOLDS_DOCUMENT(iter))                                      \
            return;                                                               \
                                                                                  \
        bson_iter_t child;                                                        \
        if (!bson_iter_recurse(iter, &child))                                     \
            return;                                                               \
                                                                                  \
        std::string textureKey = getNewKey("texture");                            \
        std::string valueKey = getNewKey("value");                                \
                                                                                  \
        while (bson_iter_next(&child))                                            \
        {                                                                         \
            std::string keyName = bson_iter_key(&child);                          \
            assert(bson_iter_value(&child) != nullptr);                           \
            /*printf("Found element key: \"%s\"\n", keyName.c_str()); */              \
            if (keyName == textureKey)                                            \
            {                                                                     \
                texture_ = std::make_shared<Texture_internal>(admfIndex_, &child); \
                texture_->setType(getTextureType());                              \
            }                                                                     \
            else if (keyName == valueKey)                                         \
            {                                                                     \
                value_ = (ADMF_FLOAT)bson_iter_as_double(&child);                  \
            }                                                                     \
        }                                                                         \
    }                                                                             \
                                                                                  \
    Texture classname::getTexture()                                               \
    {                                                                             \
        return Texture(texture_);                                                 \
    }                                                                             \
                                                                                  \
    ADMF_FLOAT classname::getValue()                                               \
    {                                                                             \
        return value_;                                                            \
    }                                                                             \
                                                                                  \
    void classname::initMissed()                                                  \
    {                                                                             \
        if (!texture_)                                                            \
        {                                                                         \
            texture_ = std::make_shared<Texture_internal>(admfIndex_);             \
            texture_->setType(getTextureType());                                  \
        }                                                                         \
    }                                                                             \
                                                                                  \
    TextureAndValue_Save_Implementation(classname)                                                          \


#ifdef ADMF_EDIT
#define Texture_Save_Implementation(classname)  \
void classname::save(bson_t* doc)                                             \
{                                                                             \
    texture_->setType(getTextureType());                                           \
    std::string textureKey = getNewKey("texture");                            \
    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);                      \
} 
#else
#define Texture_Save_Implementation(x)
#endif

#define TextureContainer_Internal_Implementation(classname)               \
    void classname::load(bson_iter_t *iter)                                       \
    {                                                                             \
        if (iter == nullptr)                                                      \
            return;                                                               \
                                                                                  \
        if (!BSON_ITER_HOLDS_DOCUMENT(iter))                                      \
            return;                                                               \
                                                                                  \
        bson_iter_t child;                                                        \
        if (!bson_iter_recurse(iter, &child))                                     \
            return;                                                               \
                                                                                  \
        std::string textureKey = getNewKey("texture");                            \
                                                                                  \
        while (bson_iter_next(&child))                                            \
        {                                                                         \
            std::string keyName = bson_iter_key(&child);                          \
            assert(bson_iter_value(&child) != nullptr);                           \
            /*printf("Found element key: \"%s\"\n", keyName.c_str()); */              \
            if (keyName == textureKey)                                            \
            {                                                                     \
                texture_ = std::make_shared<Texture_internal>(admfIndex_, &child); \
                texture_->setType(getTextureType());                              \
            }                                                                     \
        }                                                                         \
    }                                                                             \
                                                                                  \
    Texture classname::getTexture()                                               \
    {                                                                             \
        return Texture(texture_);                                                 \
    }                                                                             \
                                                                                  \
                                                                                    \
    void classname::initMissed()                                                  \
    {                                                                             \
        if (!texture_)                                                            \
        {                                                                         \
            texture_ = std::make_shared<Texture_internal>(admfIndex_);             \
            texture_->setType(getTextureType());                                  \
        }                                                                         \
    }                                                                             \
                                                                                  \
    Texture_Save_Implementation(classname)                                          \



TextureAndValueContainer_Internal_Implementation(Normal_internal);
TextureAndValueContainer_Internal_Implementation(Alpha_internal);
TextureAndValueContainer_Internal_Implementation(Metalness_internal);
TextureAndValueContainer_Internal_Implementation(Roughness_internal);
TextureAndValueContainer_Internal_Implementation(Glossiness_internal);
TextureAndValueContainer_Internal_Implementation(Anisotropy_internal);
TextureAndValueContainer_Internal_Implementation(AnisotropyRotation_internal);
TextureContainer_Internal_Implementation(AmbientOcclusion_internal);
TextureContainer_Internal_Implementation(ClearCoatNormal_internal);
TextureAndValueContainer_Internal_Implementation(ClearCoatRoughness_internal);
TextureAndValueContainer_Internal_Implementation(ClearCoatValue_internal);
TextureAndValueContainer_Internal_Implementation(SheenTint_internal);
TextureAndValueContainer_Internal_Implementation(SheenValue_internal);
TextureAndValueContainer_Internal_Implementation(SheenGloss_internal);
TextureAndValueContainer_Internal_Implementation(SheenRoughness_internal);
TextureAndValueContainer_Internal_Implementation(SpecularTint_internal);
TextureAndValueContainer_Internal_Implementation(SubSurfaceRadius_internal);
TextureAndValueContainer_Internal_Implementation(SubSurfaceValue_internal);
TextureAndValueContainer_Internal_Implementation(Transmission_internal);


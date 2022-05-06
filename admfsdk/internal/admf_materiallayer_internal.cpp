//
//  admf_materiallayer_internal.cpp
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

void MaterialLayer_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string typeKey = getNewKey("type");
    std::string shaderKey = getNewKey("shader");
    std::string previewKey = getNewKey("preview");
    std::string basicKey = getNewKey("basic");
    std::string specKey = getNewKey("spec");
    std::string enabledKey = getNewKey("enabled");
    std::string nameKey = getNewKey("name");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == typeKey)
        {
            type_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == shaderKey)
        {
            shader_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == previewKey)
        {
            preview_ = std::make_shared<BinaryData_internal>(admfIndex_, &child);
            preview_->name_->str_ = "preview.png";
        }
        else if (keyName == basicKey)
        {
            basic_ = std::make_shared<LayerBasic_internal>(admfIndex_, &child);
        }
        else if (keyName == specKey)
        {
            spec_ = std::make_shared<LayerSpec_internal>(admfIndex_, &child);
        }
        else if (keyName == enabledKey)
        {
            enabled_ = (ADMF_BYTE)bson_iter_as_int64(&child);
        }
        else if (keyName == nameKey)
        {
            name_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
    }
}

void MaterialLayer_internal::initMissed()
{
    if (!type_)
        type_ = std::make_shared<String_internal>(admfIndex_);
    if (!shader_)
        shader_ = std::make_shared<String_internal>(admfIndex_);
    if (!preview_)
    {
        preview_ = std::make_shared<BinaryData_internal>(admfIndex_);
        preview_->name_->str_ = "preview.png";
    }
    if (!basic_)
        basic_ = std::make_shared<LayerBasic_internal>(admfIndex_);
    if (!spec_)
        spec_ = std::make_shared<LayerSpec_internal>(admfIndex_);
    if (!name_)
        name_ = std::make_shared<String_internal>(admfIndex_);
}

#ifdef ADMF_EDIT
void MaterialLayer_internal::save(bson_t *doc)
{
    std::string typeKey = getNewKey("type");
    std::string shaderKey = getNewKey("shader");
    std::string previewKey = getNewKey("preview");
    std::string basicKey = getNewKey("basic");
    std::string specKey = getNewKey("spec");
    std::string enabledKey = getNewKey("enabled");
    std::string nameKey = getNewKey("name");

    ADMF_BSON_APPEND_STRING(doc, typeKey, type_);
    ADMF_BSON_APPEND_STRING(doc, shaderKey, shader_);
    ADMF_BSON_APPEND_BINARY(doc, previewKey, preview_);
    ADMF_BSON_APPEND_DOCUMENT(doc, basicKey, basic_);
    ADMF_BSON_APPEND_DOCUMENT(doc, specKey, spec_);
    ADMF_BSON_APPEND_INT32(doc, enabledKey, enabled_);
    ADMF_BSON_APPEND_STRING(doc, nameKey, name_);
}
#endif
String MaterialLayer_internal::getType() //"Fabric",
{
    return String(type_);
}
String MaterialLayer_internal::getShader() //着色类型
{
    return String(shader_);
}

String MaterialLayer_internal::getName()
{
    return name_;
}

BinaryData MaterialLayer_internal::getPreview() //content of "/preview.png"
{
    return BinaryData(preview_);
}

LayerBasic MaterialLayer_internal::getBasic()
{
    return LayerBasic(basic_);
}

LayerSpec MaterialLayer_internal::getSpec()
{
    return LayerSpec(spec_);
}

admf::ADMF_BYTE MaterialLayer_internal::isEnabled()
{
    return enabled_;
}
#ifdef ADMF_EDIT
void MaterialLayer_internal::setEnabled(admf::ADMF_BYTE enabled) 
{
    enabled_ = enabled;
}
#endif

void LayerSpec_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string refractionKey = getNewKey("refraction");


    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == refractionKey)
        {
            refraction_ = std::make_shared<LayerRefraction_internal>(admfIndex_, &child);
        }

    }
}

void LayerSpec_internal::initMissed()
{
    if (!refraction_)
        refraction_ = std::make_shared<LayerRefraction_internal>(admfIndex_);
}
#ifdef ADMF_EDIT
void LayerSpec_internal::save(bson_t *doc)
{
    std::string refractionKey = getNewKey("refraction");


    ADMF_BSON_APPEND_DOCUMENT(doc, refractionKey, refraction_);

}
#endif

LayerRefraction LayerSpec_internal::getRefraction()
{
    return LayerRefraction(refraction_);
}



void LayerRefraction_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
    std::string colorKey = getNewKey("color");
    std::string glossinessKey = getNewKey("glossiness");
    std::string textureKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");
    
    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == colorKey)
        {
            color_ = std::make_shared<ColorRGB_internal>(admfIndex_, &child);
        }
        else if (keyName == glossinessKey)
        {
            glossiness_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
        else if (keyName == textureKey)
        {
            texture_ = std::make_shared<Texture_internal>(admfIndex_, &child);
            texture_->setType(getTextureType());
        }
        else if (keyName == valueKey)
        {
            value_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
    }
}

void LayerRefraction_internal::initMissed()
{
    if (!color_)
        color_ = std::make_shared<ColorRGB_internal>(admfIndex_);
    if (!texture_)
    {
        texture_ = std::make_shared<Texture_internal>(admfIndex_);
        texture_->setType(getTextureType());
    }
}
#ifdef ADMF_EDIT
void LayerRefraction_internal::save(bson_t *doc)
{
    std::string colorKey = getNewKey("color");
    std::string glossinessKey = getNewKey("glossiness");
    std::string textureKey = getNewKey("texture");
    std::string valueKey = getNewKey("value");
    ADMF_BSON_APPEND_DOCUMENT(doc, colorKey, color_);
    ADMF_BSON_APPEND_DOUBLE(doc, glossinessKey, glossiness_);
    ADMF_BSON_APPEND_DOCUMENT(doc, textureKey, texture_);
    ADMF_BSON_APPEND_DOUBLE(doc, valueKey, value_);
}
#endif


void LayerBasic_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;
    
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;
    std::string emissiveKey = getNewKey("emissive");
    std::string basecolorKey = getNewKey("basecolor");
    std::string normalKey = getNewKey("normal");
    std::string alphaKey = getNewKey("alpha");
    std::string metalnessKey = getNewKey("metalness");
    std::string roughnessKey = getNewKey("roughness");
    std::string specularKey = getNewKey("specular");
    std::string glossinessKey = getNewKey("glossiness");
    std::string anisotropyKey = getNewKey("anisotropy");
    std::string anisotropyRotationKey = getNewKey("anisotropyRotation");
    std::string ambientOcclusionKey = getNewKey("ambientOcclusion");
    std::string heightKey = getNewKey("height");
    
    
    std::string clearCoatNormalKey = getNewKey("clearCoatNormal");
    std::string clearCoatRoughnessKey = getNewKey("clearCoatRoughness");
    std::string clearCoatValueKey = getNewKey("clearCoatValue");
    std::string clearCoatColorKey = getNewKey("clearCoatColor");
    std::string sheenTintKey = getNewKey("sheenTint");
    std::string sheenValueKey = getNewKey("sheenValue");
    std::string sheenColorKey = getNewKey("sheenColor");
    std::string sheenGlossKey = getNewKey("sheenGloss");
    std::string sheenRoughnessKey = getNewKey("sheenRoughness");
    std::string specularTintKey = getNewKey("specularTint");
    std::string subSurfaceColorKey = getNewKey("subSurfaceColor");
    std::string subSurfaceRadiusKey = getNewKey("subSurfaceRadius");
    std::string subSurfaceValueKey = getNewKey("subSurfaceValue");
    std::string transmissionKey = getNewKey("transmission");
    
    
    std::string transformKey = getNewKey("transform");
    

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == emissiveKey)
        {
            double emissiveValue = 0;
            if (bson_iter_can_convert_to_double(iter))
            {
                emissiveValue = (ADMF_FLOAT)bson_iter_as_double(&child);
                emissive_ = std::make_shared<Emissive_internal>(admfIndex_);
                emissive_->setValue(emissiveValue);
            }

            else {
                emissive_ = std::make_shared<Emissive_internal>(admfIndex_, &child);
            }

        }

        else if (keyName == basecolorKey)
            base_ = std::make_shared<BaseColor_internal>(admfIndex_, &child);
        else if (keyName == normalKey)
            normal_ = std::make_shared<Normal_internal>(admfIndex_, &child);
        else if (keyName == alphaKey)
            alpha_ = std::make_shared<Alpha_internal>(admfIndex_, &child);
        else if (keyName == metalnessKey)
            metalness_ = std::make_shared<Metalness_internal>(admfIndex_, &child);
        else if (keyName == roughnessKey)
            roughness_ = std::make_shared<Roughness_internal>(admfIndex_, &child);
        else if (keyName == specularKey)
            specular_ = std::make_shared<Specular_internal>(admfIndex_, &child);
        else if (keyName == glossinessKey)
            glossiness_ = std::make_shared<Glossiness_internal>(admfIndex_, &child);
        else if (keyName == anisotropyKey)
            anisotropy_ = std::make_shared<Anisotropy_internal>(admfIndex_, &child);
        else if (keyName == anisotropyRotationKey)
            anisotropyRotation_ = std::make_shared<AnisotropyRotation_internal>(admfIndex_, &child);
        else if (keyName == transformKey)
            transform_ = std::make_shared<LayerTransform_internal>(admfIndex_, &child);
        else if (keyName == ambientOcclusionKey)
            ambientOcclusion_ = std::make_shared<AmbientOcclusion_internal>(admfIndex_, &child);
        else if (keyName == clearCoatNormalKey)
            clearCoatNormal_ = std::make_shared<ClearCoatNormal_internal>(admfIndex_, &child);
        else if (keyName == clearCoatRoughnessKey)
            clearCoatRoughness_ = std::make_shared<ClearCoatRoughness_internal>(admfIndex_, &child);
        else if (keyName == clearCoatValueKey)
            clearCoatValue_ = std::make_shared<ClearCoatValue_internal>(admfIndex_, &child);
        else if (keyName == clearCoatColorKey)
            clearCoatColor_ = std::make_shared<ClearCoatColor_internal>(admfIndex_, &child);
        else if (keyName == sheenTintKey)
            sheenTint_ = std::make_shared<SheenTint_internal>(admfIndex_, &child);
        else if (keyName == sheenValueKey)
            sheenValue_ = std::make_shared<SheenValue_internal>(admfIndex_, &child);
        else if (keyName == sheenColorKey)
            sheenColor_ = std::make_shared<SheenColor_internal>(admfIndex_, &child);
        else if (keyName == sheenGlossKey)
            sheenGloss_ = std::make_shared<SheenGloss_internal>(admfIndex_, &child);
        else if (keyName == sheenRoughnessKey)
            sheenRoughness_ = std::make_shared<SheenRoughness_internal>(admfIndex_, &child);
        else if (keyName == specularTintKey)
            specularTint_ = std::make_shared<SpecularTint_internal>(admfIndex_, &child);
        else if (keyName == subSurfaceColorKey)
            subSurfaceColor_ = std::make_shared<SubSurfaceColor_internal>(admfIndex_, &child);
        else if (keyName == subSurfaceRadiusKey)
            subSurfaceRadius_ = std::make_shared<SubSurfaceRadius_internal>(admfIndex_, &child);
        else if (keyName == subSurfaceValueKey)
            subSurfaceValue_ = std::make_shared<SubSurfaceValue_internal>(admfIndex_, &child);
        else if (keyName == transmissionKey)
            transmission_ = std::make_shared<Transmission_internal>(admfIndex_, &child);
        else if (keyName == heightKey)
            height_ = std::make_shared<Height_internal>(admfIndex_, &child);
    }
}

void LayerBasic_internal::initMissed()
{
    if (!base_)
        base_ = std::make_shared<BaseColor_internal>(admfIndex_);
    if (!normal_)
        normal_ = std::make_shared<Normal_internal>(admfIndex_);
    if (!alpha_)
        alpha_ = std::make_shared<Alpha_internal>(admfIndex_);
    if (!metalness_)
        metalness_ = std::make_shared<Metalness_internal>(admfIndex_);
    if (!roughness_)
        roughness_ = std::make_shared<Roughness_internal>(admfIndex_);
    if (!specular_)
        specular_ = std::make_shared<Specular_internal>(admfIndex_);
    if (!glossiness_)
        glossiness_ = std::make_shared<Glossiness_internal>(admfIndex_);
    if (!anisotropy_)
        anisotropy_ = std::make_shared<Anisotropy_internal>(admfIndex_);
    if (!anisotropyRotation_)
        anisotropyRotation_ = std::make_shared<AnisotropyRotation_internal>(admfIndex_);
    if (!emissive_)
        emissive_ = std::make_shared<Emissive_internal>(admfIndex_);
    if (!ambientOcclusion_)
        ambientOcclusion_ = std::make_shared<AmbientOcclusion_internal>(admfIndex_);
    if (!height_)
        height_ = std::make_shared<Height_internal>(admfIndex_);
    if (!clearCoatNormal_)
        clearCoatNormal_ = std::make_shared<ClearCoatNormal_internal>(admfIndex_);
    if (!clearCoatRoughness_)
        clearCoatRoughness_ = std::make_shared<ClearCoatRoughness_internal>(admfIndex_);
    if (!clearCoatValue_)
        clearCoatValue_ = std::make_shared<ClearCoatValue_internal>(admfIndex_);
    if (!clearCoatColor_)
        clearCoatColor_ = std::make_shared<ClearCoatColor_internal>(admfIndex_);
    if (!sheenTint_)
        sheenTint_ = std::make_shared<SheenTint_internal>(admfIndex_);
    if (!sheenValue_)
        sheenValue_ = std::make_shared<SheenValue_internal>(admfIndex_);
    if (!sheenColor_)
        sheenColor_ = std::make_shared<SheenColor_internal>(admfIndex_);
    if (!sheenGloss_)
        sheenGloss_ = std::make_shared<SheenGloss_internal>(admfIndex_);
    if (!sheenRoughness_)
        sheenRoughness_ = std::make_shared<SheenRoughness_internal>(admfIndex_);
    if (!specularTint_)
        specularTint_ = std::make_shared<SpecularTint_internal>(admfIndex_);
    if (!subSurfaceColor_)
        subSurfaceColor_ = std::make_shared<SubSurfaceColor_internal>(admfIndex_);
    if (!subSurfaceRadius_)
        subSurfaceRadius_ = std::make_shared<SubSurfaceRadius_internal>(admfIndex_);
    if (!subSurfaceValue_)
        subSurfaceValue_ = std::make_shared<SubSurfaceValue_internal>(admfIndex_);
    if (!transmission_)
        transmission_ = std::make_shared<Transmission_internal>(admfIndex_);

    
    if (!transform_)
        transform_ = std::make_shared<LayerTransform_internal>(admfIndex_);

}
#ifdef ADMF_EDIT
void LayerBasic_internal::save(bson_t *doc)
{
    std::string emissiveKey = getNewKey("emissive");
    std::string basecolorKey = getNewKey("basecolor");
    std::string normalKey = getNewKey("normal");
    std::string alphaKey = getNewKey("alpha");
    std::string metalnessKey = getNewKey("metalness");
    std::string roughnessKey = getNewKey("roughness");
    std::string specularKey = getNewKey("specular");
    std::string glossinessKey = getNewKey("glossiness");
    std::string anisotropyKey = getNewKey("anisotropy");
    std::string anisotropyRotationKey = getNewKey("anisotropyRotation");
    std::string ambientOcclusionKey = getNewKey("ambientOcclusion");
    std::string heightKey = getNewKey("height");
    
    std::string clearCoatNormalKey = getNewKey("clearCoatNormal");
    std::string clearCoatRoughnessKey = getNewKey("clearCoatRoughness");
    std::string clearCoatValueKey = getNewKey("clearCoatValue");
    std::string clearCoatColorKey = getNewKey("clearCoatColor");
    std::string sheenTintKey = getNewKey("sheenTint");
    std::string sheenValueKey = getNewKey("sheenValue");
    std::string sheenColorKey = getNewKey("sheenColor");
    std::string sheenGlossKey = getNewKey("sheenGloss");
    std::string sheenRoughnessKey = getNewKey("sheenRoughness");
    std::string specularTintKey = getNewKey("specularTint");
    std::string subSurfaceColorKey = getNewKey("subSurfaceColor");
    std::string subSurfaceRadiusKey = getNewKey("subSurfaceRadius");
    std::string subSurfaceValueKey = getNewKey("subSurfaceValue");
    std::string transmissionKey = getNewKey("transmission");
    
    std::string transformKey = getNewKey("transform");


    ADMF_BSON_APPEND_DOCUMENT(doc, emissiveKey, emissive_);
    ADMF_BSON_APPEND_DOCUMENT(doc, basecolorKey, base_);
    ADMF_BSON_APPEND_DOCUMENT(doc, normalKey, normal_);
    ADMF_BSON_APPEND_DOCUMENT(doc, alphaKey, alpha_);
    ADMF_BSON_APPEND_DOCUMENT(doc, metalnessKey, metalness_);
    ADMF_BSON_APPEND_DOCUMENT(doc, roughnessKey, roughness_);
    ADMF_BSON_APPEND_DOCUMENT(doc, specularKey, specular_);
    ADMF_BSON_APPEND_DOCUMENT(doc, glossinessKey, glossiness_);
    ADMF_BSON_APPEND_DOCUMENT(doc, anisotropyKey, anisotropy_);
    ADMF_BSON_APPEND_DOCUMENT(doc, anisotropyRotationKey, anisotropyRotation_);
    ADMF_BSON_APPEND_DOCUMENT(doc, ambientOcclusionKey, ambientOcclusion_);
    ADMF_BSON_APPEND_DOCUMENT(doc, heightKey, height_);
    
    ADMF_BSON_APPEND_DOCUMENT(doc, clearCoatNormalKey, clearCoatNormal_);
    ADMF_BSON_APPEND_DOCUMENT(doc, clearCoatRoughnessKey, clearCoatRoughness_);
    ADMF_BSON_APPEND_DOCUMENT(doc, clearCoatValueKey, clearCoatValue_);
    ADMF_BSON_APPEND_DOCUMENT(doc, clearCoatColorKey, clearCoatColor_);
    ADMF_BSON_APPEND_DOCUMENT(doc, sheenTintKey, sheenTint_);
    ADMF_BSON_APPEND_DOCUMENT(doc, sheenValueKey, sheenValue_);
    ADMF_BSON_APPEND_DOCUMENT(doc, sheenColorKey, sheenColor_);
    ADMF_BSON_APPEND_DOCUMENT(doc, sheenGlossKey, sheenGloss_);
    ADMF_BSON_APPEND_DOCUMENT(doc, sheenRoughnessKey, sheenRoughness_);
    ADMF_BSON_APPEND_DOCUMENT(doc, specularTintKey, specularTint_);
    ADMF_BSON_APPEND_DOCUMENT(doc, subSurfaceColorKey, subSurfaceColor_);
    ADMF_BSON_APPEND_DOCUMENT(doc, subSurfaceRadiusKey, subSurfaceRadius_);
    ADMF_BSON_APPEND_DOCUMENT(doc, subSurfaceValueKey, subSurfaceValue_);
    ADMF_BSON_APPEND_DOCUMENT(doc, transmissionKey, transmission_);
    
    ADMF_BSON_APPEND_DOCUMENT(doc, transformKey, transform_);
}
#endif

BaseColor LayerBasic_internal::getBaseColor()
{
    return BaseColor(base_);
}
Normal LayerBasic_internal::getNormal()
{
    return Normal(normal_);
}
Alpha LayerBasic_internal::getAlpha()
{
    return Alpha(alpha_);
}
Metalness LayerBasic_internal::getMetalness()
{
    return Metalness(metalness_);
}
Roughness LayerBasic_internal::getRoughness()
{
    return Roughness(roughness_);
}

Specular LayerBasic_internal::getSpecular()
{
    return Specular(specular_);
}

Glossiness LayerBasic_internal::getGlossiness()
{
    return Glossiness(glossiness_);
}

Anisotropy LayerBasic_internal::getAnisotropy()
{
    return Anisotropy(anisotropy_);
}

AnisotropyRotation LayerBasic_internal::getAnisotropyRotation()
{
    return AnisotropyRotation(anisotropyRotation_);
}

AmbientOcclusion LayerBasic_internal::getAmbientOcclusion()
{
    return AmbientOcclusion(ambientOcclusion_);
}

Height LayerBasic_internal::getHeight()
{
    return Height(height_);
}

Emissive LayerBasic_internal::getEmissive()
{
    return Emissive(emissive_);
}

ClearCoatNormal LayerBasic_internal::getClearCoatNormal()
{
    return ClearCoatNormal(clearCoatNormal_);
}

ClearCoatRoughness LayerBasic_internal::getClearCoatRoughness()
{
    return ClearCoatRoughness(clearCoatRoughness_);
}

ClearCoatValue LayerBasic_internal::getClearCoatValue()
{
   return ClearCoatValue(clearCoatValue_);
}

ClearCoatColor LayerBasic_internal::getClearCoatColor()
{
    return ClearCoatColor(clearCoatColor_);
}

SheenTint LayerBasic_internal::getSheenTint()
{
    return SheenTint(sheenTint_);
}

SheenValue LayerBasic_internal::getSheenValue()
{
    return SheenValue(sheenValue_);
}

SheenColor LayerBasic_internal::getSheenColor()
{
    return SheenColor(sheenColor_);
}

SheenGloss LayerBasic_internal::getSheenGloss()
{
    return SheenGloss(sheenGloss_);
}

SheenRoughness LayerBasic_internal::getSheenRoughness()
{
    return SheenRoughness(sheenRoughness_);
}

SpecularTint LayerBasic_internal::getSpecularTint()
{
    return SpecularTint(specularTint_);
}

SubSurfaceColor LayerBasic_internal::getSubSurfaceColor()
{
    return SubSurfaceColor(subSurfaceColor_);
}

SubSurfaceRadius LayerBasic_internal::getSubSurfaceRadius()
{
    return SubSurfaceRadius(subSurfaceRadius_);
}

SubSurfaceValue LayerBasic_internal::getSubSurfaceValue()
{
    return SubSurfaceValue(subSurfaceValue_);
}

Transmission LayerBasic_internal::getTransmission()
{
    return Transmission(transmission_);
}

LayerTransform LayerBasic_internal::getTransform()
{
    return LayerTransform(transform_);
}

void LayerTransform_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;

    bson_iter_t child;
    if (!bson_iter_recurse(iter, &child))
        return;

    std::string wrappingKey = getNewKey("wrapping");
    std::string offsetKey = getNewKey("offset");
    std::string scaleKey = getNewKey("scale");
    std::string rotationKey = getNewKey("rotation");

    while (bson_iter_next(&child))
    {
        std::string keyName = bson_iter_key(&child);
        assert(bson_iter_value(&child) != nullptr);
        //printf("Found element key: \"%s\"\n", keyName.c_str());
        if (keyName == wrappingKey)
        {
            wrapping_ = std::make_shared<String_internal>(admfIndex_, &child);
        }
        else if (keyName == offsetKey)
        {
            offset_ = std::make_shared<Vec2_internal>(admfIndex_, &child);
        }
        else if (keyName == scaleKey)
        {
            scale_ = std::make_shared<Vec2_internal>(admfIndex_, &child);
        }
        else if (keyName == rotationKey)
        {
            rotation_ = (ADMF_FLOAT)bson_iter_as_double(&child);
        }
    }
}

void LayerTransform_internal::initMissed()
{
    if (!wrapping_)
        wrapping_ = std::make_shared<String_internal>(admfIndex_);
    if (!offset_)
        offset_ = std::make_shared<Vec2_internal>(admfIndex_);
    if (!scale_)
    {
        scale_ = std::make_shared<Vec2_internal>(admfIndex_);
        scale_->x_ = 1.0;
        scale_->y_ = 1.0;
    }

 
}
#ifdef ADMF_EDIT
void LayerTransform_internal::save(bson_t *doc)
{
    std::string wrappingKey = getNewKey("wrapping");
    std::string offsetKey = getNewKey("offset");
    std::string scaleKey = getNewKey("scale");
    std::string rotationKey = getNewKey("rotation");

    ADMF_BSON_APPEND_STRING(doc, wrappingKey, wrapping_);
    ADMF_BSON_APPEND_DOCUMENT(doc, offsetKey, offset_);
    ADMF_BSON_APPEND_DOCUMENT(doc, scaleKey, scale_);
    ADMF_BSON_APPEND_DOUBLE(doc, rotationKey, rotation_);
}
#endif

String LayerTransform_internal::getWrapping() //repeat
{
    return String(wrapping_);
}
Vec2 LayerTransform_internal::getOffset()
{
    return Vec2(offset_);
}

Vec2 LayerTransform_internal::getScale()
{
    return Vec2(scale_);
}

ADMF_FLOAT LayerTransform_internal::getRotation()
{
    return rotation_;
}

//
//  admf_materiallayer_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_materiallayer_internal_hpp
#define admf_materiallayer_internal_hpp

#include "admf_internal.h"

#include <memory>
namespace admf_internal {
    
    
    class String_internal;
    class BinaryData_internal;
    class LayerBasic_internal;
    class LayerSpec_internal;
    class LayerRefraction_internal;
    class BaseColor_internal;
    class Normal_internal;
    class Metalness_internal;
    class Roughness_internal;
    class Specular_internal;
    class Glossiness_internal;
    class Anisotropy_internal;
    class AnisotropyRotation_internal;
    class Emissive_internal;
    class LayerTransform_internal;
    
    class MaterialLayer_internal : public admf::MaterialLayer_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(MaterialLayer_internal);
    public:
        virtual admf::String getType() override; //"Fabric",
        virtual admf::String getShader() override; //着色类型
        
        virtual admf::BinaryData getPreview() override; //content of "/preview.png"
        
        virtual admf::LayerBasic getBasic() override;
        virtual admf::LayerSpec getSpec() override;

        virtual admf::ADMF_BYTE isEnabled() override;
        
        virtual admf::String getName() override;
#ifdef ADMF_EDIT
        virtual void setEnabled(admf::ADMF_BYTE enabled) override;
#endif
    private:
        std::shared_ptr<String_internal> type_;
        std::shared_ptr<String_internal> shader_;
        std::shared_ptr<BinaryData_internal> preview_;
        std::shared_ptr<LayerBasic_internal> basic_;
        std::shared_ptr<LayerSpec_internal> spec_;
        admf::ADMF_BYTE enabled_ = 0;
        admf::String name_;
    };
    
    
    
    class LayerSpec_internal : public admf::LayerSpec_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(LayerSpec_internal);
    public:
        virtual admf::LayerRefraction getRefraction() override;
        
    private:
        std::shared_ptr<LayerRefraction_internal> refraction_;
        

    };
    
    class LayerRefraction_internal : public admf::LayerRefraction_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(LayerRefraction_internal);
    public:
        virtual admf::ColorRGB getColor() override {return color_;};
        virtual admf::ADMF_FLOAT getGlossiness() override {return glossiness_;};
        virtual admf::Texture getTexture() override {return texture_;};
        virtual admf::TEX_TYPE getTextureType() override {return admf::TEX_TYPE_IOR;};
        virtual admf::ADMF_FLOAT getValue() override {return value_;}

    private:
        std::shared_ptr<ColorRGB_internal> color_;
        admf::ADMF_FLOAT glossiness_ = 1;
        std::shared_ptr<Texture_internal> texture_;
        admf::ADMF_FLOAT value_ = 1.5f;
        
#ifdef ADMF_EDIT
    public:
        virtual void setGlossiness(admf::ADMF_FLOAT glossiness) override {glossiness_ = glossiness;};
        virtual void setValue(admf::ADMF_FLOAT value) override {value_ = value;};
#endif
    };
    
    class LayerBasic_internal : public admf::LayerBasic_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(LayerBasic_internal);
    public:
        virtual admf::Emissive getEmissive() override;
        virtual admf::BaseColor getBaseColor() override;
        virtual admf::Normal getNormal() override;
        virtual admf::Alpha getAlpha() override;
        virtual admf::Metalness getMetalness() override;
        virtual admf::Roughness getRoughness() override;
        virtual admf::Specular getSpecular() override;
        virtual admf::Glossiness getGlossiness() override;
        virtual admf::Anisotropy getAnisotropy() override;
        virtual admf::AnisotropyRotation getAnisotropyRotation() override;
        virtual admf::AmbientOcclusion getAmbientOcclusion() override;
        virtual admf::Height getHeight() override;
        
        virtual admf::ClearCoatNormal getClearCoatNormal() override;
        virtual admf::ClearCoatRoughness getClearCoatRoughness() override;
        virtual admf::ClearCoatValue getClearCoatValue() override;
        virtual admf::ClearCoatColor getClearCoatColor() override;
        virtual admf::SheenTint getSheenTint() override;
        virtual admf::SheenValue getSheenValue() override;
        virtual admf::SheenColor getSheenColor() override;
        virtual admf::SheenGloss getSheenGloss() override;
        virtual admf::SheenRoughness getSheenRoughness() override;
        virtual admf::SpecularTint getSpecularTint() override;
        virtual admf::SubSurfaceColor getSubSurfaceColor() override;
        virtual admf::SubSurfaceRadius getSubSurfaceRadius() override;
        virtual admf::SubSurfaceValue getSubSurfaceValue() override;
        virtual admf::Transmission getTransmission() override;
        
        virtual admf::LayerTransform getTransform() override;
        
    private:

        std::shared_ptr<BaseColor_internal> base_;
        std::shared_ptr<Normal_internal> normal_;
        std::shared_ptr<Alpha_internal> alpha_;
        std::shared_ptr<Metalness_internal> metalness_;
        std::shared_ptr<Roughness_internal> roughness_;
        std::shared_ptr<Specular_internal> specular_;
        std::shared_ptr<Glossiness_internal> glossiness_;
        std::shared_ptr<Anisotropy_internal> anisotropy_;
        std::shared_ptr<AnisotropyRotation_internal> anisotropyRotation_;
        std::shared_ptr<Emissive_internal> emissive_;
        std::shared_ptr<AmbientOcclusion_internal> ambientOcclusion_;
        std::shared_ptr<Height_internal> height_;
        
        std::shared_ptr<ClearCoatNormal_internal> clearCoatNormal_;
        std::shared_ptr<ClearCoatRoughness_internal> clearCoatRoughness_;
        std::shared_ptr<ClearCoatValue_internal> clearCoatValue_;
        std::shared_ptr<ClearCoatColor_internal> clearCoatColor_;
        std::shared_ptr<SheenTint_internal> sheenTint_;
        std::shared_ptr<SheenValue_internal> sheenValue_;
        std::shared_ptr<SheenColor_internal> sheenColor_;
        std::shared_ptr<SheenGloss_internal> sheenGloss_;
        std::shared_ptr<SheenRoughness_internal> sheenRoughness_;
        std::shared_ptr<SpecularTint_internal> specularTint_;
        std::shared_ptr<SubSurfaceColor_internal> subSurfaceColor_;
        std::shared_ptr<SubSurfaceRadius_internal> subSurfaceRadius_;
        std::shared_ptr<SubSurfaceValue_internal> subSurfaceValue_;
        std::shared_ptr<Transmission_internal> transmission_;
        
        
        std::shared_ptr<LayerTransform_internal> transform_;
        

    };
    
    
    class LayerTransform_internal : public admf::LayerTransform_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(LayerTransform_internal);
    public:
        virtual admf::String getWrapping() override; //repeat
        virtual admf::Vec2 getOffset() override;
        virtual admf::Vec2 getScale() override;
        virtual admf::ADMF_FLOAT getRotation() override;
    private:
        std::shared_ptr<String_internal> wrapping_;
        std::shared_ptr<Vec2_internal> offset_;
        std::shared_ptr<Vec2_internal> scale_;
        admf::ADMF_FLOAT rotation_ = 0;

#ifdef ADMF_EDIT
    public:
        virtual void setRotation(admf::ADMF_FLOAT rotation) override {rotation_ = rotation;};
#endif
    };
    
    
    
    
}
#endif /* admf_materiallayer_internal_hpp */

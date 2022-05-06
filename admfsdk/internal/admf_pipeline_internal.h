//
//  admf_custom_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_custom_internal_hpp
#define admf_custom_internal_hpp

#include "admf_internal.h"
#include <memory>
namespace admf_internal
{

    class BaseColorData_internal;
    class Texture_internal;
    class ColorRGB_internal;
    class BaseColorChangeColorData_internal;

    class BaseColor_internal : public admf::BaseColor_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColor_internal);

    public:
        virtual admf::BaseColorData getData() override;
        virtual admf::Texture getTexture() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_BASE; };
        //virtual admf::BaseColorChangeColorData getChangeColorData() override;
    private:
        std::shared_ptr<BaseColorData_internal> data_;
        std::shared_ptr<Texture_internal> texture_;
        //std::shared_ptr<BaseColorChangeColorData_internal> changeColor_;
    };

    class Specular_internal : public admf::Specular_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Specular_internal);

    public:
        virtual admf::Texture getTexture() override;
        virtual admf::ColorRGB getColor() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_SPECULAR; };

        virtual admf::ADMF_FLOAT getValue() override { return value_; };
#ifdef ADMF_EDIT
        virtual void setValue(admf::ADMF_FLOAT value) override
        {
            value_ = value;
        };
#endif
    private:
        std::shared_ptr<Texture_internal> texture_;
        std::shared_ptr<ColorRGB_internal> color_;
        admf::ADMF_FLOAT value_ = 0;
    };

    class Emissive_internal : public admf::Emissive_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Emissive_internal);
        
        friend class LayerBasic_internal;

    public:
        virtual admf::Texture getTexture() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_EMISSIVE; };
        virtual admf::ColorRGB getColor() override;
        virtual admf::ADMF_FLOAT getValue() override { return value_; };
#ifdef ADMF_EDIT
        virtual void setValue(admf::ADMF_FLOAT value) override { value_ = value;}
#else
    private:
        void setValue(admf::ADMF_FLOAT value)  { value_ = value;}
#endif
        


    private:
        std::shared_ptr<Texture_internal> texture_;
        std::shared_ptr<ColorRGB_internal> color_;
        admf::ADMF_FLOAT value_ = 0.0;
    };

    /// Height
    class Height_internal : public admf::Height_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Height_internal);

    public:
        virtual admf::Texture getTexture() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_HEIGHT; };
        virtual admf::ADMF_FLOAT getValue() override { return value_; };
        virtual admf::ADMF_FLOAT getLevel() override { return level_; };

#ifdef ADMF_EDIT
    public:
        virtual void setValue(admf::ADMF_FLOAT value) override { value_ = value; };
        virtual void setLevel(admf::ADMF_FLOAT level) override { level_ = level; };
#endif

    private:
        std::shared_ptr<Texture_internal> texture_;
        admf::ADMF_FLOAT value_ = 1.0;
        admf::ADMF_FLOAT level_ = 1.0;
    };

    class SubSurfaceColor_internal : public admf::SubSurfaceColor_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(SubSurfaceColor_internal);

    public:
        virtual admf::Texture getTexture() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_SUBSURFACE_COLOR; };
        virtual admf::ColorRGB getColor() override;

    private:
        std::shared_ptr<Texture_internal> texture_;
        std::shared_ptr<ColorRGB_internal> color_;
    };

	class SheenColor_internal : public admf::SheenColor_, public Base_internal
	{
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(SheenColor_internal);
	public:
        virtual admf::Texture getTexture() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_SHEEN_COLOR; };
        virtual admf::ColorRGB getColor() override;
    private:
        std::shared_ptr<Texture_internal> texture_;
        std::shared_ptr<ColorRGB_internal> color_;
	};

    class ClearCoatColor_internal : public admf::ClearCoatColor_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(ClearCoatColor_internal);

    public:
        virtual admf::Texture getTexture() override;
        virtual admf::TEX_TYPE getTextureType() override { return admf::TEX_TYPE_CLEARCOAT_COLOR; };
        virtual admf::ColorRGB getColor() override;

    private:
        std::shared_ptr<Texture_internal> texture_;
        std::shared_ptr<ColorRGB_internal> color_;
    };
#ifdef ADMF_EDIT
#define SetValue_Declaration() \
    virtual void setValue(admf::ADMF_FLOAT value) override { value_ = value; };
#else
#define SetValue_Declaration()
#endif

#define TextureAndValueContainer_Internal_Declaration_(classname, basename, texType, defaultValue) \
    class classname : public admf::basename, public Base_internal                                  \
    {                                                                                              \
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(classname);                                                \
                                                                                                   \
    public:                                                                                        \
        virtual admf::Texture getTexture() override;                                               \
        virtual admf::ADMF_FLOAT getValue() override;                                              \
        virtual admf::TEX_TYPE getTextureType() override { return admf::texType; };                \
                                                                                                   \
    private:                                                                                       \
        std::shared_ptr<Texture_internal> texture_;                                                \
        admf::ADMF_FLOAT value_ = defaultValue;                                                    \
                                                                                                   \
    public:                                                                                        \
        SetValue_Declaration()                                                                     \
    }

#define TextureAndValueContainer_Internal_Declaration(basename, texType, defaultValue) \
    TextureAndValueContainer_Internal_Declaration_(basename##internal, basename, texType, defaultValue)


#define TextureContainer_Internal_Declaration_(classname, basename, texType)        \
    class classname : public admf::basename, public Base_internal                   \
    {                                                                               \
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(classname);                                 \
                                                                                    \
    public:                                                                         \
        virtual admf::Texture getTexture() override;                                \
        virtual admf::TEX_TYPE getTextureType() override { return admf::texType; }; \
                                                                                    \
    private:                                                                        \
        std::shared_ptr<Texture_internal> texture_;                                 \
    }

#define TextureContainer_Internal_Declaration(basename, texType) \
    TextureContainer_Internal_Declaration_(basename##internal, basename, texType)

    TextureAndValueContainer_Internal_Declaration(Normal_, TEX_TYPE_NORMAL, 1.2f);
    TextureAndValueContainer_Internal_Declaration(Alpha_, TEX_TYPE_ALPHA, 1.0f);
    TextureAndValueContainer_Internal_Declaration(Metalness_, TEX_TYPE_METALNESS, 1.0f);
    TextureAndValueContainer_Internal_Declaration(Roughness_, TEX_TYPE_ROUGHNESS, 1.0f);
    TextureAndValueContainer_Internal_Declaration(Glossiness_, TEX_TYPE_GLOSSINESS, 1.0f);
    TextureAndValueContainer_Internal_Declaration(Anisotropy_, TEX_TYPE_ANISOTROPY, 0.0f);
    TextureAndValueContainer_Internal_Declaration(AnisotropyRotation_, TEX_TYPE_ANISOTROPY_ROTATION, 0.0f);
    TextureContainer_Internal_Declaration(AmbientOcclusion_, TEX_TYPE_AO);
    TextureContainer_Internal_Declaration(ClearCoatNormal_, TEX_TYPE_CLEARCOAT_NORMAL);
    TextureAndValueContainer_Internal_Declaration(ClearCoatRoughness_, TEX_TYPE_CLEARCOAT_ROUGHNESS, 0.0f);
    TextureAndValueContainer_Internal_Declaration(ClearCoatValue_, TEX_TYPE_CLEARCOAT_VALUE, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SheenTint_, TEX_TYPE_SHEEN_TINT, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SheenValue_, TEX_TYPE_SHEEN_VALUE, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SheenGloss_, TEX_TYPE_SHEEN_GLOSS, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SheenRoughness_, TEX_TYPE_SHEEN_ROUGHNESS, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SpecularTint_, TEX_TYPE_SPECULAR_TINT, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SubSurfaceRadius_, TEX_TYPE_SUBSURFACE_RADIUS, 0.0f);
    TextureAndValueContainer_Internal_Declaration(SubSurfaceValue_, TEX_TYPE_SUBSURFACE_VALUE, 0.0f);
    TextureAndValueContainer_Internal_Declaration(Transmission_, TEX_TYPE_TRANSMISSION, 0.0f);
}
#endif /* admf_custom_internal_hpp */

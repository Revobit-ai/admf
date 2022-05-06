//
//  admf_material.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_pipeline_h
#define admf_pipeline_h

namespace admf
{

    /// 贴图集合类
    class TextureContainer
    {
    public:
        ///     获取贴图
        ///    @return    返回贴图
        virtual Texture getTexture() = 0;
        ///     获取贴图类型
        ///    @return    返回贴图类型
        virtual TEX_TYPE getTextureType() = 0;
    };

    /// 贴图集合和值集合类
    class TextureAndValueContainer : public TextureContainer
    {
    public:
        ///     获取贴图值
        ///    @return    返回贴图值
        virtual ADMF_FLOAT getValue() = 0;
#ifdef ADMF_EDIT
    public:
        ///     设置贴图值
        ///     @param value 贴图值
        virtual void setValue(ADMF_FLOAT value) = 0;
#endif
    };


    class BaseColor_ : public TextureContainer
    {
    public:

        virtual BaseColorData getData() = 0;
        //virtual BaseColorChangeColorData getChangeColorData() = 0;

    };

    /// 法线贴图类
    class Normal_ : public TextureAndValueContainer
    {
    };
    /// Alpha贴图类
    class Alpha_ : public TextureAndValueContainer
    {
    };
    /// 金属贴图合类
    class Metalness_ : public TextureAndValueContainer
    {
    };
    /// 粗糙贴图类
    class Roughness_ : public TextureAndValueContainer
    {
    };
    /// 高光贴图类
    class Specular_ : public TextureContainer
    {
    public:
        ///     获取颜色
        ///    @return    返回颜色
        virtual ColorRGB getColor() = 0;
        virtual admf::ADMF_FLOAT getValue() = 0;
#ifdef ADMF_EDIT
        virtual void setValue(admf::ADMF_FLOAT value) = 0;
#endif
    };
    /// 光泽贴图类
    class Glossiness_ : public TextureAndValueContainer
    {
    };

    /// 各向异性
    class Anisotropy_ : public TextureAndValueContainer
    {
    };

    /// 各向异性
    class AnisotropyRotation_ : public TextureAndValueContainer
    {
    };

    /// 自发光
    class Emissive_ : public TextureContainer
    {

    public:
        virtual admf::ColorRGB getColor() = 0;
        virtual admf::ADMF_FLOAT getValue() = 0;
#ifdef ADMF_EDIT
        virtual void setValue(admf::ADMF_FLOAT value) = 0;
#endif
    };


    /// AO
    class AmbientOcclusion_ : public TextureContainer
    {

    };

    /// Height
    class Height_ : public TextureAndValueContainer
    {
    public:
        virtual admf::ADMF_FLOAT getLevel() = 0;
#ifdef ADMF_EDIT
    public:
        virtual void setLevel(admf::ADMF_FLOAT level) = 0;
#endif
    };


    class ClearCoatNormal_ : public TextureContainer
    {
    };

    class ClearCoatRoughness_ : public TextureAndValueContainer
    {
    };

    class ClearCoatValue_ : public TextureAndValueContainer
    {
    };

    class ClearCoatColor_ : public TextureContainer
    {
    public:
        virtual admf::ColorRGB getColor() = 0;
    };
    
    // note: SheenTint_ and SheenValue_ is added refer to u3m format v1.1, and is used only in u3m_to_admf project
	// we do not use these two in our render system(RHI)
	// Instead we use a V-Ray style sheen implementation. We add 2 classes: SheenColor_ and SheenGloss_ to contain these infos
	// There's no conversion between u3m style sheen and V-Ray style sheen for now.
	// The only reason SheenTint_ and SheenValue_ exists is for data integrity. Developers working with RHI should not use these 2 values.
	// xd 2022/01/17
    class SheenTint_: public TextureAndValueContainer
    {
    };
    
    class SheenValue_: public TextureAndValueContainer
    {
    };

	class SheenColor_ : public TextureContainer
	{
    public:
        virtual admf::ColorRGB getColor() = 0;
	};

	class SheenGloss_ : public TextureAndValueContainer
	{
		
	};

    class SheenRoughness_ : public TextureAndValueContainer
    {

    };
	
    class SpecularTint_: public TextureAndValueContainer
    {
    };
    

    
    class SubSurfaceColor_: public TextureContainer
    {
    public:
        virtual ColorRGB getColor() = 0;
    };
    
    class SubSurfaceRadius_: public TextureAndValueContainer
    {
    };
    
    class SubSurfaceValue_: public TextureAndValueContainer
    {
    };
    
    class Transmission_: public TextureAndValueContainer
    {
    };

}
#endif /* admf_material_h */

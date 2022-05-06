//
//  admf_materiallayer.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_materiallayer_h
#define admf_materiallayer_h

#include "admf_pipeline.h"
namespace admf
{

    /// 材质层类
    class MaterialLayer_
    {
    public:
        ///   获取材质采集类型
        ///    @return    返回材质采集类型
        virtual String getType() = 0;   //"Fabric",
                                        ///   获取材质Shader
                                        ///    @return    返回材质Shader
        virtual String getShader() = 0; //着色类型

        ///   获取材质预览图
        ///    @return    返回材质预览图
        virtual BinaryData getPreview() = 0; //content of "/preview.png"

        ///   获取材质基础数据(各贴图)
        ///    @return    返回材质基础数据(各贴图)
        virtual LayerBasic getBasic() = 0;
        ///   获取材质Spec
        ///    @return    返回材质Spec
        virtual LayerSpec getSpec() = 0;

		///   是否启用
		///    @return    是否启用
        virtual ADMF_BYTE isEnabled() = 0;
        
        ///   Layer名称
        ///    @return    Layer名称
        virtual String getName() = 0;
        

#ifdef ADMF_EDIT
		///   设置是否启用
		virtual void setEnabled(admf::ADMF_BYTE enabled) = 0;
#endif
    };

    /// 材质层数据类
    class LayerSpec_
    {
    public:
        ///     获取材质折射
        ///    @return    返回材质折射
        ///    @see LayerRefraction
        virtual LayerRefraction getRefraction() = 0;
    };

    /// 材质层折射率类
    class LayerRefraction_ : public TextureAndValueContainer
    {
    public:
        ///     获取材质颜色
        ///    @return    返回材质颜色
        ///    @see ColorRGB
        virtual ColorRGB getColor() = 0;
        ///     获取材质光泽度
        ///    @return    返回材质光泽度
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getGlossiness() = 0;

#ifdef ADMF_EDIT
    public:
        /// 设置光泽度
        /// @param glossiness 光泽度值
        virtual void setGlossiness(ADMF_FLOAT glossiness) = 0;
#endif
    };

    /// 材质层基本数据类
    class LayerBasic_
    {
    public:
        ///     获取反射率
        ///    @return    返回反射率
        virtual Emissive getEmissive() = 0;
        ///     获取DiffUse信息
        ///    @return    返回DiffUse信息
        ///    @see BaseColor
        virtual BaseColor getBaseColor() = 0;
        ///     获取法线贴图
        ///    @return    返回法线贴图
        ///    @see Normal
        virtual Normal getNormal() = 0;
        ///     获取透明贴图
        ///    @return    返回透明贴图
        ///    @see Alpha
        virtual Alpha getAlpha() = 0;
        ///     获取金属贴图
        ///    @return    返回金属贴图
        ///    @see Metalness
        virtual Metalness getMetalness() = 0;
        ///     获取粗糙贴图
        ///    @return    返回粗糙贴图
        ///    @see Roughness
        virtual Roughness getRoughness() = 0;
        ///     获取反射贴图
        ///    @return    返回反射贴图
        ///    @see Specular
        virtual Specular getSpecular() = 0;
        ///     获取光泽贴图
        ///    @return    返回光泽贴图
        ///    @see Glossiness
        virtual Glossiness getGlossiness() = 0;
        ///     获取各向异性贴图
        ///    @return    返回各向异性贴图
        ///    @see Glossiness
        virtual Anisotropy getAnisotropy() = 0;
        ///     获取各向异性贴图
        ///    @return    返回各向异性贴图
        ///    @see Glossiness
        virtual AnisotropyRotation getAnisotropyRotation() = 0;
        ///     获取AO贴图
        ///    @return    返回AO贴图
        ///    @see AmbientOcclusion
        virtual AmbientOcclusion getAmbientOcclusion() = 0;
        ///     获取Height贴图
        ///    @return    返回Height贴图
        ///    @see Height
        virtual Height getHeight() = 0;
        
        ///     获取Transform
        ///    @return    返回Transform
        ///    @see LayerTransform
        virtual LayerTransform getTransform() = 0;

        
        
        virtual ClearCoatNormal getClearCoatNormal() = 0;
        
        virtual ClearCoatRoughness getClearCoatRoughness() = 0;
        
        virtual ClearCoatValue getClearCoatValue() = 0;

        virtual ClearCoatColor getClearCoatColor() = 0;
        
        virtual SheenTint getSheenTint() = 0;
        
        virtual SheenValue getSheenValue() = 0;

        virtual SheenColor getSheenColor() = 0;

        virtual SheenGloss getSheenGloss() = 0;
        
        virtual SheenRoughness getSheenRoughness() = 0;

        virtual SpecularTint getSpecularTint() = 0;
        
        virtual SubSurfaceColor getSubSurfaceColor() = 0;
        
        virtual SubSurfaceRadius getSubSurfaceRadius() = 0;
        
        virtual SubSurfaceValue getSubSurfaceValue() = 0;
        
        virtual Transmission getTransmission() = 0;
        
        

    };

    /// 材质层Transform类
    class LayerTransform_
    {
    public:
        ///     获取Wrapping
        ///    @return    返回Wrapping
        virtual String getWrapping() = 0; //repeat
        ///     获取偏移量
        ///    @return    返回偏移量
        virtual Vec2 getOffset() = 0;
        ///     获取缩放量
        ///    @return    返回缩放量
        virtual Vec2 getScale() = 0;
        ///     获取旋转量
        ///    @return    返回旋转量
        virtual ADMF_FLOAT getRotation() = 0;
#ifdef ADMF_EDIT
    public:
        ///    设置旋转量
        ///    @param rotation 旋转量
        virtual void setRotation(ADMF_FLOAT rotation) = 0;
#endif
    };

}
#endif /* admf_materiallayer_h */

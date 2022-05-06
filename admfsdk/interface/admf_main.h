//
//  admf_main.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_main_h
#define admf_main_h

namespace admf
{

    extern ADMF loadFromFile(const char *filePath, ADMF_RESULT &result);
#ifdef ADMF_EDIT
    extern ADMF createADMF();
#endif
    /// ADMF管理类
    class ADMF_
    {
    public:
        /// 设置描述文件和二进制文件限制
        /// @param descriptionSizeLimit 描述文件的限制大小
        /// @param binarySizeLimit 二进制文件的限制大小
        static void setSizeLimit(ADMF_UINT descriptionSizeLimit, ADMF_UINT binarySizeLimit);
        /// 获取描述文件和二进制文件限制
        /// @param descriptionSizeLimit 描述文件的限制大小
        /// @param binarySizeLimit 二进制文件的限制大小
        static void getSizeLimit(ADMF_UINT &descriptionSizeLimit, ADMF_UINT &binarySizeLimit);

        /// 获取版本号
        virtual StringReadOnly getSchema() = 0; //"1.0"
        
        //上次编辑时的SDK版本
        virtual  StringReadOnly getSDKVersion() = 0; //"1.1"
        
        ///   获取ADMF材质数据
        ///    @return    返回材质数据
        ///    @see Material
        virtual Material getMaterial() = 0;
        ///   获取ADMF物理数据
        ///    @return    返回物理数据
        ///    @see Physics
        virtual Physics getPhysics() = 0;
        ///   获取ADMF用户自定义数据
        ///    @return    返回用户自定义数据
        ///    @see Custom
        virtual Custom getCustom() = 0;
        ///   获取ADMF几何数据
        ///    @return    返回几何数据
        ///    @see Geometry
        virtual Geometry getGeometry() = 0;
        

#ifdef ADMF_EDIT
    public:
        /// 保存文件
        /// @param filePath 保存路径
        virtual ADMF_RESULT saveToFile(const char *filePath) = 0;
#endif
    };

}
#endif /* admf_structures_h */

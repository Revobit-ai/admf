//
//  admf_base.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_base_h
#define admf_base_h

#define ADMF_MAX_STR_LEN (256)

#define ADMF_SDK_VERSION "1.1"  //每次内容调整后升级, 例如增加了新的贴图
#define ADMF_SCHEMA "1.0" //文件格式，例如json和二进制的格式排版， 如果只是内容增加

#include <memory>
#include <string>
namespace admf
{

    typedef int ADMF_INT;
    typedef unsigned int ADMF_UINT;
    typedef float ADMF_FLOAT;
    typedef double ADMF_DOUBLE;
    typedef unsigned long long ADMF_DATE;
    typedef unsigned char ADMF_BYTE;
    typedef char ADMF_CHAR;

    /// SDK的错误类型
    enum ADMF_RESULT
    {
        ADMF_SUCCESS = 0,                  ///< 成功
        ADMF_FILE_NOT_EXIST = 1,           ///< 文件不存在
        ADMF_DESC_NOT_FOUND = 2,           ///< 描述信息未找到
        ADMF_DESC_SIZE_ERROR = 3,          ///< 描述信息大小错误
        ADMF_GET_DESC_CONTENT_FAILED = 4,  ///< 获取描述信息失败
        ADMF_PARAM_ERROR = 5,              ///< 参数错误
        ADMF_DESC_FORMAT_ERROR = 6,        ///< 描述信息格式错误
        ADMF_BINARY_NOT_FOUND = 7,         ///< 二进制内容未找到
        ADMF_WRITE_TO_FILE_FAILED = 8,     ///< 写入文件失败
        ADMF_BINARY_SIZE_EXCEED_LIMIT = 9, ///< 二进制大小超过限制
        ADMF_ALLOC_FAILED = 10,            ///< 内存分配失败
        ADMF_OPEN_FILE_FAILED = 11,        ///< 打开文件失败
        ADMF_UNKNOWN_ERROR = 100,          ///< 未知错误
        ADMF_INTERNAL_ERROR = 101,         ///< 内部错误
    };

    /// 这是一个材质所包含所有贴图枚举
    enum TEX_TYPE
    {
        TEX_TYPE_BASE = 0,   ///<漫反射贴图
        TEX_TYPE_NORMAL,     ///<法线贴图
        TEX_TYPE_ALPHA,      ///<Alpha贴图
        TEX_TYPE_METALNESS,  ///<金属贴图
        TEX_TYPE_ROUGHNESS,  ///<粗糙贴图
        TEX_TYPE_SPECULAR,   ///<高光贴图
        TEX_TYPE_GLOSSINESS, ///<光滑度贴图
        TEX_TYPE_ANISOTROPY, ///<各向异性贴图
        TEX_TYPE_ANISOTROPY_ROTATION, ///<各向异性贴图
        TEX_TYPE_EMISSIVE, ///<自发光贴图
        TEX_TYPE_AO, ///<AO贴图
        TEX_TYPE_HEIGHT, ///<HEIGHT
        TEX_TYPE_CLEARCOAT_NORMAL,
        TEX_TYPE_CLEARCOAT_ROUGHNESS,
        TEX_TYPE_CLEARCOAT_VALUE,
        TEX_TYPE_CLEARCOAT_COLOR,
        TEX_TYPE_SHEEN_TINT,
        TEX_TYPE_SHEEN_VALUE,
        TEX_TYPE_SHEEN_COLOR,
        TEX_TYPE_SHEEN_GLOSS,
        TEX_TYPE_SHEEN_ROUGHNESS,
        TEX_TYPE_SPECULAR_TINT,
        TEX_TYPE_SUBSURFACE_COLOR,
        TEX_TYPE_SUBSURFACE_RADIUS,
        TEX_TYPE_SUBSURFACE_VALUE,
        TEX_TYPE_TRANSMISSION,
        TEX_TYPE_IOR,
        TEX_TYPE_UNKNOWN     ///<未知类型
    };

#define ADMF_DEF(x) \
    class x##_;    \
    typedef std::shared_ptr<x##_> x;

    template <typename T>
    class Array_;
    template <typename T>
    using Array = std::shared_ptr<Array_<T>>;

    ADMF_DEF(StringReadOnly);
    ADMF_DEF(String);
    ADMF_DEF(Vec2);
    ADMF_DEF(ColorRGB);
    ADMF_DEF(BinaryData);
    ADMF_DEF(Texture);

    ADMF_DEF(ADMF);
    ADMF_DEF(Physics);
    ADMF_DEF(PhysicsComposition);
    ADMF_DEF(Custom);
    ADMF_DEF(Geometry);
    ADMF_DEF(Material);
    ADMF_DEF(MaterialDevice);
    ADMF_DEF(MaterialLayer);
    ADMF_DEF(MaterialMetaData);

    ADMF_DEF(LayerBasic);
    ADMF_DEF(LayerTransform);
    ADMF_DEF(LayerSpec);
    ADMF_DEF(LayerRefraction);

    ADMF_DEF(BaseColorData);
    ADMF_DEF(BaseColorDataSolid);
    ADMF_DEF(BaseColorDataSolidBlock);
    ADMF_DEF(BaseColorDataMulti);
    ADMF_DEF(BaseColorDataMultiBlock);
    ADMF_DEF(BaseColorDataMultiBlockMask);
    //ADMF_DEF(BaseColorChangeColorData);
    //放入Custom字段

    ADMF_DEF(BaseColor);
    ADMF_DEF(Normal);
    ADMF_DEF(Alpha);
    ADMF_DEF(Metalness);
    ADMF_DEF(Roughness);
    ADMF_DEF(Specular);
    ADMF_DEF(Glossiness);
    ADMF_DEF(Anisotropy);
    ADMF_DEF(AnisotropyRotation);
    ADMF_DEF(Emissive);
    ADMF_DEF(AmbientOcclusion);
    ADMF_DEF(Height);
    ADMF_DEF(ClearCoatNormal);
    
    
    ADMF_DEF(ClearCoatRoughness);
    ADMF_DEF(ClearCoatValue);
    ADMF_DEF(ClearCoatColor);
    ADMF_DEF(SheenTint);
    ADMF_DEF(SheenValue);
    ADMF_DEF(SheenColor);
    ADMF_DEF(SheenGloss);
    ADMF_DEF(SheenRoughness);
    ADMF_DEF(SpecularTint);
    ADMF_DEF(SubSurfaceColor);
    ADMF_DEF(SubSurfaceRadius);
    ADMF_DEF(SubSurfaceValue);
    ADMF_DEF(Transmission);

    
    
    /// 字符串类
    class StringReadOnly_
    {
    public:
        virtual bool isEmpty() = 0;
        /// 获取字符串长度
        ///    @see ADMFUINT
        virtual ADMF_UINT getLength() = 0;
        /// 获取字符串
        ///      @param buff 传入的指针
        ///    @see ADMFUINT
        virtual ADMF_UINT getString(ADMF_CHAR *buff, ADMF_UINT) = 0;
        
        virtual const std::string& getInternalString() = 0;
        

    };

    /// 字符串类
    class String_:public StringReadOnly_
    {

#ifdef ADMF_EDIT
    public:
        virtual void setString(const char *str) = 0;
#endif
    };

    /// 顶点类
    class Vec2_
    {
    public:
        ///   获取 X 值
        ///    @return    返回 X 值
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getX() = 0;
        ///   获取 Y值
        ///    @return    返回 Y 值
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getY() = 0;

#ifdef ADMF_EDIT
    public:
        /// 设置X
        ///      @param x x的值
        virtual void setX(ADMF_FLOAT x) = 0;
        /// 设置Y
        ///      @param y y的值
        virtual void setY(ADMF_FLOAT y) = 0;
#endif
    };

    /// 颜色类
    class ColorRGB_
    {
    public:
        ///   获取颜色 R 值
        ///    @return    返回颜色 R 值
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getR() = 0;
        ///   获取颜色 G 值
        ///    @return    返回颜色 G 值
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getG() = 0;
        ///   获取颜色 B 值
        ///    @return    返回颜色 B 值
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getB() = 0;
        ///   获取颜色空间值
        ///    @return    返回颜色空间值
        virtual String getColorSpace() = 0;

#ifdef ADMF_EDIT
    public:
        ///   设置颜色 R 值
        ///      @param r r的值
        virtual void setR(ADMF_FLOAT r) = 0;
        ///   设置颜色 G 值
        ///      @param g g的值
        virtual void setG(ADMF_FLOAT g) = 0;
        ///   设置颜色 B 值
        ///      @param b b的值
        virtual void setB(ADMF_FLOAT b) = 0;
#endif
    };

    /// 二进制数据结构
    class BinaryData_
    {
        //from file or zipentry? //for edit sdk

        //contains data and name fields
    public:
        ///   获取ADMF数据长度
        ///    @return    返回ADMF数据长度
        ///    @see ADMFUINT
        virtual ADMF_UINT getDataLength() = 0;
        ///   获取ADMF数据
        ///    @param buff 传入内存地址的key
        ///    @param len key 所对应value的字节长度
        ///    @return    返回value所对应的数据
        ///    @see ADMFUINT
        virtual ADMF_UINT getData(const void *buff, ADMF_UINT len) = 0;
        ///   获取二进制名字
        ///    @return    返回二进制名字
        virtual String getName() = 0;
        ///   导出ADMF文件
        ///    @param filePath 写入文件的路径
        ///    @return    返回导出结果
        ///    @see ADMFRESULT
        virtual ADMF_RESULT exportToFile(const char *filePath) = 0;
        
        virtual bool isEmpty() = 0;

#ifdef ADMF_EDIT
    public:
        ///通过二进制数据更新内容
        virtual ADMF_RESULT updateFromData(const void *buff, ADMF_UINT len) = 0;
        ///通过文件名更新内容
        virtual ADMF_RESULT updateFromFile(const char *filePath, bool needDelete = false) = 0;
        ///设置二进制名字
        virtual void setName(const char *name) = 0;
#endif
    };

    
    enum class TextureFileType {
        None = 0,
        RAW = 1,
        PNG = 2,
        JPG = 3,
        GIF = 4,
        TIFF = 5,
        // https://www.filesignatures.net
    };
    /// 贴图数据结构
    class Texture_
    {
    public:
        ///   获取二进制数据
        ///    @return    返回二进制数据
        virtual BinaryData getBinaryData() = 0; //content of "/normal.png""
                                                ///   获取贴图数据
        ///    @return    返回贴图数据
        ///    @see TEX_TYPE
        virtual TEX_TYPE getType() = 0;
        ///   获取贴图名字
        ///    @return    返回贴图名字
        virtual String getTypeName() = 0;
        ///                 获取贴图颜色空间
        ///    @return      返回贴图颜色空间
        virtual String getColorSpace() = 0;
        ///                 获取贴图密度
        ///    @return      返回贴图密度
        ///    @see Vec2
        virtual Vec2 getDpi() = 0;
        ///                 获取贴图宽度
        ///    @return      返回贴图宽度
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getWidth() = 0;
        ///                 获取贴图高度
        ///    @return      返回贴图高度
        ///    @see ADMFFLOAT
        virtual ADMF_FLOAT getHeight() = 0;

        virtual ADMF_FLOAT getPhysicalWidth() = 0;

        virtual ADMF_FLOAT getPhysicalHeight() = 0;
        ///获取通道数
        virtual ADMF_UINT getChannels() = 0;
        ///获取每个通道字节数
        virtual ADMF_UINT getElementSize() = 0;
        
        virtual TextureFileType getTypeByBinaryData() = 0;

#ifdef ADMF_EDIT
    public:
        ///                 获取贴图宽度
        ///                 @param width 设置的宽度
        virtual void setWidth(ADMF_FLOAT width) = 0;
        ///                 获取贴图高度
        ///                 @param height 设置的高度
        virtual void setHeight(ADMF_FLOAT height) = 0;
		
        virtual void setPhysicalWidth(ADMF_FLOAT width) = 0;
        virtual void setPhysicalHeight(ADMF_FLOAT height) = 0;
        ///设置通道数
        virtual void setChannels(ADMF_UINT channels) = 0;
        ///设置每个通道字节数
        virtual void setElementSize(ADMF_UINT elementSize) = 0;
#endif
    };

    template <typename T>
    /// 数组类
    class Array_
    {
    public:
        /// 获取数组大小
        virtual admf::ADMF_UINT size() = 0;
        /// 获取数组大小
        virtual T get(admf::ADMF_UINT index) = 0;

#ifdef ADMF_EDIT
    public:
        /// 清空数组
        virtual void clear() = 0;
        /// 数组追加
        virtual T append() = 0;
        /// 数组插入元素
        /// @param index 插入元素的索引
        virtual T insert(admf::ADMF_UINT index) = 0;
        /// 移除数组元素
        /// @param index 移除元素的索引
        virtual admf::ADMF_UINT remove(admf::ADMF_UINT index) = 0;
#endif
    };

}
//test
#endif /* admf_base_h */

//
//  admf_basecolordata.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_basecolordata_h
#define admf_basecolordata_h

namespace admf
{
    /// 基本颜色数据类
    class BaseColorData_
    {
    public:
        ///    获取类型
        ///    @return    返回当前生效的改色类型
        virtual String getType() = 0; //"null",

        ///    获取类型
        ///    @return    返回当前生效的改色色卡index
        virtual ADMF_INT getIndex() = 0; //"null",

        ///    获取实色/单色改色数据
        ///    @return    返回改色数据
        ///    @see BaseColorDataSolid
        virtual BaseColorDataSolid getSolid() = 0;

        ///    获取多色改色数据
        ///    @return    返回多色改色数据
        ///    @see BaseColorDataMulti
        virtual BaseColorDataMulti getMulti() = 0;
        
        


#ifdef ADMF_EDIT
        ///    获取类型
        ///    @param  index 当前生效的色卡index
        virtual void setIndex(ADMF_INT index) = 0;

#endif


    };

    /// 单色改色数据
    class BaseColorDataSolid_
    {
    public:
/*
        virtual String getColorSpace() = 0;   //"srgb",


        virtual ColorRGB getValue() = 0;      //

 
        virtual ADMF_INT getInputBlack() = 0;  //0,


        virtual ADMF_INT getInputWhite() = 0;  //255,


        virtual ADMF_FLOAT getGamma() = 0;     //1.0,


        virtual ADMF_INT getOutputBlack() = 0; //0,

        virtual ADMF_INT getOutputWhite() = 0; //0,
*/
        ///    获取改色数据数组
        ///    @return    返回改色数据数组
        virtual Array<BaseColorDataSolidBlock> getBlockArray() = 0;
        
        /*
#ifdef ADMF_EDIT
    public:
        /// 色阶改色输入黑场
        /// @param inputBlack 黑场值
        virtual void setInputBlack(ADMF_INT inputBlack) = 0;
        /// 色阶改色输入白场
        /// @param inputWhite 白场值
        virtual void setInputWhite(ADMF_INT inputWhite) = 0;
        /// 设置伽马
        /// @param gamma 伽马值
        virtual void setGamma(ADMF_FLOAT gamma) = 0;           //1.0,

        /// 色阶改色输出黑场
        /// @param outputBlack 黑场值
        virtual void setOutputBlack(ADMF_INT outputBlack) = 0; //0,

        /// 色阶改色输出白场
        /// @param outputWhite 白场值
        virtual void setOutputWhite(ADMF_INT outputWhite) = 0; //0,
#endif
         */
    };

    /// 单色改色数据块
    class BaseColorDataSolidBlock_
    {
    public:
        ///    获取色卡名字
        ///    @return    返回名字
        virtual String getName() = 0;  //"(166,202,240)",

        ///    获取色域类型
        ///    @return    返回色域类型
        virtual String getType() = 0;  //"RGB",

        ///    获取色卡值
        ///    @return    返回色卡值
        virtual String getValue() = 0; //"166,202,240",

        ///    是否原色(不做去色和改色)
        ///    @return    返回色卡值
        virtual ADMF_BYTE isOriginal() = 0; //0 => false ,
#ifdef ADMF_EDIT
        virtual void setOriginal(ADMF_BYTE isOriginal) = 0;
#endif
    };

    /// 多色改色数据
    class BaseColorDataMulti_
    {
    public:

        ///    获取多重改色块数组
        ///    @return    返回多重改色方案数组
        virtual admf::Array<BaseColorDataMultiBlock> getBlockArray() = 0;


    };

    /// 多色改色方案
    class BaseColorDataMultiBlock_
    {
    public:
        ///    获取色卡名字
        ///    @return    返回名字
        virtual admf::String getName() = 0;

        ///    获取色卡颜色值
        ///    @return    返回颜色值
        virtual admf::Array<BaseColorDataMultiBlockMask> getMaskArray() = 0;
    };

    /// 多色改色方案Mask数据
    class BaseColorDataMultiBlockMask_
    {
    public:
        ///    获取多色改色的Mask文件
        ///    @return    返回Mask文件
        virtual admf::Texture getMask() = 0;

        ///    获取改色颜色值
        ///    @return    返回改色颜色值
        virtual admf::String getValue() = 0; //"166,202,240",
    };
    
    /*
  
    class BaseColorChangeColorData_
    {
    public:
        virtual ADMF_BYTE isEnable() = 0;
        virtual ADMF_DOUBLE getBottomS() = 0;
        virtual ADMF_DOUBLE getBottomV() = 0;
        virtual ADMF_DOUBLE getMeanS() = 0;
        virtual ADMF_DOUBLE getMeanV() = 0;
        virtual ADMF_DOUBLE getKS() = 0;
        virtual ADMF_DOUBLE getKV() = 0;
        
#ifdef ADMF_EDIT
    public:
        virtual void setEnabled(ADMF_BYTE enabled) = 0;
        virtual void setBottomS(ADMF_DOUBLE bottomS) = 0;
        virtual void setBottomV(ADMF_DOUBLE bottomV) = 0;
        virtual void setMeanS(ADMF_DOUBLE meanS) = 0;
        virtual void setMeanV(ADMF_DOUBLE meanV) = 0;
        virtual void setKS(ADMF_DOUBLE kS) = 0;
        virtual void setKV(ADMF_DOUBLE kV) = 0;
#endif
    };
     */
    

}

#endif /* admf_basecolordata_h */

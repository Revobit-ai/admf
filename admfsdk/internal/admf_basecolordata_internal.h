//
//  admf_basecolordata_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_basecolordata_internal_hpp
#define admf_basecolordata_internal_hpp
#include <vector>
#include <memory>
#include "admf_internal.h"
namespace admf_internal {
    class String_internal;
    class BaseColorDataSolid_internal;
    class BaseColorDataMulti_internal;
    class ColorRGB_internal;
    class BaseColorDataSolidBlock_internal;




    class BaseColorData_internal : public admf::BaseColorData_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorData_internal);
    public:
        virtual admf::String getType() override; //"null",
        virtual admf::ADMF_INT getIndex() override;
        virtual admf::BaseColorDataSolid getSolid() override;
        virtual admf::BaseColorDataMulti getMulti() override;
        

        
#ifdef ADMF_EDIT
        virtual void setIndex(admf::ADMF_INT index) override;
#endif
    private:
        std::shared_ptr<String_internal> type_;
        std::shared_ptr<BaseColorDataSolid_internal> solid_;
        std::shared_ptr<BaseColorDataMulti_internal> multi_;
        admf::ADMF_INT index_  = 0;
    };

    class BaseColorDataSolid_internal : public admf::BaseColorDataSolid_, public Base_internal
    {
    public:
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorDataSolid_internal);
        /*
        virtual admf::String getColorSpace() override; //"srgb",
        virtual admf::ColorRGB getValue() override; //
        virtual admf::ADMF_INT getInputBlack() override; //0,
        virtual admf::ADMF_INT getInputWhite() override; //255,
        virtual admf::ADMF_FLOAT getGamma() override; //1.0,
        virtual admf::ADMF_INT getOutputBlack() override; //0,
        virtual admf::ADMF_INT getOutputWhite() override; //0,
        */
        virtual admf::Array<admf::BaseColorDataSolidBlock> getBlockArray() override;

    private:
        /*
        std::shared_ptr<String_internal> colorSpace_;
        std::shared_ptr<ColorRGB_internal> value_;
        admf::ADMF_INT inputBlack_ = 0;
        admf::ADMF_INT inputWhite_ = 255;
        admf::ADMF_FLOAT gamma_ = 1;
        admf::ADMF_INT outputBlack_ = 0;
        admf::ADMF_INT outputWhite_ = 255;
        */
        std::shared_ptr<Array_internal<admf::BaseColorDataSolidBlock>> blockArray_;
        
#ifdef ADMF_EDIT
    public:
        /*
        virtual void setInputBlack(admf::ADMF_INT inputBlack) override { inputBlack_ = inputBlack;};
        virtual void setInputWhite(admf::ADMF_INT inputWhite) override { inputWhite_ = inputWhite;};
        virtual void setGamma(admf::ADMF_FLOAT gamma) override { gamma_ = gamma;}; //1.0,
        virtual void setOutputBlack(admf::ADMF_INT outputBlack) override { outputBlack_ = outputBlack;};//0,
        virtual void setOutputWhite(admf::ADMF_INT outputWhite) override { outputWhite_ = outputWhite;}; //0,
        */
#endif
        };

    //color cards
    class BaseColorDataSolidBlock_internal : public admf::BaseColorDataSolidBlock_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorDataSolidBlock_internal);
    public:
        virtual admf::String getName() override; //"(166,202,240)",
        virtual admf::String getType() override; //"RGB",
        virtual admf::String getValue() override; //"166,202,240",
        virtual admf::ADMF_BYTE isOriginal() override; //0 => false ,
#ifdef ADMF_EDIT
        virtual void setOriginal(admf::ADMF_BYTE isOriginal) override;
#endif
    private:
        std::shared_ptr<String_internal> name_;
        std::shared_ptr<String_internal> type_;
        std::shared_ptr<String_internal> value_;
        admf::ADMF_BYTE isOriginal_ = 0;

    };

    class BaseColorDataMulti_internal : public admf::BaseColorDataMulti_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorDataMulti_internal);
    public:
        virtual admf::Array<admf::BaseColorDataMultiBlock> getBlockArray() override;
    private:
        std::shared_ptr<Array_internal<admf::BaseColorDataMultiBlock>> blockArray_;
    };


    class BaseColorDataMultiBlock_internal : public admf::BaseColorDataMultiBlock_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorDataMultiBlock_internal);
    public:
        virtual admf::String getName() override;
        virtual admf::Array<admf::BaseColorDataMultiBlockMask> getMaskArray() override;

    private:
        std::shared_ptr<String_internal> name_;
        std::shared_ptr<Array_internal<admf::BaseColorDataMultiBlockMask>> maskArray_;
    };


    class BaseColorDataMultiBlockMask_internal : public admf::BaseColorDataMultiBlockMask_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorDataMultiBlockMask_internal);
    public:
        virtual admf::Texture getMask() override;
        virtual admf::String getValue() override;

    private:
        std::shared_ptr<Texture_internal> mask_;
        std::shared_ptr<String_internal> value_;

    };
    /*
    class BaseColorChangeColorData_internal : public admf::BaseColorChangeColorData_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BaseColorChangeColorData_internal);
    public:
        virtual admf::ADMF_BYTE isEnable() override {return enabled_;};
        virtual admf::ADMF_DOUBLE getBottomS() override {return bottomS_;};
        virtual admf::ADMF_DOUBLE getBottomV() override {return bottomV_;};
        virtual admf::ADMF_DOUBLE getMeanS() override {return meanS_;};
        virtual admf::ADMF_DOUBLE getMeanV() override {return meanV_;};
        virtual admf::ADMF_DOUBLE getKS() override {return kS_;};
        virtual admf::ADMF_DOUBLE getKV() override {return kV_;};
        
    private:
        admf::ADMF_BYTE enabled_ = false;
        admf::ADMF_DOUBLE bottomS_ = false;
        admf::ADMF_DOUBLE bottomV_ = false;
        admf::ADMF_DOUBLE meanS_ = false;
        admf::ADMF_DOUBLE meanV_ = false;
        admf::ADMF_DOUBLE kS_ = false;
        admf::ADMF_DOUBLE kV_ = false;
        
#ifdef ADMF_EDIT
    public:
        virtual void setEnabled(admf::ADMF_BYTE enabled) override {enabled_ = enabled;};
        virtual void setBottomS(admf::ADMF_DOUBLE bottomS) override {bottomS_ = bottomS;};
        virtual void setBottomV(admf::ADMF_DOUBLE bottomV) override {bottomV_ = bottomV;};
        virtual void setMeanS(admf::ADMF_DOUBLE meanS) override {meanS_ = meanS;};
        virtual void setMeanV(admf::ADMF_DOUBLE meanV) override {meanV_ = meanV;};
        virtual void setKS(admf::ADMF_DOUBLE kS) override {kS_ = kS;};
        virtual void setKV(admf::ADMF_DOUBLE kV) override {kV_ = kV;};
#endif
    };
     */
}
#endif /* admf_basecolordata_internal_hpp */

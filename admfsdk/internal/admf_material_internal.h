//
//  admf_material_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_material_internal_hpp
#define admf_material_internal_hpp

#include "admf_internal.h"
#include <vector>
#include <memory>
namespace admf_internal {
    class MaterialLayer_internal;
    class MaterialDevice_internal;
    class String_internal;
    class MaterialMetaData_internal;
    class BinaryData_internal;


    class Material_internal : public admf::Material_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Material_internal);
    public:

        virtual admf::Array<admf::MaterialLayer> getLayerArray() override;
        virtual admf::MaterialLayer getSideLayer() override;

        virtual admf::ADMF_DATE getCreatedTime() override;  //"2018-11-08T12:35:37",
        virtual admf::ADMF_DATE getModifiedTime() override; //"2020-11-03T07:59:55Z",

        virtual admf::MaterialDevice getDevice() override;

        virtual admf::String getId() override; //"0001"
        virtual admf::String getName() override; //"PANTONE 20-0060TPM",

        virtual admf::MaterialMetaData getMetaData() override;
        

    private:
        std::shared_ptr<Array_internal<admf::MaterialLayer>> layerArray_;
        std::shared_ptr<MaterialLayer_internal> sideLayer_;
        admf::ADMF_DATE createTime_ = 0;
        admf::ADMF_DATE modifiedTime_ = 0;

        std::shared_ptr<MaterialDevice_internal> device_;
        std::shared_ptr<String_internal> id_;
        std::shared_ptr<String_internal> name_;
        std::shared_ptr<MaterialMetaData_internal> metaData_;
        
        

        
#ifdef ADMF_EDIT
        virtual void setCreatedTime(admf::ADMF_DATE date) override {createTime_ = date;};  //"2018-11-08T12:35:37",
        virtual void setModifiedTime(admf::ADMF_DATE date) override {modifiedTime_ = date;}; //"2020-11-03T07:59:55Z",
#endif

    };

    class MaterialDevice_internal : public admf::MaterialDevice_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(MaterialDevice_internal);
    public:
        virtual admf::String getType() override; //"material_scan",
        virtual admf::String getModel() override; //"Idemera",
        virtual admf::String getRevision() override; //"1.0",
        virtual admf::String getSerialNumber() override; //"1000"
    private:
        std::shared_ptr<String_internal> type_;
        std::shared_ptr<String_internal> model_;
        std::shared_ptr<String_internal> revision_;
        std::shared_ptr<String_internal> serialNumber_;
    };

    class MaterialMetaData_internal : public admf::MaterialMetaData_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(MaterialMetaData_internal);
    public:
        virtual admf::BinaryData getSource() override; // source material file
        virtual admf::String getType() override; //"4ddat",
        virtual admf::String getVersion() override; //"1.0",
    private:
        std::shared_ptr<BinaryData_internal> source_;
        std::shared_ptr<String_internal> type_;
        std::shared_ptr<String_internal> version_;
    };
}
#endif /* admf_material_internal_hpp */

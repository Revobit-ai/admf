//
//  admf_base_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_base_internal_hpp
#define admf_base_internal_hpp

#include "admf_internal.h"
//#include "ZipFile.h"
#include <vector>
#include <fstream>
#include <string>
#include <functional>
namespace admf_internal
{

    class StringReadOnly_internal : public admf::StringReadOnly_, public Base_internal
    {
        friend class ADMF_internal;

        ADMF_INTERNAL_CLASS_CONSTRUCTOR(StringReadOnly_internal);

    public:
        virtual admf::ADMF_UINT getLength() override;
        virtual bool isEmpty() override;
        virtual admf::ADMF_UINT getString(admf::ADMF_CHAR *buff, admf::ADMF_UINT) override;

        const std::string &getInternalString() override { return str_; };

    private:
        StringReadOnly_internal(const std::string &str) : Base_internal(-1), str_(str) {}
        std::string str_;
    };

    class String_internal : public admf::String_, public Base_internal
    {
        friend class MaterialMetaData_internal;
        friend class MaterialLayer_internal;
        friend class Texture_internal;
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(String_internal);

    public:
        virtual admf::ADMF_UINT getLength() override;
        virtual bool isEmpty() override;
        virtual admf::ADMF_UINT getString(admf::ADMF_CHAR *buff, admf::ADMF_UINT) override;

        const std::string &getInternalString() override { return str_; };

    private:
        String_internal(const std::string &str) : Base_internal(-1), str_(str) {}
        std::string str_;
#ifdef ADMF_EDIT
    public:
        virtual void setString(const char *str) override { str_ = str; };
#endif
    };

    class Vec2_internal : public admf::Vec2_, public Base_internal
    {
        friend class LayerTransform_internal;
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Vec2_internal);

    public:
        virtual admf::ADMF_FLOAT getX() override;
        virtual admf::ADMF_FLOAT getY() override;

    private:
        admf::ADMF_FLOAT x_ = 0;
        admf::ADMF_FLOAT y_ = 0;

#ifdef ADMF_EDIT
    public:
        virtual void setX(admf::ADMF_FLOAT x) override { x_ = x; };
        virtual void setY(admf::ADMF_FLOAT y) override { y_ = y; };
#endif
    };

    class ColorRGB_internal : public admf::ColorRGB_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(ColorRGB_internal);

    public:
        virtual admf::ADMF_FLOAT getR() override;
        virtual admf::ADMF_FLOAT getG() override;
        virtual admf::ADMF_FLOAT getB() override;
        virtual admf::String getColorSpace() override;

    private:
        std::shared_ptr<String_internal> colorSpace_;
        admf::ADMF_FLOAT r_ = 0;
        admf::ADMF_FLOAT g_ = 0;
        admf::ADMF_FLOAT b_ = 0;

#ifdef ADMF_EDIT
    public:
        virtual void setR(admf::ADMF_FLOAT r) override { r_ = r; };
        virtual void setG(admf::ADMF_FLOAT g) override { g_ = g; };
        virtual void setB(admf::ADMF_FLOAT b) override { b_ = b; };
#endif
    };

    class BinaryData_internal : public admf::BinaryData_, public Base_internal
    {
        friend class Texture_internal;
        friend class MaterialLayer_internal;
        friend class MaterialMetaData_internal;
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(BinaryData_internal);

    public:
        //contains data and name fields
        virtual admf::ADMF_UINT getDataLength() override;
        virtual admf::ADMF_UINT getData(const void *buff, admf::ADMF_UINT len) override;
        virtual admf::String getName() override;
        virtual admf::ADMF_RESULT exportToFile(const char *filePath) override;
        ZipArchiveEntry::Ptr getZipEntry();
        admf::String getAssignedName();
        admf::String getRawName();

        bool isEmpty() override { return getDataLength() == 0; };

        //如果BinaryData_internal被释放， 返回的std::istream*也会被reset， 多次调用这个函数，返回的是同一个指针(待验证)
        std::istream *getCompressedStream();

        std::string getNameString()
        {
            if (!assignedName_->isEmpty())
                return std::to_string(baseIndex_) + "_" + assignedName_->getInternalString();
            if (!name_->getInternalString().empty())
                return std::to_string(baseIndex_) + "_" + name_->getInternalString();
            return std::string("binary_") + std::to_string(baseIndex_);
        };

    private:
        std::shared_ptr<String_internal> name_;         //in zip
        std::shared_ptr<String_internal> assignedName_; //外部强制给的值， 例如texture给的base.png

    private:
        enum class OriginType
        {
            None = 0,
            ZipEntry = 1,
            File = 2,
            Data = 3,
        };

        struct OriginInfo
        {
            OriginType originType = OriginType::None;

            std::string content;

            bool needDeleteFile = false; //temp File

            void clear()
            {
                content.clear();
                originType = OriginType::None;
            }
            ~OriginInfo()
            {
                clear();
            }
        };
        OriginInfo originInfo_;

#ifdef ADMF_EDIT

    public:
        virtual void setName(const char *name) override;
        virtual admf::ADMF_RESULT updateFromData(const void *buff, admf::ADMF_UINT len) override;
        virtual admf::ADMF_RESULT updateFromFile(const char *filePath, bool needDelete) override;
        ;

#endif
    };

    class Texture_internal : public admf::Texture_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Texture_internal);

    public:
        virtual admf::BinaryData getBinaryData() override; //content of "/normal.png""
        virtual admf::TEX_TYPE getType() override;
        virtual admf::String getTypeName() override;
        virtual admf::String getColorSpace() override;
        virtual admf::Vec2 getDpi() override;
        virtual admf::ADMF_FLOAT getWidth() override;
        virtual admf::ADMF_FLOAT getHeight() override;
        virtual admf::ADMF_FLOAT getPhysicalWidth() override;
        virtual admf::ADMF_FLOAT getPhysicalHeight() override;
        virtual admf::ADMF_UINT getChannels() override { return channels_; };
        virtual admf::ADMF_UINT getElementSize() override { return elementSize_; };
        ;

    private:
        std::shared_ptr<String_internal> getTypeName_internal();

    private:
        std::shared_ptr<BinaryData_internal> binaryData_;
        admf::TEX_TYPE type_ = admf::TEX_TYPE_UNKNOWN;
        std::shared_ptr<String_internal> colorSpace_;
        std::shared_ptr<Vec2_internal> dpi_;
        admf::ADMF_FLOAT width_ = 0;
        admf::ADMF_FLOAT height_ = 0;
        admf::ADMF_FLOAT physicalWidth_ = 0;
        admf::ADMF_FLOAT physicalHeight_ = 0;
        admf::ADMF_UINT channels_ = 0;
        admf::ADMF_UINT elementSize_ = 0;
        std::shared_ptr<String_internal> typeName_;

    public:
        void setType(admf::TEX_TYPE type);

        virtual admf::TextureFileType getTypeByBinaryData() override;
        static admf::TextureFileType getTypeByBinaryData(const unsigned char *data, admf::ADMF_UINT len);
        std::string getExtensionByTextureFileType(admf::TextureFileType type);

#ifdef ADMF_EDIT
    public:
        virtual void setWidth(admf::ADMF_FLOAT width) override { width_ = width; };
        virtual void setHeight(admf::ADMF_FLOAT height) override { height_ = height; };
        virtual void setPhysicalWidth(admf::ADMF_FLOAT width) override { physicalWidth_ = width; };
        virtual void setPhysicalHeight(admf::ADMF_FLOAT height) override { physicalHeight_ = height; };
        virtual void setChannels(admf::ADMF_UINT channels) override { channels_ = channels; };
        virtual void setElementSize(admf::ADMF_UINT elementSize) override { elementSize_ = elementSize; };
        ;
#endif
    };

    template <class T>
    class Array_internal : public admf::Array_<T>
    {

    public:
        using ArrayElementCreateLambda = std::function<T(void)>;
        Array_internal() = delete;
        Array_internal(ArrayElementCreateLambda lambda) : createLambda(lambda){};
        ~Array_internal() = default;
        virtual admf::ADMF_UINT size() override { return (admf::ADMF_UINT)vector_.size(); };
        virtual T get(admf::ADMF_UINT index) override
        {
            if (index >= vector_.size())
                return nullptr;
            return vector_[index];
        };

    public:
        virtual void pushBack(T element) { vector_.emplace_back(element); };

    private:
        std::vector<T> vector_;
        ArrayElementCreateLambda createLambda;

#ifdef ADMF_EDIT
    public:
        virtual void clear() override { vector_.clear(); };
        virtual T append() override
        {
            T t = createLambda();
            vector_.emplace_back(t);
            return t;
        }
        virtual T insert(admf::ADMF_UINT index) override
        {
            if (index >= vector_.size())
                return append();

            T t = createLambda();
            vector_.insert(vector_.begin() + index, t);
            return t;
        }
        virtual admf::ADMF_UINT remove(admf::ADMF_UINT index) override
        {
            if (index < vector_.size())
                vector_.erase(vector_.begin() + index);
            return (admf::ADMF_UINT)vector_.size();
        };

    private:
        std::function<T(void)> lambda;
#endif
    };

}
#endif /* admf_base_internal_hpp */

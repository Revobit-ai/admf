//
//  admf_internal.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef admf_internal_h
#define admf_internal_h

#include "admf_cpp.h"
//#include "bson.h"
#include <unordered_map>
#include <string>



typedef struct _bson_t bson_t;
typedef struct _bson_iter_t bson_iter_t;
 
typedef struct _bson_reader_t bson_reader_t;
#ifdef ADMF_EDIT
#define ADMF_SAVE_FUNC_DECLARATION() virtual void save(bson_t* doc) override
#else
#define ADMF_SAVE_FUNC_DECLARATION()
#endif

#define ADMF_INTERNAL_CLASS_CONSTRUCTOR_(classname, baseClass) \
    private: \
        virtual void load(bson_iter_t *iter) override; \
        virtual void initMissed() override; \
    public: \
        classname(int admfIndex, bson_iter_t *iter = nullptr) \
        :baseClass(admfIndex) \
        { \
            load(iter); \
            initMissed(); \
        }; \
        ~classname() = default;\
        ADMF_SAVE_FUNC_DECLARATION(); \


#define ADMF_INTERNAL_CLASS_CONSTRUCTOR(classname) \
    ADMF_INTERNAL_CLASS_CONSTRUCTOR_(classname, Base_internal)


#define ADMF_BSON_APPEND_ARRAY(doc, key, variable, internalClass) \
{ \
    ADMF_UINT count = variable->size(); \
    if (count > 0) \
    { \
        bson_t child; \
        BSON_APPEND_ARRAY_BEGIN(doc, key.c_str(), &child); \
        for (int i=0; i< count; i++) \
        { \
            bson_t child_; \
            BSON_APPEND_DOCUMENT_BEGIN(&child, "", &child_); \
            auto p = variable->get(i); \
            dynamic_cast<internalClass*>(p.get())->save(&child_); \
            bson_append_document_end(&child, &child_); \
        } \
        bson_append_array_end(doc, &child); \
    } \
}

#define ADMF_BSON_APPEND_DOCUMENT(doc, key, variable) \
    {   \
        bson_t child; \
        BSON_APPEND_DOCUMENT_BEGIN(doc, key.c_str(), &child); \
        variable->save(&child); \
        bson_append_document_end(doc, &child); \
    }

#define ADMF_BSON_APPEND_BINARY(doc, key, variable)   \
    {   \
        BSON_APPEND_UTF8(doc, key.c_str(), variable->isEmpty() ?  "" :variable->getNameString().c_str()); \
        variable->save(doc);    \
    }

#define ADMF_BSON_APPEND_STRING(doc, key, variable)   \
    BSON_APPEND_UTF8(doc, key.c_str(), variable->getInternalString().c_str());

#define ADMF_BSON_APPEND_DOUBLE(doc, key, variable) \
    BSON_APPEND_DOUBLE(doc, key.c_str(), variable)

#define ADMF_BSON_APPEND_INT32(b, key, variable) \
    BSON_APPEND_INT32(b, key.c_str(), variable)

#define ADMF_BSON_APPEND_INT64(b, key, variable) \
    BSON_APPEND_INT64(b, key.c_str(), variable)

#define ADMF_BSON_APPEND_DATE_TIME(b, key, variable) \
    BSON_APPEND_DATE_TIME(b, key.c_str(), variable)

namespace admf_internal
{
    
    extern std::string getNewKey(const std::string& origKey);
    extern char* getNewKey_(const char* origKey);
    extern std::string getBsonString(const bson_iter_t *iter);
    extern unsigned int filesize(const std::string& filename);
    extern bool copyFile(const char* src, const char* dest);
    
    class Base_internal
    {
    protected:
        int admfIndex_;
        int baseIndex_;
    private:
        virtual void load(bson_iter_t *iter) = 0;
        virtual void initMissed() = 0; //no iter provided, create all the member objects.
#ifdef ADMF_EDIT
    public:
        virtual void save(bson_t* doc) = 0;
#endif
        
    public:
        Base_internal(int admfIndex):admfIndex_(admfIndex) {
            static int baseIndex = 1;
            baseIndex_ = baseIndex ++;
        }
        //virtual ~Base_internal() = 0;
        
        
    };
    
    
    
    //bson helper code
    //bson字段格式变化后进行兼容处理

    bool bson_iter_can_convert_to_double (const bson_iter_t *iter);

    extern admf::ADMF_UINT g_descriptionSizeLimit;
    extern admf::ADMF_UINT g_binarySizeLimit;
}



#endif /* admf_internal_h */

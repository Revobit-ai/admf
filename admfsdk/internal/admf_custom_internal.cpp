//
//  admf_custom_internal.cpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#include "admf_internal_header.h"
#include "bson.h"
using namespace admf_internal;


void Custom_internal::load(bson_iter_t *iter) //save
{
    
    if (iter == nullptr)
        return;
    
    if (!BSON_ITER_HOLDS_DOCUMENT(iter))
        return;
    
    bson_iter_t child;
    if (!bson_iter_recurse (iter, &child))
        return;
    
    valueMap_.clear();
    
    while (bson_iter_next (&child)) {
        std::string key = bson_iter_key(&child);
        auto value = admf_internal::getBsonString(&child);
        valueMap_[key] = value;
    }
}

void Custom_internal::initMissed()
{
}
#ifdef ADMF_EDIT
void Custom_internal::save(bson_t *doc)
{
    
    for (auto& iter : valueMap_)
    {
        BSON_APPEND_UTF8(doc, iter.first.c_str(), iter.second.c_str());
    }
    
}
#endif

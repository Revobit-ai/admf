//
//  admf_geometry_internal.cpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#include "admf_internal_header.h"
#include "bson.h"
using namespace admf_internal;
using namespace admf;

void Geometry_internal::load(bson_iter_t *iter) //save
{
    if (iter == nullptr)
        return;

    bson_iter_t child;
    if (BSON_ITER_HOLDS_DOCUMENT(iter) && bson_iter_recurse(iter, &child))
    {
        //TODO
    }
}

void Geometry_internal::initMissed()
{
}
#ifdef ADMF_EDIT
void Geometry_internal::save(bson_t *doc)
{
}
#endif
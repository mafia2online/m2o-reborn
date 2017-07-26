// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_components_interpolable_h
#define librg_components_interpolable_h

#include <librg/entities.h>
#include <librg/components/transform.h>

namespace librg
{
    struct interpolable_t {
        interpolable_t(transform_t tran) : time(), target(tran), latest(tran) {}

        double time;

        transform_t target;
        transform_t latest;
    };
}

#endif // librg_components_interpolable_h

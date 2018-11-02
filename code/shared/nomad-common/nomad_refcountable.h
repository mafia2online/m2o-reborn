#pragma once

/*nomad common- Nomad group common libs.
Author(s) : Force67

Copyright(c) 2015 - 2017 Nomad Group

Distributed under the MIT license(See accompanying file LICENSE or copy at
https://opensource.org/licenses/MIT)*/

namespace nomad
{
    template<typename ClassType>
    class nw_refcountable
    {
        static unsigned ref_count_;

    public:
           
        nw_refcountable() { ref_count_++; }
        nw_refcountable(nw_refcountable const&) { ref_count_++; }

        virtual ~nw_refcountable() { ref_count_--; }
        static unsigned count() { return ref_count_; }
    };

    template <typename ClassType>
    unsigned nw_refcountable<ClassType>::ref_count_ = 0;

}
#pragma once

/*nomad common- Nomad group common libs.
Author(s) : Force67

Copyright(c) 2015 - 2017 Nomad Group

Distributed under the MIT license(See accompanying file LICENSE or copy at
https://opensource.org/licenses/MIT)*/

#include <memory>

namespace nomad
{
    template <class ClassType>
    class static_singleton
    {
    public:

        static_singleton() = default;
        virtual ~static_singleton() = default;

        template <typename... Args>
        static constexpr ClassType& instance(Args... a) noexcept
        {
            static ClassType inst(a ...);
            return inst;
        }

        // Disable copy as this is a singleton
        static_singleton &operator=(const static_singleton&) = delete;
        static_singleton(const static_singleton&) = delete;
    };
}

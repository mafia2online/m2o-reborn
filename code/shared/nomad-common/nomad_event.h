#pragma once

/*nomad common- Nomad group common libs.
Author(s) : Force67

Copyright(c) 2015 - 2017 Nomad Group

Distributed under the MIT license(See accompanying file LICENSE or copy at
https://opensource.org/licenses/MIT)*/

#include <stdlib.h>
#include <forward_list>

namespace nomad
{
    template<typename... Args>
    class nw_event
    {
    public:
		using func_t = void(*)(Args...);

        nw_event() = default;
        virtual ~nw_event() = default;

        void connect(const func_t event)
        {
            handlers_.push_front(event);
        }

        size_t size() const
        {
            return handlers_._Size();
        }

		void operator()(Args... args)
        {
            for (const auto &fn : handlers_)
            {
                fn(args...);
            }
        }

    private:

		std::forward_list<func_t> handlers_;
    };
}

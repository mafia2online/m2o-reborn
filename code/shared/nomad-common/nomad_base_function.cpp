
/*nomad common- Nomad group common libs.
Author(s) : Force67

Copyright(c) 2015 - 2017 Nomad Group

Distributed under the MIT license(See accompanying file LICENSE or copy at
https://opensource.org/licenses/MIT)*/

#include <nomad_base_function.h>

namespace nomad
{
    namespace internal
    {
        static init_function_base* g_init_functions;

        void init_function_base::register_this()
        {
            next_ = g_init_functions;
            g_init_functions = this;
        }

        void init_function_base::run_all()
        {
            for (auto func = g_init_functions; func; func = func->next_)
            {
                func->run();
            }
        }
    }
}

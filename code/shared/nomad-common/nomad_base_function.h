#pragma once

/*nomad common- Nomad group common libs.
Author(s) : Force67

Copyright(c) 2015 - 2017 Nomad Group

Distributed under the MIT license(See accompanying file LICENSE or copy at
https://opensource.org/licenses/MIT)*/

namespace nomad
{
    namespace internal
    {
        class init_function_base
        {
            init_function_base * next_;

        public:
            virtual ~init_function_base() = default;

            init_function_base()
            {
                register_this();
            }

            virtual void run() = 0;

            static void run_all();
            void register_this();
        };
    }

    class base_function  : public internal::init_function_base
    {
    private:
        void(*function_)();

    public:
        explicit base_function(void(*fn)())
        {
            function_ = fn;
        }

        void run() override {
            function_();
        }
    };
}

#pragma once

/*nomad common- Nomad group common libs.
Author(s) : Force67

Copyright(c) 2015 - 2017 Nomad Group

Distributed under the MIT license(See accompanying file LICENSE or copy at
https://opensource.org/licenses/MIT)*/

#include <unordered_map>
#include <string>
#include <nomad_refcountable.h>
#include <cassert>

#ifdef _WIN32
#define MAKE_GLOBAL __declspec (selectany)
#else
#define MAKE_GLOBAL __attribute__((selectany))
#endif

namespace nomad
{
    template <typename ClassType>
    class instance_collection_base : public nw_refcountable<instance_collection_base<ClassType>>
    {
        std::unordered_map<std::string, ClassType> collection_;

    public:

        ClassType get_instance(const char *type_name)
        {
            auto it = collection_.find(type_name);
            ClassType inst = ClassType();

            if (it != collection_.end())
            {
                inst = it->second;
            }
            else
            {
                //Extensions
#ifdef WD2MP_DBG
                assert("Missing instance");
#endif
                //

                inst = nullptr;
            }

            return inst;
        }

        void add_instance(const char *type_name, const ClassType &inst)
        {
            collection_[type_name] = inst;
        }
    };

    using instance_collection = instance_collection_base<void*>;

    //Extensions
    class internal_collection
    {
    public:
        static instance_collection g_instance_collection;
    };
    //

    template <typename ClassType>
    class nw_instance
    {
        static const char *type_name;
        static ClassType *nw_catched_inst_;
      //  static ClassType inst;

    public:

        static ClassType *get(instance_collection *collection)
        {
           return static_cast<ClassType*>(collection->get_instance(type_name));
        }

        static void set(ClassType *inst, instance_collection *collection)
        {
            collection->add_instance(type_name, inst);
        }

        //Extensions
        static ClassType *get()
        {
            if (!nw_catched_inst_)
            {
                nw_catched_inst_ = static_cast<ClassType*>(internal_collection::g_instance_collection.get_instance(type_name));
            }

            return nw_catched_inst_;
        }

        static void set(ClassType *inst)
        {
            internal_collection::g_instance_collection.add_instance(type_name, inst);
        }

        template <typename... Args>
        static ClassType *create(Args... a) noexcept
        {
            static ClassType inst(a ...);
            internal_collection::g_instance_collection.add_instance(type_name, &inst);
            return &inst;
        }

        //

        static const char *get_name()
        {
            return type_name;
        }
    };

#define INST(name) nomad::nw_instance<name>::get()

#define DECLARE_INSTANCE_TYPE(name) \
	template<> MAKE_GLOBAL const char* nomad::nw_instance<name>::type_name = #name; \
    template<> MAKE_GLOBAL name* nomad::nw_instance<name>::nw_catched_inst_ = nullptr;// \
  //  template<> MAKE_GLOBAL name nomad::nw_instance<name>::inst;
}

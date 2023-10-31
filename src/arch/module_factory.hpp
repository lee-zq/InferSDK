#ifndef INFERSDK_MODULE_FACTORY_H_
#define INFERSDK_MODULE_FACTORY_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "all_type.h"
#include "base_module.h"

class ModuleRegistry
{
public:
    typedef std::shared_ptr<ModuleBase> (*Creator)(const ModuleParamType&);
    typedef std::map<std::string, Creator> CreatorRegistry;

    static CreatorRegistry& Registry()
    {
        static CreatorRegistry* g_registry_ = new CreatorRegistry();
        return *g_registry_;
    }

    // Adds a creator.
    static void AddCreator(const std::string& type, Creator creator)
    {
        CreatorRegistry& registry = Registry();
        // CHECK_EQ(registry.count(type), 0)
        //     << "Module type " << type << " already registered.";
        registry[type] = creator;
    }

    // Get a module using a InitParam.
    static std::shared_ptr<ModuleBase> CreateModuleBase(const ModuleParamType& param)
    {
        // if (Caffe::root_solver()) {
        //   LOG(INFO) << "Creating module " << param.name();
        // }
        const std::string& type = param.name;
        CreatorRegistry& registry = Registry();
        // CHECK_EQ(registry.count(type), 1) << "Unknown module type: " << type
        //     << " (known types: " << ModuleBaseTypeListString() << ")";
        return registry[type](param);
    }

    static std::vector<std::string> ModuleTypeList()
    {
        CreatorRegistry& registry = Registry();
        std::vector<std::string> module_types;
        for (typename CreatorRegistry::iterator iter = registry.begin(); iter != registry.end(); ++iter)
        {
            module_types.push_back(iter->first);
        }
        return module_types;
    }

private:
    // Module registry should never be instantiated - everything is done with its
    // static variables.
    ModuleRegistry()
    {
    }

    static std::string ModuleTypeListString()
    {
        std::vector<std::string> module_types = ModuleTypeList();
        std::string module_types_str;
        for (std::vector<std::string>::iterator iter = module_types.begin(); iter != module_types.end(); ++iter)
        {
            if (iter != module_types.begin())
            {
                module_types_str += ", ";
            }
            module_types_str += *iter;
        }
        return module_types_str;
    }
};

class ModuleRegisterer
{
public:
    ModuleRegisterer(const std::string& type, std::shared_ptr<ModuleBase> (*creator)(const ModuleParamType&))
    {
        // LOG(INFO) << "Registering module type: " << type;
        ModuleRegistry::AddCreator(type, creator);
    }
};

#define REGISTER_MODULE_CLASS(name)                                                                                            \
    shared_ptr<ModuleBase> Creator_##name##Module(const ModuleParamType& param)                                                \
    {                                                                                                                          \
        return shared_ptr<ModuleBase>(new name(param));                                                                        \
    }                                                                                                                          \
    static ModuleRegisterer g_creator_##name(#name, Creator_##name##Module);

#endif // INFERSDK_MODULE_FACTORY_H_

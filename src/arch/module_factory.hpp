#ifndef INFERSDK_MODULE_FACTORY_H_
#define INFERSDK_MODULE_FACTORY_H_

#include <map>
#include <string>
#include <vector>

#include "module/imodule.hpp"

class ModuleRegistry {
 public:
  typedef shared_ptr<IModule> (*Creator)(const CreateParam&);
  typedef std::map<string, Creator> CreatorRegistry;

  static CreatorRegistry& Registry() {
    static CreatorRegistry* g_registry_ = new CreatorRegistry();
    return *g_registry_;
  }

  // Adds a creator.
  static void AddCreator(const string& type, Creator creator) {
    CreatorRegistry& registry = Registry();
    // CHECK_EQ(registry.count(type), 0)
    //     << "Module type " << type << " already registered.";
    registry[type] = creator;
  }

  // Get a module using a InitParam.
  static shared_ptr<IModule> CreateIModule(const CreateParam& param) {
    // if (Caffe::root_solver()) {
    //   LOG(INFO) << "Creating module " << param.name();
    // }
    const string& type = param.type();
    CreatorRegistry& registry = Registry();
    // CHECK_EQ(registry.count(type), 1) << "Unknown module type: " << type
    //     << " (known types: " << IModuleTypeListString() << ")";
    return registry[type](param);
  }

  static vector<string> ModuleTypeList() {
    CreatorRegistry& registry = Registry();
    vector<string> module_types;
    for (typename CreatorRegistry::iterator iter = registry.begin();
         iter != registry.end(); ++iter) {
      module_types.push_back(iter->first);
    }
    return module_types;
  }

 private:
  // Module registry should never be instantiated - everything is done with its
  // static variables.
  ModuleRegistry() {}

  static string ModuleTypeListString() {
    vector<string> module_types = ModuleTypeList();
    string module_types_str;
    for (vector<string>::iterator iter = module_types.begin();
         iter != module_types.end(); ++iter) {
      if (iter != module_types.begin()) {
        module_types_str += ", ";
      }
      module_types_str += *iter;
    }
    return module_types_str;
  }
};

class ModuleRegisterer {
 public:
  ModuleRegisterer(const string& type,
                  shared_ptr<IModule> (*creator)(const CreateParam&)) {
    // LOG(INFO) << "Registering module type: " << type;
    ModuleRegistry::AddCreator(type, creator);
  }
};

#define REGISTER_MODULE_CLASS(name)                                          \
  shared_ptr<IModule> Creator_##name##Module(const CreateParam& param)       \
  {                                                                            \
    return shared_ptr<IModule>(new name(param));                       \
  }                                                                              \
  static IModuleRegisterer g_creator_f_##name(#name, Creator_##name##Module);    

#endif  // INFERSDK_MODULE_FACTORY_H_

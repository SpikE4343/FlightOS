/*************************************************************************************
g3::ModuleManager
Copyright (c) 2016 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_ModuleManager_H_INCLUDED
#define g3_ModuleManager_H_INCLUDED

#include <unordered_map>

#include "g3Types.h"
#include "g3Threading.h"

namespace g3
{
  template<typename TOwner>
  class ModuleManager;

  //
  // Interface for swappable module
  //
  class IModule
  {
  public:
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
  };

  //
  // Generic module interface
  //
  template<typename TOwner>
  class Module : public IModule
  {
  protected:
    TOwner* owner;

    friend class ModuleManager<TOwner>;
  };

  //
  // Container of IModule<TOwner> types
  //
  template<typename TOwner>
  class ModuleManager
  {
  public:
    ModuleManager()
    {

    }

    virtual ~ModuleManager()
    {

    }

    //
    //
    //
    template<typename TAddModule>
    void addModule(TAddModule* module)
    {
      TypeId id = TypeReference<TAddModule>::getId();
      module->owner = this;
      modules[id] = (IModule*)module;
      module->initialize();
    }

    //
    //
    //
    template<typename TGetModule>
    TGetModule* getModule()
    {
      TypeId id = TypeReference<TGetModule>::getId();
      auto iter = modules.find(id);
      if (iter == modules.end())
        return nullptr;

      return iter->second;
    }

    //
    //
    //
    template<typename TRemoveModule>
    void removeModule(TRemoveModule* module)
    {
      TypeId id = TypeReference<TRemoveModule>::getId();
      auto iter = modules.find(id);
      if (iter == modules.end())
        return;

      auto module = iter->second;
      module->shutdown();
      module->owner = nullptr;
      modules.erase(id);  
    }

    void removeAllModules()
    {

    }

  private:
    std::unordered_map<TypeId, IModule*> modules;
  };
}

#endif

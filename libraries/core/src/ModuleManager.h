//=====================================================================
// This file is part of FlightOS.
//
// FlightOS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FlightOS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlightOS.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FlightOS_ModuleManager_H_INCLUDED
#define FlightOS_ModuleManager_H_INCLUDED

#include <unordered_map>
#include <memory>
#include <shared_mutex>

#include "Types.h"
#include "Threading.h"
#include "Util.h"

namespace FlightOS
{
  template<typename TOwner>
  class ModuleManager;

  /// Interface for swappable module
  class IModule
  {
  public:
    virtual int moduleBind() = 0;
    virtual int moduleUnbind() = 0;
  };

  namespace {
    template<typename TModifierOwner>
    class IModuleOwnerModifier
    {
    public:
      virtual void setOwner(TModifierOwner* owner) = 0;
    };
  }

  /// Generic module interface
  template<typename TOwner>
  class Module 
    : public IModule
    , public IModuleOwnerModifier<TOwner>
  {
  protected:
    TOwner* owner() { return mOwner; }

    void setOwner(TOwner* _owner) { mOwner = _owner; }

    // Module
    virtual int moduleBind() 
    {
      return 0;
    }
    virtual int moduleUnbind()
    {
      return 0;
    }
    
  private:
    TOwner* mOwner;

    friend class ModuleManager<TOwner>;
  };

  /// Container of IModule<TOwner> types
  template<typename TOwner>
  class ModuleManager
  {
  public:
    ModuleManager()
    {

    }

    virtual ~ModuleManager()
    {
      removeAllModules();
    }

    ///
    template<typename TAddModule>
    void addModule(TAddModule* module)
    {
      fosAssert(module != NULL);

      TypeId id = TypeReference<TAddModule>::getId();

      {
        ModulesLock addLock(mModulesLock);
        modules[id] = (IModule*)module;
      }

      ((IModuleOwnerModifier<TOwner>*)module)->setOwner((TOwner*)this);
      module->moduleBind();
    }

    ///
    template<typename TCreateModule>
    TCreateModule* createModule()
    {
      auto module = new TCreateModule();
      addModule(module);
      return module;
    }

    template<typename TCreateModule, typename TAsModuleType>
    TCreateModule* createModuleAs()
    {
      auto module = new TCreateModule();
      addModule<TAsModuleType>((TAsModuleType*)module);
      addModule<TCreateModule>(module);
      return module;
    }

    ///
    template<typename TGetModule>
    TGetModule* getModule()
    {
      TypeId id = TypeReference<TGetModule>::getId();

      ModulesSharedLock lock(mModulesLock);
      auto iter = modules.find(id);
      if (iter == modules.end())
        return nullptr;

      return (TGetModule*)(iter->second);
    }

    /// Shutdown
    template<typename TRemoveModule>
    void removeModule(TRemoveModule* module, bool deleteModule=false)
    {
      g3Assert(module != NULL);

      mModulesLock.lock_shared();

      TypeId id = TypeReference<TRemoveModule>::getId();
      auto iter = modules.find(id);
      if (iter == modules.end())
        return;

      mModulesLock.unlock_shared();

      {
        ModulesLock removeLock(mModulesLock);
        modules.erase(id);
      }
      
      module->moduleUnbind();
      ((IModuleOwnerModifier<TOwner>*)module)setOwner(NULL);

      if (deleteModule)
      {
        delete module;
        module = NULL;
      }
      
    }

    /// Shutdown all registered modules
    void removeAllModules(bool deleteModules=true)
    {
      ModulesLock lock(mModulesLock);
      for (auto iter = modules.begin(); iter != modules.end(); ++iter)
      {
        auto module = iter->second;
        module->moduleUnbind();
        if (deleteModules)
        {
          delete module;
          module = NULL;
        }
      }

      modules.clear();
    }

    typedef std::unordered_map<TypeId, IModule*> ModuleTypeHash;


    ModuleTypeHash::iterator getModulesBegin()
    {
      return modules.begin();
    }

    ModuleTypeHash::iterator getModulesEnd()
    {
      return modules.end();
    }

  private:
    ModuleTypeHash modules;
    typedef std::shared_lock<std::shared_timed_mutex> ModulesSharedLock;
    typedef std::shared_lock<std::shared_timed_mutex> ModulesLock;
    std::shared_timed_mutex mModulesLock;
  };
}

#endif

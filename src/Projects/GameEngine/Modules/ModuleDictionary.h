#ifndef MODULE_DICTIONARY_H
#define MODULE_DICTIONARY_H

#include <map>
#include <vector>
#include "GameEngine/Util/Identifier/Incrementator.h"
#include "IEngineModule.h"

#include "GameEngine/Macros/DLL.h"

#pragma warning(push)
#pragma warning( disable : 4251 ) // std::map export

namespace GameEngine
{
   class IEngineModule;

   /// Contains a collection of dictionary.
   class DLL_DECL ModuleDictionary
   {
      friend class iterator; /// It's iterator needs to have access to private member to iterate
   public:
      /// Simple iterator to iterate through all the modules of this collection
      struct iterator
      {
      public:
         /// \param mContainer ModuleDictionnary that this iterator came from
         /// \param aModuleIndex The index of the module inside the modules' id list
         inline iterator(const ModuleDictionary& mContainer, int aModuleIndex);
         /// Go to the next module.
         inline iterator& operator++();
         /// Get the IEngineModule from the iterator
         inline IEngineModule* operator->();
         inline bool operator !=(const iterator& b) const;

      private:
         const ModuleDictionary& mContainer;   ///< Container that the iterator came from
         int mModuleIndex;                      ///< Index of the module inside the modules' id list

         iterator & operator=( const iterator & ) {}
      };

      /// Add a module to the collection
      void Add(IEngineModule* aModule);
      /// Get a module from the collection
      IEngineModule* Get(ModuleId aId);
      /// Remove a module from the collection
      void Remove(IEngineModule* aModule);

      /// Get the an iterator pointing to the first module
      inline iterator Begin() const;

      /// Get the an iterator pointing at the end of the container
      inline iterator End() const;

   private:
      /// Get the module at the specified position
      inline IEngineModule& Get(int i) const;

      std::map<ModuleId, IEngineModule*> mModules; ///< Dictionary of modules
      std::vector<ModuleId> mModulesId; ///< List of modules' id (for quick traversal)
   };
}

#include "ModuleDictionary.inl"

#pragma warning(pop)

#endif MODULE_DICTIONARY_H
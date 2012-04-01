#ifdef MODULE_DICTIONARY_H

namespace GameEngine
{
   inline ModuleDictionary::iterator ModuleDictionary::Begin() const 
   { 
      return iterator(*this, 0);
   }

   inline ModuleDictionary::iterator ModuleDictionary::End() const 
   { 
      return iterator(*this, mModulesId.size());
   }

   inline IEngineModule& ModuleDictionary::Get(int i) const 
   {
      return *mModules.find(mModulesId[i])->second;
   }

   inline ModuleDictionary::iterator::iterator(const ModuleDictionary& aContainer, int aModuleIndex)
      : mContainer(aContainer)
      , mModuleIndex(aModuleIndex)
   {
   }

   inline ModuleDictionary::iterator& ModuleDictionary::iterator::operator++()
   {
      ++mModuleIndex;

      return *this;
   }

   inline IEngineModule* ModuleDictionary::iterator::operator->()
   {
      return &mContainer.Get(mModuleIndex);
   }

   inline bool ModuleDictionary::iterator::operator!=(const ModuleDictionary::iterator& aOtherIterator) const
   {
      return mModuleIndex != aOtherIterator.mModuleIndex;
   }
}

#endif
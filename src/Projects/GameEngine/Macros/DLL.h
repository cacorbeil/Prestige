#if defined DLL_EXPORT
   #define DLL_DECL __declspec(dllexport)
#else
   #define DLL_DECL __declspec(dllimport)
#endif

#define EXPORT_STL_VECTOR( vectype ) \
  template class DLL_DECL std::allocator< vectype >; \
  template class DLL_DECL std::vector<vectype, \
    std::allocator< vectype > >;
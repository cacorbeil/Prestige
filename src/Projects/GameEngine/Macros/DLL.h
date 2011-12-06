#if defined DLL_EXPORT
	#define DLL_DECL __declspec(dllexport)
#else
	#define DLL_DECL __declspec(dllimport)
#endif

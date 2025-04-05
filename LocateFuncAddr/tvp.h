#include <afxwin.h>

typedef unsigned int tjs_uint;
#ifdef __cplusplus
typedef wchar_t tjs_char;
#else
typedef unsigned short tjs_char;
#endif

struct iTVPFunctionExporter {
	virtual bool __cdecl QueryFunctions(const tjs_char **name, void **function, tjs_uint count) = 0;
	virtual bool __cdecl QueryFunctionsByNarrowString(const char **name, void **function, tjs_uint count) = 0;
};

static iTVPFunctionExporter *TVPFunctionExporter = NULL;

void *TVPGetImportFuncPtr(const char *name) {
	if (TVPFunctionExporter) {
		void *ptr;
		if (TVPFunctionExporter->QueryFunctionsByNarrowString(&name, &ptr, 1)) {
			return ptr;
		}
		else {
			AfxMessageBox("Unknown function!");
		}
	}
	else {
		AfxMessageBox("Initialization isn't finished!");
	}
	return nullptr;
}

bool TVPInitImportStub(iTVPFunctionExporter *exporter) {
	TVPFunctionExporter = exporter;
	return true;
}

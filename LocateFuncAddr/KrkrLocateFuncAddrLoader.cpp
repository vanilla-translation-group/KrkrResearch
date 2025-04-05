#include <stdlib.h>
#include <Windows.h>
#include "../src/detours.h"
#pragma comment(lib, "../detours.lib")

int main(int argc, char* argv[]) {
	if (argc > 1) {
		STARTUPINFOA si = { sizeof(STARTUPINFOA) };
		PROCESS_INFORMATION pi = {0};
		DetourCreateProcessWithDllA(
			argv[1],
			NULL, NULL, NULL, FALSE, 0, NULL, NULL,
			&si, &pi,
			"KrkrLocateFuncAddr.dll",
			NULL
		);
	}
	else {
		return 1;
	}
	return 0;
}

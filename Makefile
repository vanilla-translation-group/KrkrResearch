all: icon LocateFuncAddr

icon: logo.ico
	rc icon.rc

LocateFuncAddr: icon
	cd LocateFuncAddr
	cl /LD /D_WINDLL /D_USRDLL KrkrLocateFuncAddr.cpp exports.def /link /SUBSYSTEM:WINDOWS
	cl KrkrLocateFuncAddrLoader.cpp ..\icon.res
	cd ..

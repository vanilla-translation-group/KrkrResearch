# KiriKiri-related utilities

## LocateFuncAddr

Query the pointer address of the imported function. For krkrz only, compatibility with krkr2 and other versions has not yet been tested.

How to use: drag-and-drop main program onto KrkrLocateFuncAddrLoader.exe (make sure that KrkrLocateFuncAddr.dll is in the same directory).

## ReinsertBres

Decrypt and encrypt the binary resource files embedded in main program.

How to use: run `findfirstblock.js` with frida, then select type and paste the block found.

TODO: reverse the block generation algorithm

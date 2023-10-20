# <img src="Resources/Images/DcmTools.png" width="40" > Project Functions

> **Function**
> - .dcm files to (.mhd,.raw) files
> - (.mhd,.raw) permute by defined axes
> - (.mhd,.raw) flip by defined axes
> - (.mhd,.raw) clip

# <img src="Resources/Images/DcmTools.png" width="40" > Build this project

## modify CMakeLists.txt

This project relies on three libraries: VTK/DCMTK/GDCM.

And use Qt as the GUI interaction interface.

## Possible issues with generating and running exe

If you do not set Qt environment variables, there may be problems during code execution.

Solution: Enter the Qt installation directory and locate windeployqt.exe. For example:

D:\DevTools\Qt\5.9\msvc2017_64\bin

Then call the windeployqt.exe:

windeployqt.exe D:\YourDirectory\DcmTools.exe














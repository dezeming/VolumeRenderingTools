# <img src="Resources/Images/DcmTools.png" width="40" > Project Functions

> **功能**
> - .dcm文件转为(.mhd,.raw)文件
> - (.mhd,.raw)按轴置换(permute)
> - (.mhd,.raw)按轴翻转(flip)
> - (.mhd,.raw)裁剪(clip)

# <img src="Resources/Images/DcmTools.png" width="40" > Build this project

## 修改CMakeLists.txt

本项目依赖三个库：VTK/DCMTK/GDCM，并使用Qt作为GUI交互界面。


## 生成exe运行时可能存在的问题

Qt不设置环境变量，因此代码执行时可能会出现问题。

解决方法：进入Qt安装目录，找到windeployqt.exe。例如：

D:\DevTools\Qt\5.9\msvc2017_64\bin

然后对生成exe目录下的exe文件调用：

windeployqt.exe D:\你的目录\DcmTools.exe














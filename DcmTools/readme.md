# <img src="Resources/Images/DcmTools.png" width="40" > Project Functions

## Implemented/Unrealized Features

> **Functions-convert**
> - convert (.dcm) image sequence to (.mhd,.raw) file (Compressed)
> - convert (.dcm) image sequence to (Feimos) format file (Uncompressed)

> **Functions-process (.mhd,.raw) format file**
> - permute (.mhd,.raw) by defined axes
> - flip (.mhd,.raw) by defined axes
> - clip (.mhd,.raw) by defined boundaries
> - downsampling (.mhd,.raw) according to the interval
> - resize (.mhd,.raw) according to the interval

> **Functions-process (Feimos) format file**
> - permute (Feimos) by defined axes
> - flip (Feimos) by defined axes
> - clip (Feimos) by defined boundaries
> - downsampling (Feimos) according to the interval
> - resize (Feimos) according to the interval

## Some setting information

To ensure security, it is mandatory not to overwrite any files when generating files.

The tags in the DICOM file contain information about the image, including pixel data type, image size, pixel spacing, and so on. For example, 
the label Pixel Representation (0028, 0103) is used to specify the representation of pixel data (signed or unsigned), 
while the label Bits Allocated (0028, 0100) specifies the number of bits per pixel.

(.mhd,.raw) files are typically file formats used to describe medical image data, 
and they are often used in conjunction with open-source medical image processing tools such as 
ITK (Insight Segmentation and Registration Toolkit) and VTK (Visualization Toolkit). 
These two files are usually used together, 
where the .mhd file contains metadata, while the (.raw) file contains the actual binary image data.

If the file format described is not (.mhd) but only one (.raw) file, then it is an uncompressed binary data file. 
To parse, you need to know the resolution and format of the data. Here I use the "Feimos" format to record its size and data format.

## Note

The complete version of the foundation of this project is currently under development, and the executable program has a large number of unimplemented features and bugs.

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














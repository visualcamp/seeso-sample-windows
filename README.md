<p align="center">
    <img src="https://seeso.io/img/logo_shape.bdd77355.svg"
        height="130">
    <img src="https://seeso.io/img/logo_text.6064523e.svg"
        height="130">
</p>
<div align="center">
    <h1>SeeSo Windows Sample</h1>
    <a href="https://github.com/visualcamp/seeso-sample-windows/releases" alt="release">
        <img src="https://img.shields.io/badge/version-2.4.0-blue" />
    </a>
</div>

## Sample Demo Requirements
* [SeeSo Windows SDK](https://console.seeso.io/)
* Must be issued a license key in [SeeSo Console](https://console.seeso.io/)
* Windows 10
* C++14
* [CMake](https://cmake.org/download/) >= 3.5
* [OpenCV](https://opencv.org/releases/)


## Build
1. Sign in to https://seeso.io and download a SeeSo Windows SDK from https://console.seeso.io
2. Copy the `seeso` folder to this project.
3. Get a license key from https://console.seeso.io and copy your key to `main.cpp`
   ```
   // authenticate
   const char* license_key = "PUT YOUR LICENSE KEY HERE"; // paste your key here
   ```
4. Download OpenCV windows library from https://opencv.org/releases/
5. Copy the extracted `opencv` into this project's [`opencv` directory](opencv/)  
    By now, the project structure should be like below  
    ![](media/1.PNG)
6. Build the project with CMake
    * You can load a CMake project with IDEs, such as CLion or Visual Studio
    * Or you can build with command line  
    ```
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
    ```
    * vcpkg is not supported yet. If you want to build with Visual Studio project instead of CMake,
      you have to manually implement the source codes and third party libraries.
      
If you have any problems, feel free to [contact us](https://seeso.io/Contact-Us) 

<p align="center"><a title="Jeremy Kratz, Public domain, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:ISO_C%2B%2B_Logo.svg"><img width="256" alt="ISO C++ Logo" src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/ISO_C%2B%2B_Logo.svg/256px-ISO_C%2B%2B_Logo.svg.png"></a></p>

<h1 align="center">Scarlet Nexus Trainer Internal</h1>
<p align="center">
  <a href="https://github.com/skript023/Scarlet-Nexus-Internal/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/skript023/Scarlet-Nexus-Internal.svg?style=flat-square"/>
   </a>
  <a href="https://github.com/skript023/Scarlet-Nexus-Internal/actions">
      <img src="https://img.shields.io/github/workflow/status/skript023/Scarlet-Nexus-Internal/CI/main?style=flat-square"/>
   </a>
  <br>
  A trainer menu base Scarlet Nexus Trainer.
  Strictly for educational purposes.
</p>

## Features
* ImGui–based user interface
* Log console
* Unreal Classes
* Native Function Access
* Fiber Pool
* Thread Pool
* Unload at runtime

## Bugs
* Trainer window will not show if you have overlay hooked for example MSI Afterburner and something like that.


## Building
To build Scarlet Nexus Internal you need:
* Visual Studio 2022
* [Premake 5.0](https://premake.github.io/download.html) in your PATH

To set up the build environment, run the following commands in a terminal:
```dos
git clone https://github.com/skript023/Scarlet-Nexus-Internal.git --recurse-submodules
GenerateProjects.bat
```
Now, you will be able to open the solution, and simply build it in Visual Studio.

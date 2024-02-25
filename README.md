
# Schism
## An experimental game engine
![example workflow](https://github.com/42dotmk/schism/actions/workflows/build.yml/badge.svg)

## Description
This is the repository for base42's experimental learning experience game engine.
Everything here will have major breaking changes for the time being as the API stablizes.

For now the project tasks can be found at:
https://github.com/orgs/42dotmk/projects/5

### Planned features:
Ordered by priorities
- Ecs api similar to [bevy](https://github.com/bevyengine/bevy)
- Lua scripting
- Editor
- 3D Rendering
- Deffered shading
- Graphics api abstraction
- Stable rendering system (possibly multithreaded with vulkan)
- Networking

## Getting started
#### Clone the repository
Clone the repository and all of it's submodules with your preferred method, example below is using https 

```bash
git clone --recursive https://github.com/42dotmk/schism
cd prism
```

#### Setup vcpkg
Follow the steps in the official vcpkg repository:
https://github.com/microsoft/vcpkg
`VCPKG_ROOT` has to be set as an environment variable that will point to root directory of vcpkg.

#### Build

##### Linux:
###### Install dependencies
Debian:
```
sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev libwayland-dev libxkbcommon-dev libglew-dev lib-tool autoconf automake
```
Installing the dependencies for other distributions should be fairly straightforward, make sure you also have `cmake` and `ninja` installed.
###### Generate cmake and build
In the `schism` directory run 
```bash
chmod +x ./cmake_generate.sh
chmod +x ./cmake_build.sh
./cmake_generate.sh
./cmake_build.sh
```
If successfull you should have the binaries in the `bin` directory.

##### Windows:
###### Visual Studio of Clion
For windows this will be fairly straightforward, you can just open up the root `schism` directory in visual studio or clion and it should be able to generate cmake and build the project, provided of course if you have setup vcpkg (make sure you have set the target to the schism executable).



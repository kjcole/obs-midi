# Compiling obs-midi

## Prerequisites

You'll need [Qt 5.15.2](https://download.qt.io/official_releases/qt/5.15/),
[CMake](https://cmake.org/download/) and a working [OBS Studio development environment](https://obsproject.com/wiki/install-instructions) installed on your
computer.

Once you have successfully built OBS Studio, in the obs-studio source directory

- Go into the {obs-studio}/plugins
- `git clone --recursive git@github.com:cpyarger/obs-midi.git`
- open {obs-studio}/source/CMakeLists.txt
- append `add_subdirectory(obs-midi)` to the end of the file
- rerun cmake-gui configure, generate 
- in cmake-gui you will need to set a few variables
  - (path)`LIBOBS_INCLUDE_DIR = {obs-studio}/libobs`
  - (filepath)`LIBOBS_LIB = {obs-studio-build}/libobs/Debug/obs.lib`
  - (path)`QTDIR = C:\Qt\5.15.2\msvc2019_64`
  - (boolean)`isCI = true`
  - (filepath)`OBS_FRONTEND_LIB = {obs-studio-build}/UI/obs-frontend-api/Debug/obs-frontend-api.lib`
 - in cmake-gui configure, generate, and open project

Now you should be good to edit things

## Linux

On Debian/Ubuntu :

```shell
sudo apt-get install libboost-all-dev
git clone --recursive git@github.com:cpyarger/obs-midi.git
cd obs-midi
mkdir build && cd build
cmake -DLIBOBS_INCLUDE_DIR="<path to the libobs sub-folder in obs-studio's source code>" -DCMAKE_INSTALL_PREFIX=/usr -DUSE_UBUNTU_FIX=true ..
make -j4
sudo make install
```

On other linux OS's, use this cmake command instead:

```shell
cmake -DLIBOBS_INCLUDE_DIR="<path to the libobs sub-folder in obs-studio's source code>" -DCMAKE_INSTALL_PREFIX=/usr ..
```

## OS X

As a prerequisite, you will need Xcode for your current OSX version, the Xcode command line tools, and [Homebrew](https://brew.sh/).
Homebrew's setup will guide you in getting your system set up, you should be good to go once Homebrew is successfully up and running.

Use of the macOS CI scripts is recommended. Please note that these
scripts install new software and can change several settings on your system. An
existing obs-studio development environment is not required, as
`install-build-obs-macos.sh` will install it for you. If you already have a
working obs-studio development environment and have built obs-studio, you can
skip that script.

Of course, you're encouraged to dig through the contents of these scripts to
look for issues or specificities.

```shell
git clone --recursive git@github.com:cpyarger/obs-midi.git
cd obs-midi
./CI/install-dependencies-macos.sh
./CI/install-build-obs-macos.sh
./CI/build-macos.sh
./CI/package-macos.sh
```

This will result in a ready-to-use `obs-midi.pkg` installer in the `release` subfolder.

## Automated Builds

![CI Multiplatform Build](https://github.com/cpyarger/obs-midi/workflows/CI%20Multiplatform%20Build/badge.svg)

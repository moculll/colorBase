# ColorBase Library
- ### Test Environment: Ubuntu 22.04 / Windows 10
- ### currently compile support:
```
1. vscode + cmake Extension on Windows/Linux
2. makefile on Linux
```
## Usage
```
git clone https://github.com/moculll/colorBase.git
make
./build/test
```

or

```
sudo apt-get install ninja-build cmake
git clone https://github.com/moculll/colorBase.git
cmake -G Ninja -S . -B build
cmake --build build
./build/colorBase

```

or

```
git clone https://github.com/moculll/colorBase.git
open project directory with Vscode
click Extensions button and install cmake/cmake tools
click the run button in the left buttom of vscode to run
```

## Embedded Device Support

### we have two places that are designed for porting:

- #### TimerEvents
this module is for colorEffects like color linear / color loop etc.
```

1. we need to implement the class TimerEvents, just make sure its start/stop functions work is enough
```

- #### colorBasePort
what we need to focus on colorBasePort only is the colorBasePort.cpp
```
1. we need to implement colorCallbackImpl<>, which template(rgb/hsv/cctb/cw) to impl depends on which color type you actually call in your application
(notice that if we want to uniform color type, you can call colorBase::effects::convert() in other impl functions)

2. since many of color devices may not use standard color range, we have makeGammaTable in colorBasePort.cpp, if your device receives 0-255, call makeGammaTable(*colorBasePort::gammaTable, 256, 1.0, 8) is enough
```


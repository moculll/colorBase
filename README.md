# ColorBase Library
- ### Test Environment: Ubuntu 22.04 / Windows 10
- ### currently compile support:
```
1. vscode(cmakeExtension) on Windows/Linux
2. makefile/cmake on Linux
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
# TimerEvents.h

1. [class TimerEvents]
```

- #### colorBasePort
```
# colorBasePort.cpp

1. [void colorBase::colorBasePort::colorCallbackImpl<colorType>(const Color<colorType &)]
(notice that if we want to uniform color type, you can call colorBase::effects::convert() in other impl functions)

2. [int colorBasePortMgr::getRandom(int, int)]
(this is for effects that randomly generate color vals)
```


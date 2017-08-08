# m2o-reborn

## Building (`VS` 2015/2017)

0. Install cmake

```
https://cmake.org/files/v3.9/cmake-3.9.0-win64-x64.msi
```

1. Clone

```sh
$ git clone git@gitlab.com:Mafia2Online/M2OClient-MyU.git
```
2. Generate project

```
run vs2015_cmake.bat
or  vs2017_cmake.bat
```

3. Open the solution

```
build/m2o-reborn.sln
```

4. Build

```
use gui in visual studio as always
```

5. Run

```
binary/launcher.exe
```

## Building (`make` linux/macOS)

0. Install cmake

```
depends on platform
```

1. Clone

```sh
$ git clone git@gitlab.com:Mafia2Online/M2OClient-MyU.git
```
2. Generate project

```
$ cd build && cmake ..
```

4. Build

```
$ make -j8
```

5. Run

```
$ binary/server
```

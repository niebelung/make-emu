# make-emu
  make utility emulator
  
  Parses the simplest makefile syntax like
  ```
  target : dependency0, dependency1,...
    action0
    action1
    ...
  ```
## PREREQUISITES
- cmake >=2.8 installed
- c++11 capable compiler (gcc 4.7.2 works)

## BUILD
```bash
  mkdir build
  cd build/
  cmake ..
  make
```
## RUN
```bash
  ../bin/make-emu [target]
```

# Not Among Us

## Installation

1. Create a directory `build` and go into it:

```
mkdir build
cd build
```

2. Build the makefile using `cmake`:

```
cmake ..
```

3. Compile the program using the `Makefile`:

```
make
```

4. Move to the root directory of the project and execute the program

```
./build/Assignment0
```

**Note**: Moving to root directory of the project is required. Without it,
shader resources will not be loaded and the executable may work in unexpected 
ways.

# Bezier curve drawing utility

Simple tool that allows drawing a cubic bezier curve by placing its control
points.

## Building

This utility depends solely on SDL3. As of now (2024-12-14) there is no official
release of SDL3, so you might need to build it yourself.

To build the program using meson, run the following commands.

```sh
meson setup build      # Generate the build directory
meson compile -C build # Compile the program
```

The resulting executagble will be stored in the build/ directory.

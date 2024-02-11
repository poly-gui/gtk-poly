# gtk-poly

This is an implementation of Poly using [GTK](https://www.gtk.org/) provided as a C++ library named `gtkpoly`. Poly
applications running on top of this layer will be native GTK4 apps. This library is automatically included in Poly
projects generated via the [Poly CLI](https://github.com/poly-gui/cli) as a Git submodule, in
the `<project>/linux/lib/gtk-poly` directory.

## Dependencies

- [nanopack](https://github.com/poly-gui/nanopack): A C++ support library
  for [nanopack](https://polygui.org/nanopack/introduction/), included as a Git submodule in `lib/nanopack`

## Building

This is a typical CMake project. To build this library locally, the following tools are required:

- CMake >= 3.27
- Make or Ninja as the CMake generator
- C++ compiler that supports C++20

First, clone this repository:

```
git clone https://github.com/poly-gui/gtk-poly.git
```

Then, make sure all the Git submodule dependencies are pulled:

```
git submodule update --init
```

The project is now ready to be configured and built.

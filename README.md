# About

These are simple test ROMs for N64 emulator development

The built binaries are placed in the respective directories (test_1, etc.) with README.md

# Setup

1. `git submodule update --init`

1. `npm install`

1. With docker running, `npx libdragon init`

1. Use libdragon `unstable` branch by

   `cd libdragon`

   `npx libdragon install`

# Build

For example, when building test_1,

```
cd test_1
npx libdragon make
```

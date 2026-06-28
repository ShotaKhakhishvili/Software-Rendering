# Software-Rendering

A CPU rasterizer that renders 3D meshes straight into the Windows console. No GPU, no DirectX, no OpenGL, just math and characters.

It loads an OBJ model, projects it into screen space every frame, rotates it, culls back faces, and shades each triangle by mapping its lighting value to an ASCII block character. The result is a spinning, shaded 3D object made entirely out of text.

## How it works

- **OBJ parsing**: `FuncLib.h` has a small parser that reads `v` (vertex) and `f` (face) lines from a `.obj` file and builds a triangle mesh, splitting quads into two triangles when needed.
- **Transform pipeline**: each triangle goes through rotation (Z then Y), translation away from the camera, and a perspective projection matrix, all using a basic `mat4x4` / `vec3` setup in `Structures.h`.
- **Back-face culling**: the triangle normal is checked against the camera's look direction with a dot product, and anything facing away gets skipped.
- **Lighting**: a fixed directional light is dotted against the triangle normal to get a luminosity value, which then picks a foreground/background color pair and a block character (quarter, half, three-quarters, solid) to approximate shading in text.
- **Output**: drawing happens through `olcConsoleGameEngine.h`, which handles the console window, frame buffer, and `FillTriangle` calls.

## Built on

This uses [olcConsoleGameEngine](https://github.com/OneLoneCoder/Javidx9) by Javidx9 for the console window and rendering primitives. The rasterization, projection, culling, and OBJ loading are custom.

## Running it

Windows only, since it depends on the Windows console API.

1. Open `srEngine3D.sln` in Visual Studio.
2. Build and run (x64, Debug or Release).
3. It loads `object.txt` (an OBJ-format file) from the working directory by default.

To render your own model, export it as an OBJ from Blender or similar, point the loader at it in `main()`, and rebuild.

## Possible next steps

- Depth buffering instead of relying purely on culling
- Texture or UV support
- Replace the console engine dependency with a custom windowing/input layer
- Camera movement instead of a fixed view

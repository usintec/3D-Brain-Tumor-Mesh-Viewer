# 3D Brain Tumor Mesh Viewer

A lightweight OpenGL-based medical visualization viewer for 3D tumor segmentation meshes. The viewer loads multiple OBJ meshes representing tumor regions and renders them with color-coded transparency, lighting, and keyboard-driven navigation.

## Features

- Loads multiple OBJ meshes: `tumor_mesh.obj`, `wt.obj`, `tc.obj`, `et.obj`
- Color-coded tumor segmentation layers:
  - `et.obj` — enhanced tumor (red)
  - `tc.obj` — tumor core (orange)
  - `wt.obj` — whole tumor (blue)
  - `tumor_mesh.obj` — reference volume boundary (grey)
- Phong-like lighting with boundary enhancement
- Keyboard controls for rotation, panning, and zoom
- Depth-aware transparent rendering order

## Keyboard Controls

- `Left` / `Right` — rotate around the vertical axis
- `Up` / `Down` — rotate around the horizontal axis
- `A` / `D` — pan left / right
- `W` / `S` — pan up / down
- `Q` — zoom in
- `E` — zoom out

## Project Structure

- `3D Brain Tumor Mesh Viewer.sln` — Visual Studio solution
- `3D Brain Tumor Mesh Viewer.vcxproj` — Visual Studio C++ project
- `src/` — source code
  - `main.cpp` — application entry point, rendering loop, input handling
  - `Camera.cpp`, `Camera.h` — camera and projection logic
  - `Mesh.cpp`, `Mesh.h` — mesh rendering code
  - `ModelLoader.cpp`, `ModelLoader.h` — OBJ loader and normal computation
  - `Renderer.cpp`, `Renderer.h` — render helpers
  - `Shader.cpp`, `Shader.h` — GLSL shader management
- `shaders/` — GLSL source files
  - `Mesh.vert` — vertex shader
  - `Mesh.frag` — fragment shader
- `assets/` — model assets and OBJ meshes

## Build Instructions

1. Open `3D Brain Tumor Mesh Viewer.sln` in Visual Studio.
2. Configure the solution for `x64` and `Release` or `Debug` mode.
3. Build the solution.
4. Run the resulting executable from `x64\Release` or `x64\Debug`.

If you are using a command line environment with MSBuild available, you can also build with a command like:

```powershell
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "3D Brain Tumor Mesh Viewer.sln" /p:Configuration=Release /p:Platform=x64
```

## Dependencies

- OpenGL 3.3 or higher
- GLFW for window and input handling
- GLAD for OpenGL function loading
- Visual Studio C++ build tools (for Windows)

## Notes

- `modelZoom` is currently implemented by scaling the model matrix rather than moving the camera or changing the projection.
- Transparent rendering uses a fixed draw order to preserve layer visibility.
- The viewer currently applies keyboard-only navigation; mouse controls are not implemented.

## Recommended Workflow

1. Place the OBJ meshes in the `assets/` folder.
2. Launch the application.
3. Use the keyboard controls to inspect tumor layers interactively.

## License

This project does not include a license file. Add one to define usage terms.

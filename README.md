# 3D Brain Tumor Mesh Viewer

A compact OpenGL-based medical visualization viewer for 3D tumor segmentation meshes.

This project loads multiple OBJ meshes representing tumor segmentation regions and renders them with transparent overlays, directional lighting, and keyboard-controlled navigation. It is intended as a lightweight viewer for inspecting tumor anatomy layers, such as enhanced tumor, tumor core, and whole tumor regions.

---

## Table of Contents

- [Features](#features)
- [Supported Meshes](#supported-meshes)
- [How It Works](#how-it-works)
- [Controls](#controls)
- [Build and Run](#build-and-run)
- [Project Structure](#project-structure)
- [Asset and Shader Paths](#asset-and-shader-paths)
- [Development Notes](#development-notes)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)

---

## Features

- Loads and displays multiple segmented tumor meshes in one scene
- Transparent rendering with depth-aware draw ordering
- Per-mesh color-coded segmentation classes
- Directional lighting and view-dependent shading
- Smooth keyboard navigation for rotation, panning, and zoom
- Automatic OBJ loading via `ModelLoader`
- Output of performance and debug evaluation metrics to console

---

## Supported Meshes

The viewer currently loads the following OBJ files from the `assets/` folder:

- `tumor_mesh.obj` — reference volume boundary
- `wt.obj` — whole tumor segmentation
- `tc.obj` — tumor core segmentation
- `et.obj` — enhanced tumor segmentation

### Render Order and Visibility

Meshes are rendered in this fixed order to preserve transparency and overlap:

1. `tumor_mesh.obj`
2. `wt.obj`
3. `tc.obj`
4. `et.obj`

This ensures the outer reference volume and whole tumor regions appear behind the more central tumor core and enhanced tumor layers.

---

## How It Works

The application performs the following main tasks:

1. Initializes GLFW and creates a window.
2. Loads OpenGL function pointers using GLAD.
3. Sets up blending, depth testing, and back-face culling.
4. Resolves shader and asset file paths relative to the repository root.
5. Loads OBJ meshes with `ModelLoader::loadOBJ(...)`.
6. Compiles GLSL shaders from `shaders/Mesh.vert` and `shaders/Mesh.frag`.
7. Configures the camera and model transform state.
8. Enters the render loop:
   - compute delta time and handle input
   - update rotation, pan, and zoom
   - compute view and projection matrices
   - render meshes with per-class shader uniforms
   - swap buffers and poll events
9. Terminates GLFW on exit.

---

## Controls

Use the keyboard to navigate the loaded meshes:

- `Left Arrow` / `Right Arrow` — rotate model around vertical axis
- `Up Arrow` / `Down Arrow` — rotate model around horizontal axis
- `A` / `D` — pan left / right
- `W` / `S` — pan up / down
- `Q` — zoom in
- `E` — zoom out

> Note: Mouse controls are not currently implemented.

---

## Build and Run

### Prerequisites

- Windows x64
- Visual Studio with C++ desktop development workload
- OpenGL-compatible GPU and drivers
- GLFW and GLAD available to the project build
- `glm` math library available in the include path

### Build using Visual Studio

1. Open `3D Brain Tumor Mesh Viewer.sln` in Visual Studio.
2. Select the `x64` platform.
3. Choose `Release` or `Debug` configuration.
4. Build the solution.
5. Run the executable from `x64\Release` or `x64\Debug`.

### Build using MSBuild

If you prefer the command line and have MSBuild installed:

```powershell
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "3D Brain Tumor Mesh Viewer.sln" /p:Configuration=Release /p:Platform=x64
```

### Running the Viewer

Run the compiled executable from the build folder. The application expects the repository's `assets/` and `shaders/` folders to remain in place because it constructs absolute paths from the source file location.

If the program cannot find shaders or OBJ files, verify that `src/main.cpp` is still compiled from the repository root and that `assets/` and `shaders/` exist alongside the source tree.

---

## Project Structure

- `3D Brain Tumor Mesh Viewer.sln` — Visual Studio solution file
- `3D Brain Tumor Mesh Viewer.vcxproj` — project file
- `assets/` — OBJ mesh assets used by the viewer
- `shaders/` — GLSL vertex and fragment shaders
- `src/` — C++ source files
  - `main.cpp` — entry point, render loop, input, loading, and scene setup
  - `Camera.cpp`, `Camera.h` — camera transform and projection logic
  - `Mesh.cpp`, `Mesh.h` — mesh data structures and draw calls
  - `ModelLoader.cpp`, `ModelLoader.h` — OBJ parsing and mesh creation
  - `Renderer.cpp`, `Renderer.h` — optional render helpers
  - `Shader.cpp`, `Shader.h` — shader compilation and uniform helpers
  - `Headers/Evaluation.h` — runtime evaluation utilities used for console feedback

---

## Asset and Shader Paths

The executable locates assets with a path derived from the compiled source file path. The code determines the repository root by stripping `\src` from `__FILE__`, then appends:

- `\shaders\Mesh.vert`
- `\shaders\Mesh.frag`
- `\assets\tumor_mesh.obj`
- `\assets\wt.obj`
- `\assets\tc.obj`
- `\assets\et.obj`

> If you move the project folder or build output, keep the source tree structure intact so the application can load the files correctly.

---

## Development Notes

- The current zoom implementation scales the model directly instead of moving the camera or changing projection parameters.
- Transparent rendering disables depth writes during mesh drawing, then re-enables depth writes afterward.
- The shader receives a `tumorClass` uniform to select per-mesh coloring and transparency behavior.
- Console output includes FPS and evaluation values generated by `Evaluation` utilities.
- The render loop does not currently support window resizing callbacks beyond updating the viewport each frame.

---

## Troubleshooting

### Common issues

- `GLFWwindow* window` is null: ensure GLFW is initialized and a valid OpenGL context is available.
- Shader load failure: verify `shaders/Mesh.vert` and `shaders/Mesh.frag` exist and are readable.
- OBJ load failure: ensure mesh files are present in `assets/` and named exactly as expected.
- Crashes when running from outside the repo: run the executable from the repository root or update the asset path logic.

### Debugging tips

- Use Visual Studio's Output window to inspect runtime errors.
- Add logging inside `ModelLoader::loadOBJ(...)` and `Shader` constructor for file load failures.
- Confirm the working directory in Visual Studio is the project folder, not the build folder.

---

## Future Improvements

- Add mouse-based camera orbit and zoom controls.
- Support drag-and-drop OBJ loading at runtime.
- Add a UI overlay for mesh visibility toggles and color selection.
- Implement file path configuration instead of hard-coded relative paths.
- Add a proper license file and documentation for contributions.

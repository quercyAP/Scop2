# 3D Object Viewer

## Overview
This 3D Object Viewer is an application designed to display and interact with 3D models in a gratifying way. Developed with C++ and OpenGL, it renders a 3D object provided by the user in a `.obj` file format, with an optional texture in `.bmp` format applied. This project emphasizes manual parsing and rendering techniques, showcasing interactive features with the 3D model.



## Features
- **3D Model Display**: Renders 3D objects in perspective, simulating depth by making distant objects appear smaller.
- **Rotation**: The object continuously rotates around its main symmetrical axis for a 360-degree view.
- **Color and Texture**: Supports color differentiation for various sides of the object. An optional texture can be applied if a `.bmp` file is provided as a second argument.
- **Movement**: The object can be moved along three axes, offering interactive exploration of the model in space.
- **Manual Parsing and Rendering**: Implements manual parsing of `.obj` files and direct application of textures and colors without relying on external libraries for these tasks.

## Technical Constraints
- **Programming Languages**: Written primarily in C++.
- **Graphics API**: OpenGL. Alternatives like Vulkan, Metal, and MinilibX are permissible but not utilized in this project.
- **Build System**: Features a classic `Makefile` for straightforward compilation.
- **External Libraries**: Limited to window and event management. Direct handling of 3D models, textures, matrices, and shaders is a project requirement.

## Project Structure
- `src/` - Contains source files.
- `include/` - Header files for the project.
- `assets/` - 3D models (.obj files) and textures (.bmp files).
- `shaders/` - Shader files for enhanced rendering effects.
- `Makefile` - Compiles and builds the project.

## Compilation and Usage
To compile the project, execute:
```
make
```
To run the application, use:

```
./scop <path_to_obj_file> [path_to_texture_file.bmp]
```
- The first argument is the path to the `.obj` file you wish to render.
- The second, optional argument is the path to a `.bmp` texture file to apply to the 3D object.
- 3DObject and Texture are store in `assets/`

## Inputs
Control the camera and object movement with the following keys:
- **Camera Movement**: Use `ZQSD` keys to move the camera around the scene.
- **Object Movement**: Use the arrow keys to move the object along the X and Y axes.
- **Object Elevation**: Use `Page Up` and `Page Down` keys to move the object up and down along the Z axis.

## Demonstration
As a proof of concept, the application can render the iconic 42 logo in 3D, rotating around its central axis. When provided, a `.bmp` texture of your choice (e.g., ponies, kittens, unicorns) can be applied, showcasing the application's flexibility and rendering capabilities.

## Dependencies
- OpenGL for rendering.
- GLFW or another library for window and event management.

## Author
Guillaume Lamazere

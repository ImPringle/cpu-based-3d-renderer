## CPU-Based 3D Renderer in C++

Soft3D is a minimal software 3D renderer written entirely in C++.
The project focuses on **graphics fundamentals**, implementing a full 3D
transformation and projection pipeline **without GPU APIs**.

All transformations are performed on the CPU using linear algebra,
with SDL used only for window creation and pixel output.

### Features
- 3D to 2D perspective projection
- Manual rotation using trigonometric functions
- Translation along the Z-axis
- Fixed timestep game loop (FPS capped)
- Software-rendered geometry (no OpenGL / Vulkan / DirectX)

### Why this project?
This project was built to deeply understand how 3D graphics work under the hood:
from vector math and transformations to projection and screen-space rendering.

It intentionally avoids high-level graphics libraries to focus on
**math, data flow, and rendering fundamentals**.

### Tech Stack
- C++
- SDL2
- Linear Algebra (vectors, transformations, projections)

### Demo
*(Add GIF or screenshot here — very important)*

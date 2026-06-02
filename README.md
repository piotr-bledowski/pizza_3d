# pizza_3d
An interactive pizza model in OpenGL

## Quick code map (offline-friendly)

- `main.cpp` - app entry point and frame orchestration (UI -> animation -> scene sync -> render).
- `Renderer.cpp` - OpenGL setup, camera application, hovered-slice detection, and drawing.
- `Input.cpp` - keyboard/mouse handling and mode switching (`Camera` vs `UI`).
- `Camera.cpp` - first-person camera movement and `gluLookAt` view setup.
- `Topping/ToppingManager.cpp` - spawning/removing toppings, sauce layer logic, bake/slice sync.
- `UI/UI.cpp` - immediate-mode overlay UI (buttons, labels, click routing).
- `Scene/SceneManager.cpp` - owns base scene objects and provides them for final scene composition.

# MathCore

**MathCore** is a lightweight C++ mathematics library focused on 2D and 3D mathematical operations, spatial geometry, and collision detection.

Designed with simplicity and modularity in mind, MathCore provides essential mathematical tools for graphics programming, game development, and other computational applications.

## Features

* **Vec2** — 2D vector operations and mathematical utilities.
* **Vec3** — 3D vector arithmetic, dot products, cross products, and normalization.
* **Rotation** — Euler angle rotations and directional vector calculations.
* **AABB** — Axis-Aligned Bounding Boxes for spatial queries and intersection testing.
* **OBB** — Oriented Bounding Boxes with rotation support.
* **Collision Detection** — SAT-based collision detection for oriented bounding boxes.
* **Debug Utilities** — Basic vector and collision information for debugging.

## Requirements

* C++17 or later
* A compatible C++ compiler

## Installation

MathCore is a header-based library.

Include the library header in your project:

```cpp
#include "math.h"
```

No external dependencies are required.

## Example

```cpp
#include "math.h"

int main()
{
    Vec3 a(1, 2, 3);
    Vec3 b(4, 5, 6);

    Vec3 result = a + b;

    OBB boxA({0, 0, 0}, {2, 2, 2});
    OBB boxB({1, 0, 0}, {2, 2, 2});

    CollisionInfo collision = checkCollision(boxA, boxB);

    return 0;
}
```

## Project Status

MathCore is an early-stage project under active development.

Features, interfaces, and internal implementations may change as development progresses.

## License

See the [LICENSE](LICENSE) file for licensing information.

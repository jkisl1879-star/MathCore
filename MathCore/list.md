# MathCore list command and on update

* `constexpr float PI`

* `float rad(float d)`

* `Vec2()`

* `Vec2(float x, float y)`

* `Vec2 operator+(Vec2 v) const`

* `Vec2 operator-(Vec2 v) const`

* `Vec2 operator*(float s) const`

* `float length() const`

* `Vec3()`

* `Vec3(float x, float y, float z)`

* `Vec3 operator+(Vec3 v) const`

* `Vec3 operator-(Vec3 v) const`

* `Vec3 operator-() const`

* `Vec3 operator*(float s) const`

* `Vec3 operator/(float s) const`

* `float dot(Vec3 v) const`

* `Vec3 cross(Vec3 v) const`

* `float length() const`

* `Vec3 normalized() const`

* `Rotation()`

* `Rotation(float x, float y, float z)`

* `Vec3 rotate(Vec3 v) const`

* `Vec3 right() const`

* `Vec3 up() const`

* `Vec3 forward() const`

* `AABB()`

* `AABB(Vec3 a, Vec3 b)`

* `static AABB fromCenter(Vec3 p, Vec3 s)`

* `Vec3 center() const`

* `Vec3 size() const`

* `bool contains(Vec3 p) const`

* `bool intersects(const AABB& b) const`

* `OBB()`

* `OBB(Vec3 p, Vec3 s, Rotation r = {})`

* `Vec3 axis(int i) const`

* `Vec3 corner(int i) const`

* `AABB bounds() const`

* `bool hit`

* `bool touching`

* `Vec3 point`

* `Vec3 normal`

* `Vec3 overlap`

* `float depth`

* `std::string axis`

* `CollisionInfo checkCollision(const OBB& a, const OBB& b)`

* `CollisionInfo checkCollision(const AABB& a, const AABB& b)`

* `std::string debugVec(Vec3 v)`

* `std::string debugCollision(const OBB& a, const OBB& b)`

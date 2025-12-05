#ifndef extra_math_h
#define extra_math_h

#include "main.h"

using namespace std;


constexpr float pi = 3.1415926f;

static float degrees_to_radians(float degrees) {
    return degrees * (pi / 180.0f);
}

static float radians_to_degrees(float radians) {
    return radians * (180.0f / pi);
}

/// <summary>
/// This function clamps a value between a minimum and maximum value.
/// </summary>
/// <param name="value">The value to clamp</param>
/// <param name="Min">The minimum allowed value</param>
/// <param name="Max">The maximum allowed value</param>
/// <returns>The clamped value</returns>
static float clamp(float value, float Min, float Max) {
    if (value < Min) value = Min;
    if (value > Max) value = Max;
    return value;
}

static vector<float> parse_string_to_vector_float(string s, int expected_count) {
    // Check for extra commas
    if (count_reoccurrence(s, ',') != expected_count - 1) {
        LOG_ERROR("parseVectorString: invalid number of commas.");
    }

    if (count_reoccurrence(s, '(') > 1) {
        LOG_ERROR("parseVectorString: too many open brackets.");
    }

    if (count_reoccurrence(s, ')') > 1) {
        LOG_ERROR("parseVectorString: too many close brackets.");
    }

    // Trim parentheses if present
    unsigned long long start_bracket_index = (s.find_first_of('(') != string::npos) ? s.find_first_of('(') + 1 : 0;

    unsigned long long end_bracket_index = (s.find_first_of(')') != string::npos) ? s.find_first_of(')') : s.size();

    string parsed_string = s.substr(static_cast<size_t>(start_bracket_index), static_cast<size_t>(end_bracket_index) - static_cast<size_t>(start_bracket_index));

    // Split by comma
    vector<float> values;
    size_t start = 0;
    while (true) {
        size_t pos = parsed_string.find(',', start);
        string token = parsed_string.substr(start, pos - start);

        // Trim spaces
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());

        try {
            values.push_back(stof(token));
        }
        catch (...) {
            LOG_ERROR("parseVectorString: invalid numeric value.");
        }

        if (pos == string::npos) break;
        start = pos + 1;
    }

    if ((int)values.size() != expected_count) {
        LOG_ERROR("parseVectorString: wrong number of components.");
    }

    return values;
}

/// <summary>
/// Generates a random integer between specified minimum and maximum values (inclusive).
/// </summary>
/// <param name="Min">The minimum value (inclusive)</param>
/// <param name="Max">The maximum value (inclusive)</param>
/// <returns>A random integer within the specified range</returns>
static int RandomNumber(int Min, int Max) {
    static random_device Dev;
    static mt19937 Rng(Dev());
    uniform_int_distribution<mt19937::result_type> Dist(Min, Max);

    return Dist(Rng);
}

/// <summary>
/// Converts an angle from degrees to radians.
/// </summary>
/// <param name="deg">The angle in degrees to convert.</param>
/// <returns>The angle in radians.</returns>
static inline float DegToRad(float deg) {
    return deg * (pi / 180.0f);
}

class LERP {

public:

    /// <summary>
    /// Performs linear interpolation between two values.
    /// </summary>
    /// <param name="Start">The starting value</param>
    /// <param name="End">The ending value</param>
    /// <param name="Alpha">The interpolation factor (0.0 to 1.0)</param>
    /// <returns>The interpolated value</returns>
    static float Lerp(float Start, float End, float Alpha) {
        return Start + (End - Start) * clamp(Alpha, 0.0f, 1.0f);
    }

    /// <summary>
    /// Ping-pongs between Start and End values based on Alpha (0-1 = one full cycle)
    /// </summary>
    /// <param name="Start">First target value</param>
    /// <param name="End">Second target value</param>
    /// <param name="Alpha">Progress (0-1 = one full cycle, continues beyond)</param>
    /// <returns>Value smoothly moving between Start and End</returns>
    static float PingPong(float Start, float End, float Alpha)
    {
        // Normalize Alpha to 0-2 range for a full cycle
        float NormalisedAlpha = fmod(Alpha, 2.0f);

        // First half of cycle: lerp from Start to End
        if (NormalisedAlpha <= 1.0f) {
            return Lerp(Start, End, NormalisedAlpha);
        }
        // Second half: lerp back from End to Start
        else {
            return Lerp(End, Start, NormalisedAlpha - 1.0f);
        }
    }
};



struct Vec2 {
    float x, y;

    Vec2() : x(0.0f), y(0.0f) {}

    Vec2(float x, float y) : x(x), y(y) {}

    Vec2(Vec2 const& value) : x(value.x), y(value.y) {}

    Vec2 operator+(Vec2 const& other) const {
        return { x + other.x, y + other.y };
    }

    Vec2 operator+(float const& other) const {
        return { x + other, y + other };
    }

    Vec2 operator-(Vec2 const& other) const {
        return { x - other.x, y - other.y };
    }

    Vec2 operator-(float const& other) const {
        return { x - other, y - other };
    }

    Vec2 operator-() const { return { -x, -y }; }

    Vec2 operator/(Vec2 const& other) const {
        if (other.x == 0.0f || other.y == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / other.x, y / other.y };
    }

    Vec2 operator/(float const& other) const {
        if (other == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / other, y / other };
    }

    Vec2 operator*(Vec2 const& other) const {
        return { x * other.x, y * other.y };
    }

    Vec2 operator*(float const& other) const {
        return { x * other, y * other };
    }

    Vec2& operator=(Vec2 const& other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    bool operator==(Vec2 const& other) const {
        return x == other.x && y == other.y;
    }

    bool operator==(float const& other) const {
        return x == other && y == other;
    }

    bool operator!=(Vec2 const& other) const {
        return x != other.x || y != other.y;
    }

    bool operator!=(float const& other) const {
        return x != other || y != other;
    }

    void operator+=(Vec2 const& other) {
        x += other.x;
        y += other.y;
    }

    void operator+=(float const& other) {
        x += other;
        y += other;
    }

    void operator-=(Vec2 const& other) {
        x -= other.x;
        y -= other.y;
    }

    void operator-=(float const& other) {
        x -= other;
        y -= other;
    }

    void operator/=(Vec2 const& other) {
        if (other.x == 0.0f || other.y == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        x /= other.x;
        y /= other.y;
    }

    void operator/=(float const& other) {
        if (other == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        x /= other;
        y /= other;
    }

    void operator*=(Vec2 const& other) {
        x *= other.x;
        y *= other.y;
    }

    void operator*=(float const& other) {
        x *= other;
        y *= other;
    }

    float& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        LOG_ERROR("Vec2 index out of range");
    }

    const float& operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        LOG_ERROR("Vec2 index out of range");
    }

    float Magnitude() const { return sqrt(x * x + y * y); }

    Vec2 Normalize() const {
        float mag = Magnitude();
        if (mag == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / mag, y / mag };
    }

    Vec2 Normalised() {
        float mag = Magnitude();
        if (mag == 0.0f) {
            LOG_ERROR("Division by zero");
        }

        x /= mag;
        y /= mag;

        return {x, y};
    }

    float Distance(Vec2 const& other) const {
        Vec2 diff = *this - other;
        return sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    static float Distance(Vec2 pointA, Vec2 pointB) {
        Vec2 diff = pointA - pointB;
        return sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    float Dot(Vec2 const& other) const { return x * other.x + y * other.y; }

    static float Dot(Vec2 const& pointA, Vec2 const& pointB) {
        return pointA.x * pointB.x + pointA.y * pointB.y;
    }

    float Cross(Vec2 const& other) {
        return (x * other.y) - (y * other.x);
    }

    static float Cross(Vec2 const& a, Vec2 const& b) {
        return (a.x * b.y) - (a.y * b.x);
    }

    static Vec2 rotate2d(Vec2 const& point, float angleRadians) {
        float cosAngle = cosf(angleRadians);
        float sinAngle = sinf(angleRadians);
        return {
            point.x * cosAngle - point.y * sinAngle,
            point.x * sinAngle + point.y * cosAngle
        };
    }

    friend ostream& operator<<(ostream& os, const Vec2& value) {
        os << "Vector2(" << value.x << ", " << value.y << ")";
        return os;
    }

    string tostr() const {
        return "Vector2(" + to_string(x) + ", " + to_string(y) + ")";
    }

    string operator+(const string& _string) const { return _string + tostr(); }

    static Vec2 Zero() { return { 0, 0 }; }
    static Vec2 One() { return { 1, 1 }; }

    static Vec2 Right() { return { 1, 0 }; }
    static Vec2 Left() { return { -1, 0 }; }

    static Vec2 Up() { return { 0, 1 }; }
    static Vec2 Down() { return { 0, -1 }; }

    static Vec2 stov2(string s) {
        vector<float> v = parse_string_to_vector_float(s, 2);
        return { v[0], v[1] };
    }
};

struct Vec3 {
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3(Vec3 const& value) : x(value.x), y(value.y), z(value.z) {}

    Vec3(Vec2 const& value) : x(value.x), y(value.y), z(0.0f) {}

    Vec3 operator+(Vec3 const& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vec3 operator+(float const& other) const {
        return { x + other, y + other, z + other };
    }

    Vec3 operator-(Vec3 const& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vec3 operator-(float const& other) const {
        return { x - other, y - other, z - other };
    }

    Vec3 operator-() const { return { -x, -y, -z }; }

    Vec3 operator/(Vec3 const& other) const {
        if (other.x == 0.0f || other.y == 0.0f || other.z == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / other.x, y / other.y, z / other.z };
    }

    Vec3 operator/(float const& other) const {
        if (other == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / other, y / other, z / other };
    }

    Vec3 operator*(Vec3 const& other) const {
        return { x * other.x, y * other.y, z * other.z };
    }

    Vec3 operator*(float const& other) const {
        return { x * other, y * other, z * other };
    }

    Vec3& operator=(Vec3 const& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vec3& operator=(Vec2 const& other) {
        x = other.x;
        y = other.y;
        z = 0.0f;
        return *this;
    }

    bool operator==(Vec3 const& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator==(float const& other) const {
        return x == other && y == other && z == other;
    }

    bool operator!=(Vec3 const& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    bool operator!=(float const& other) const {
        return x != other || y != other || z != other;
    }

    void operator+=(Vec3 const& other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void operator+=(float const& other) {
        x += other;
        y += other;
        z += other;
    }

    void operator-=(Vec3 const& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    void operator-=(float const& other) {
        x -= other;
        y -= other;
        z -= other;
    }

    void operator/=(Vec3 const& other) {
        if (other.x == 0 || other.y == 0 || other.z == 0) {
            LOG_ERROR("Division by zero");
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }

    void operator/=(float const& other) {
        if (other== 0.0f) {
            LOG_ERROR("Division by zero");
        }
        x /= other;
        y /= other;
        z /= other;
    }

    void operator*=(Vec3 const& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    void operator*=(float const& other) {
        x *= other;
        y *= other;
        z *= other;
    }

    float& operator[](int i) {
        if (i== 0.0f) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        LOG_ERROR("Vec3 index out of range");
    }

    const float& operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        LOG_ERROR("Vec3 index out of range");
    }

    float Magnitude() const { return sqrt(x * x + y * y + z * z); }

    Vec3 Normalize() const {
        float mag = Magnitude();
        if (mag == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / mag, y / mag, z / mag };
    }

    Vec3 Normalised() {
        float mag = Magnitude();
        if (mag == 0.0f) {
            LOG_ERROR("Division by zero");
        }

        x /= mag;
        y /= mag;
        z /= mag;

        return { x, y, z };
    }

    float Distance(Vec3 const& other) const {
        Vec3 diff = *this - other;
        return sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    }

    static float Distance(Vec3 pointA, Vec3 pointB) {
        Vec3 diff = pointA - pointB;
        return sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    }

    float Dot(Vec3 const& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    static float Dot(Vec3 const& pointA, Vec3 const& pointB) {
        return pointA.x * pointB.x + pointA.y * pointB.y + pointA.z * pointB.z;
    }

    Vec3 Cross(Vec3 const& other) {
        return {
            (y * other.z) - (z * other.y),
            (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x)
        };
    }

    static Vec3 Cross(Vec3 const& a, Vec3 const& b) {
        return {
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        };
    }

    static Vec3 rotate3d(Vec3 const& point, Vec3 const& axis, float angleRadians) {
        Vec3 axisNormalized = axis.Normalize();

        float cosAngle = cosf(angleRadians);
        float sinAngle = sinf(angleRadians);

        Vec3 term1 = axisNormalized * cosAngle;
        Vec3 term2 = Cross(axisNormalized, point) * sinAngle;
        Vec3 term3 = axisNormalized * Dot(axisNormalized, point) * (1.0f - cosAngle);

        return term1 + term2 + term3;
    }

    friend ostream& operator<<(ostream& os, const Vec3& value) {
        os << "Vector3(" << value.x << ", " << value.y << ", " << value.z << ")";
        return os;
    }

    string tostr() const {
        return "Vector3(" + to_string(x) + ", " + to_string(y) + ", " +
            to_string(z) + ")";
    }

    string operator+(string _string) const { return _string + tostr(); }

    static Vec3 Zero() { return { 0, 0, 0 }; }
    static Vec3 One() { return { 1, 1, 1 }; }

    static Vec3 Right() { return { 1, 0, 0 }; }
    static Vec3 Left() { return { -1, 0, 0 }; }

    static Vec3 Up() { return { 0, 1, 0 }; }
    static Vec3 Down() { return { 0, -1, 0 }; }

    static Vec3 Forward() { return { 0, 0, 1 }; }
    static Vec3 Backward() { return { 0, 0, -1 }; }

    static Vec3 stov3(string s) {
        vector<float> v = parse_string_to_vector_float(s, 3);
        return { v[0], v[1], v[2] };
    }
};

struct Vec4 {
    float x, y, z, w;

    Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vec4(Vec4 const& value) : x(value.x), y(value.y), z(value.z), w(value.w) {}

    Vec4(Vec2 const& value) : x(value.x), y(value.y), z(0.0f), w(0.0f) {}

    Vec4(Vec3 const& value) : x(value.x), y(value.y), z(value.z), w(0.0f) {}

    Vec4 operator+(Vec4 const& other) const {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    Vec4 operator+(float const& other) const {
        return { x + other, y + other, z + other, w + other };
    }

    Vec4 operator-(Vec4 const& other) const {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    Vec4 operator-(float const& other) const {
        return { x - other, y - other, z - other, w - other };
    }

    Vec4 operator-() const { return { -x, -y, -z, -w }; }

    Vec4 operator/(Vec4 const& other) const {
        if (other.x == 0.0f || other.y == 0.0f || other.z == 0.0f || other.w == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / other.x, y / other.y, z / other.z, w / other.w };
    }

    Vec4 operator/(float const& other) const {
        if (other == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / other, y / other, z / other, w / other };
    }

    Vec4 operator*(Vec4 const& other) const {
        return { x * other.x, y * other.y, z * other.z, w * other.w };
    }

    Vec4 operator*(float const& other) const {
        return { x * other, y * other, z * other, w * other };
    }

    Vec4& operator=(Vec4 const& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    Vec4& operator=(Vec2 const& other) {
        x = other.x;
        y = other.y;
        z = 0.0f;
        w = 0.0f;
        return *this;
    }

    Vec4& operator=(Vec3 const& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = 0.0f;
        return *this;
    }

    bool operator==(Vec4 const& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator==(float const& other) const {
        return x == other && y == other && z == other && w == other;
    }

    bool operator!=(Vec4 const& other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    bool operator!=(float const& other) const {
        return x != other || y != other || z != other || w != other;
    }

    void operator+=(Vec4 const& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    void operator+=(float const& other) {
        x += other;
        y += other;
        z += other;
        w += other;
    }

    void operator-=(Vec4 const& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    void operator-=(float const& other) {
        x -= other;
        y -= other;
        z -= other;
        w -= other;
    }

    void operator/=(Vec4 const& other) {
        if (other.x == 0.0f || other.y == 0.0f || other.z == 0.0f || other.w == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
    }

    void operator/=(float const& other) {
        if (other == 0) {
            LOG_ERROR("Division by zero");
        }
        x /= other;
        y /= other;
        z /= other;
        w /= other;
    }

    void operator*=(Vec4 const& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
    }

    void operator*=(float const& other) {
        x *= other;
        y *= other;
        z *= other;
        w *= other;
    }

    float& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;
        LOG_ERROR("Vec4 index out of range");
    }

    const float& operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;
        LOG_ERROR("Vec4 index out of range");
    }

    float Magnitude() const { return sqrt(x * x + y * y + z * z + w * w); }

    Vec4 Normalize() const {
        float mag = Magnitude();
        if (mag == 0.0f) {
            LOG_ERROR("Division by zero");
        }
        return { x / mag, y / mag, z / mag, w / mag };
    }

    Vec4 Normalised() {
        float mag = Magnitude();
        if (mag == 0.0f) {
            LOG_ERROR("Division by zero");
        }

        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;

        return { x, y, z, w };
    }

    float Distance(Vec4 const& other) const {
        Vec4 diff = *this - other;
        return sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z +
            diff.w * diff.w);
    }

    static float Distance(Vec4 pointA, Vec4 pointB) {
        Vec4 diff = pointA - pointB;
        return sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z +
            diff.w * diff.w);
    }

    float Dot(Vec4 const& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    static float Dot(Vec4 const& pointA, Vec4 const& pointB) {
        return pointA.x * pointB.x + pointA.y * pointB.y + pointA.z * pointB.z +
            pointA.w * pointB.w;
    }

    friend ostream& operator<<(ostream& os, const Vec4& value) {
        os << "Vector4(" << value.x << ", " << value.y << ", " << value.z << ", "
            << value.w << ")";
        return os;
    }

    string tostr() const {
        return "Vector4(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ", " + to_string(w) + ")";
    }

    string operator+(string _string) const { return _string + tostr(); }

    static Vec4 Zero() { return { 0, 0, 0, 1 }; }
    static Vec4 One() { return { 1, 1, 1, 1 }; }

    static Vec4 Right() { return { 1, 0, 0, 0 }; }
    static Vec4 Left() { return { -1, 0, 0, 0 }; }

    static Vec4 Up() { return { 0, 1, 0, 0 }; }
    static Vec4 Down() { return { 0, -1, 0, 0 }; }

    static Vec4 Forward() { return { 0, 0, 1, 0 }; }
    static Vec4 Backward() { return { 0, 0, -1, 0 }; }

    static Vec4 stov4(string s) {
        vector<float> v = parse_string_to_vector_float(s, 4);
        return { v[0], v[1], v[2], v[3] };
    }
};

#endif // !extra_math.h
#ifndef GODOT_VARIANT_H
#define GODOT_VARIANT_H

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct {
    int type;
    union {
        int i;
        float f;
        char s[256];
        int b;
        Vector2 vec2;
        Vector3 vec3;
        Color color;
    } value;
} GDExtensionVariant;

typedef enum {
    VARTYPE_NULL = 0,
    VARTYPE_BOOL = 1,
    VARTYPE_INT = 2,
    VARTYPE_FLOAT = 3,
    VARTYPE_STRING = 4,
    VARTYPE_VECTOR2 = 5,
    VARTYPE_VECTOR3 = 6,
    VARTYPE_COLOR = 7
} GDExtensionVarType;

#endif

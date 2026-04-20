// GDTinyCC 3D demo - floating CSGSphere3Ds with random colors
#include "stddef.h"
#include "gdtinycc_runtime.h"

#define NUM_SPHERES 100

typedef struct {
    void* node;
    float speed_x;
    float speed_y;
    float speed_z;
    float offset;
} SphereData;

static SphereData spheres[NUM_SPHERES];
static int sphere_count = 0;
static float time_total = 0.0f;

void main() {
    godot_print("demo3d: floating CSGSphere3Ds");
}

void _ready(void* self) {
    godot_print("demo3d: _ready called");
    godot_randomize();

    void* parent = godot_get_node(self, "/root/Main");
    if (!parent) {
        parent = self;
    }

    for (int i = 0; i < NUM_SPHERES; i++) {
        void* sphere = godot_create("CSGSphere3D");
        if (!sphere) {
            godot_print("failed to create CSGSphere3D %d", i);
            continue;
        }

        GDExtensionVariant v;

        v.type = VARTYPE_VECTOR3;
        v.value.vec3.x = -10.0f + (godot_randf() * 20.0f);
        v.value.vec3.y = -5.0f + (godot_randf() * 10.0f);
        v.value.vec3.z = -5.0f + (godot_randf() * 10.0f);
        godot_set_variant(sphere, "position", v);

        v.type = VARTYPE_FLOAT;
        v.value.f = 0.5f + (godot_randf() * 1.0f);
        godot_set_variant(sphere, "radius", v);

        void* material = godot_create("StandardMaterial3D");
        if (material) {
            GDExtensionVariant mat_v;
            mat_v.type = VARTYPE_COLOR;
            mat_v.value.color.r = godot_randf();
            mat_v.value.color.g = godot_randf();
            mat_v.value.color.b = godot_randf();
            mat_v.value.color.a = 1.0f;
            godot_set_variant(material, "albedo_color", mat_v);

            GDExtensionVariant mat_var;
            mat_var.type = VARTYPE_OBJECT;
            mat_var.ptr = material;
            godot_set_variant(sphere, "material_override", mat_var);
        }

        spheres[i].node = sphere;
        spheres[i].speed_x = (godot_randf() - 0.5f) * 2.0f;
        spheres[i].speed_y = (godot_randf() - 0.5f) * 2.0f;
        spheres[i].speed_z = (godot_randf() - 0.5f) * 2.0f;
        spheres[i].offset = godot_randf() * 6.28f;

        godot_add_child_deferred(parent, sphere);
        sphere_count++;
    }

    void* light = godot_create("DirectionalLight3D");
    if (light) {
        GDExtensionVariant v;
        v.type = VARTYPE_VECTOR3;
        v.value.vec3.x = -5.0f;
        v.value.vec3.y = 5.0f;
        v.value.vec3.z = 5.0f;
        godot_set_variant(light, "position", v);

        v.type = VARTYPE_VECTOR3;
        v.value.vec3.x = -1.0f;
        v.value.vec3.y = -1.0f;
        v.value.vec3.z = -1.0f;
        v.value.vec3.x = -1.0f;
        godot_set_variant(light, "light_direction", v);

        GDExtensionVariant rot;
        rot.type = VARTYPE_VECTOR3;
        rot.value.vec3.x = 45.0f;
        rot.value.vec3.y = 45.0f;
        rot.value.vec3.z = 0.0f;
        godot_set_variant(light, "rotation", rot);

        godot_add_child_deferred(parent, light);
        godot_print("added DirectionalLight3D");
    }

    void* camera = godot_create("Camera3D");
    if (camera) {
        GDExtensionVariant v;
        v.type = VARTYPE_VECTOR3;
        v.value.vec3.x = 0.0f;
        v.value.vec3.y = 0.0f;
        v.value.vec3.z = 12.0f;
        godot_set_variant(camera, "position", v);
        godot_add_child_deferred(parent, camera);
        godot_print("added Camera3D");
    }

    godot_print("demo3d: created %d spheres", sphere_count);

    // test godot_get_rendering_server
    GDExtensionVariant args[1];
    GDExtensionVariant ret;
    void* rs = godot_get_rendering_server();
    args[0].type = VARTYPE_STRING;
    args[0].type = VARTYPE_COLOR;
    args[0].value.color.r = 0.0f;
    args[0].value.color.g = 0.0f;
    args[0].value.color.b = 0.0f;
    args[0].value.color.a = 1.0f;
    godot_call(rs, "set_default_clear_color", 1, args, &ret);
}

void _process(void* self, double delta) {
    time_total += delta;

    for (int i = 0; i < sphere_count; i++) {
        if (!spheres[i].node) continue;

        GDExtensionVariant v;
        godot_get_variant(spheres[i].node, "position", &v);

        float t = time_total + spheres[i].offset;

        v.value.vec3.x += spheres[i].speed_x * delta;
        v.value.vec3.y += spheres[i].speed_y * delta;
        v.value.vec3.z += spheres[i].speed_z * delta;

        v.value.vec3.y += sin(t * 2.0f) * delta * 0.5f;

        if (v.value.vec3.y > 8.0f) v.value.vec3.y = 8.0f;
        if (v.value.vec3.y < -8.0f) v.value.vec3.y = -8.0f;
        if (v.value.vec3.x > 12.0f) v.value.vec3.x = -12.0f;
        if (v.value.vec3.x < -12.0f) v.value.vec3.x = 12.0f;
        if (v.value.vec3.z > 8.0f) v.value.vec3.z = -8.0f;
        if (v.value.vec3.z < -8.0f) v.value.vec3.z = 8.0f;

        godot_set_variant(spheres[i].node, "position", v);
    }
}

void _input(void* self, void* event) {
    if (!event) return;
    if (godot_is_pressed(event)) {
        if (godot_eventcode(event) == KEY_ESCAPE) {
            GDExtensionVariant result;
            void* tree = godot_get_tree(self);
            godot_call(tree, "quit", 0, NULL, &result);
        }
    }
}
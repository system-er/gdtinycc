#include "gdtinycc.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/sprite3d.hpp>
#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/image.hpp>


#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <cstring>
#include <cstdio>

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

extern "C" {
    #include "tinycc-mob/libtcc.h"
}

#include "gdtinycc.h"

typedef struct TCCState TCCState;
godot::GDTinyCC* godot::GDTinyCC::_current_instance = nullptr;

void tcc_error_callback(void *opaque, const char *msg);
void godot_print(const char *msg);
void* godot_get_node(const char *path);
const char* godot_get_property(void* node, const char *property);
void godot_print_float(float f);

GDExtensionVariant godot_get_variant(void* node, const char *property);
void godot_set_variant(void* node, const char *property, GDExtensionVariant variant);
void* godot_instantiate(const char* scene_path);
void* godot_create(const char* class_name);
void godot_add_child(void* parent, void* child);
void godot_add_child_deferred(void* parent, void* child);
GDExtensionVariant godot_call(void* node_ptr, const char* method_name, int arg_count, GDExtensionVariant* args);
void godot_queue_free(void* node_ptr);
const char* godot_get_type_name(int type);
void godot_emit_signal(void* node_ptr, const char* signal_name, int arg_count, GDExtensionVariant* args);
long long godot_get_ticks_msec();
void godot_print_int(int value);
void godot_connect(void* node_ptr, const char* signal_name, void* callback_func, void* user_data);


using namespace godot;

void GDTinyCC::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_source_file", "path"), &GDTinyCC::set_source_file);
    ClassDB::bind_method(D_METHOD("get_source_file"), &GDTinyCC::get_source_file);
    ClassDB::bind_method(D_METHOD("compile_file"), &GDTinyCC::compile_file);

    ClassDB::add_property("GDTinyCC", PropertyInfo(Variant::STRING, "source_file", PROPERTY_HINT_FILE, "*.c"), "set_source_file", "get_source_file");
}


GDTinyCC::GDTinyCC() {
    _current_instance = this;
    tcc_state = nullptr;
    UtilityFunctions::print("GDTinyCC started.");
    set_process_input(true);
}

GDTinyCC::~GDTinyCC() {
    if (_current_instance == this) {
        _current_instance = nullptr;
    }
}


void GDTinyCC::_ready() {
    compile_file();
    
    if (tcc_state) {
        using ReadyFunc = void(*)();
        ReadyFunc ready_func = (ReadyFunc)tcc_get_symbol((TCCState*)tcc_state, "_ready");
        if (ready_func) {
            ready_func();
        }
    }
}

void GDTinyCC::_process(double delta) {
   if (tcc_state) {
        using ProcessFunc = void(*)(double);
        ProcessFunc process_func = (ProcessFunc)tcc_get_symbol((TCCState*)tcc_state, "_process");
        if (process_func) {
            process_func(delta);
        }
    }
}

void GDTinyCC::_physics_process(double delta) {
   if (tcc_state) {
        using ProcessFunc = void(*)(double);
        ProcessFunc pprocess_func = (ProcessFunc)tcc_get_symbol((TCCState*)tcc_state, "_physics_process");
        if (pprocess_func) {
            pprocess_func(delta);
        }
    }
}

void GDTinyCC::_input(const Ref<InputEvent> &event) {
    if (tcc_state) {
        using InputFunc = void(*)(void*);
        InputFunc input_func = (InputFunc)tcc_get_symbol((TCCState*)tcc_state, "_input");

        if (input_func) {
            input_func((void*)event.ptr());
        }
    }
}


void GDTinyCC::set_source_file(const String &p_path) {
    source_file = p_path;
}

String GDTinyCC::get_source_file() const {
    return source_file;
}

void GDTinyCC::compile_file() {
    if (source_file.is_empty()) {
        UtilityFunctions::print("error: compile_file - no sourcefile!");
        return;
    }

    char dll_path[1024];
    
#ifdef _WIN32
    HMODULE hModule;
    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&godot_print, &hModule)) {
        UtilityFunctions::print("error: compile_file - no dll handle");
        return;
    }
    GetModuleFileNameA(hModule, dll_path, sizeof(dll_path));
    char *p = strrchr(dll_path, '\\');
#else
    Dl_info info;
    if (dladdr((void*)godot_print, &info) == 0) {
        UtilityFunctions::print("error: compile_file - no so handle");
        return;
    }
    strncpy(dll_path, info.dli_fname, sizeof(dll_path) - 1);
    char *p = strrchr(dll_path, '/');
#endif
    if (p) *p = '\0';

    TCCState *s = tcc_new();
    if (!s) return;

    tcc_set_error_func(s, nullptr, tcc_error_callback);
    tcc_set_lib_path(s, dll_path);
    
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    
    tcc_add_include_path(s, dll_path);
    tcc_add_sysinclude_path(s, dll_path);
    
    String include_path = String(dll_path) + PATH_SEPARATOR + "include";
    tcc_add_include_path(s, include_path.utf8().get_data());
    tcc_add_sysinclude_path(s, include_path.utf8().get_data());
    
    String tinycc_include = String(dll_path) + PATH_SEPARATOR ".." PATH_SEPARATOR ".." PATH_SEPARATOR "src" PATH_SEPARATOR "tinycc-mob" PATH_SEPARATOR "win32" PATH_SEPARATOR "include";
    tcc_add_sysinclude_path(s, tinycc_include.utf8().get_data());
    
    tcc_add_library_path(s, dll_path);
    tcc_add_symbol(s, "godot_print", (void*)godot_print);
    tcc_add_symbol(s, "godot_get_node", (void*)godot_get_node);
    tcc_add_symbol(s, "godot_get_property", (void*)godot_get_property);
    tcc_add_symbol(s, "godot_get_variant", (void*)godot_get_variant);
    tcc_add_symbol(s, "godot_set_variant", (void*)godot_set_variant);
    tcc_add_symbol(s, "godot_instantiate", (void*)godot_instantiate);
    tcc_add_symbol(s, "godot_create", (void*)godot_create);
    tcc_add_symbol(s, "godot_add_child", (void*)godot_add_child);
    tcc_add_symbol(s, "godot_add_child_deferred", (void*)godot_add_child_deferred);
    tcc_add_symbol(s, "godot_call", (void*)godot_call);
    tcc_add_symbol(s, "godot_queue_free", (void*)godot_queue_free);
    tcc_add_symbol(s, "godot_get_type_name", (void*)godot_get_type_name);
    tcc_add_symbol(s, "godot_get_ticks_msec", (void*)godot_get_ticks_msec);
    tcc_add_symbol(s, "godot_emit_signal", (void*)godot_emit_signal);
    tcc_add_symbol(s, "godot_connect", (void*)godot_connect);

    String libtcc1_path = String(dll_path) + PATH_SEPARATOR "lib" PATH_SEPARATOR "libtcc1.a";
    if (tcc_add_file(s, libtcc1_path.utf8().get_data()) < 0) {
        UtilityFunctions::print("error: compile_file - could not load libtcc1.a");
    }
 
    String full_path = source_file;
    if (!source_file.begins_with("res://")) {
        full_path = "res://" + source_file;
    }
    
    Ref<FileAccess> fa = FileAccess::open(full_path, FileAccess::READ);
    if (fa.is_null()) {
        UtilityFunctions::print("error: compile_file - file open error: ", full_path);
        tcc_delete(s);
        return;
    }
    
    String source_code = fa->get_as_text();

    UtilityFunctions::print("compile: ", full_path);
    
    if (tcc_compile_string(s, source_code.utf8().get_data()) < 0) {
        UtilityFunctions::print("error: compile_file - compileerror!");
        tcc_delete(s);
        return;
    }

    if (tcc_relocate(s) < 0) {
        UtilityFunctions::print("error: compile_file - relocationerror!");
        tcc_delete(s);
        return;
    }

    tcc_state = s;

    using MainFunc = void(*)();
    MainFunc main_func = (MainFunc)tcc_get_symbol(s, "main");

    if (main_func) {
        main_func();
    } else {
        UtilityFunctions::print("error: compile_file - main-function not found!");
    }
}

void tcc_error_callback(void *opaque, const char *msg) {
    UtilityFunctions::print("TCC Error: ", msg);
}

void godot_print(const char *msg) {
    UtilityFunctions::print(msg);
}

void* godot_get_node(const char* path) {
    if (GDTinyCC::_current_instance) {
        godot::NodePath node_path(path);
        godot::Node* node = GDTinyCC::_current_instance->get_node<godot::Node>(node_path);
        if (!node){
            UtilityFunctions::print("error: godot_get_node - node not found ");
        }
        return static_cast<void*>(node);
    } else {
        UtilityFunctions::print("error: godot_get_node - no current instance found");
        return nullptr;
    }
}

const char* godot_get_property(void* node_ptr, const char* property) {
    static char buffer[256] = "";
    if (!node_ptr) {
        buffer[0] = '\0';
        return buffer;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::Variant value = node->get(property);
    
    switch (value.get_type()) {
        case godot::Variant::STRING:
            snprintf(buffer, sizeof(buffer), "%s", ((godot::String)value).utf8().get_data());
            break;
        case godot::Variant::INT:
            snprintf(buffer, sizeof(buffer), "%d", (int)value);
            break;
        case godot::Variant::FLOAT:
            snprintf(buffer, sizeof(buffer), "%f", (double)value);
            break;
        case godot::Variant::BOOL:
            snprintf(buffer, sizeof(buffer), "%s", (bool)value ? "true" : "false");
            break;
        default:
            snprintf(buffer, sizeof(buffer), "[%s]", ((godot::String)value).utf8().get_data());
            break;
    }
    return buffer;
}



GDExtensionVariant godot_get_variant(void* node_ptr, const char* property) {
    GDExtensionVariant result = {0, {0}};
    
    if (!node_ptr) {
        UtilityFunctions::print("godot_get_variant: node is null");
        return result;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::Variant value = node->get(property);
    
    switch ((int)value.get_type()) {
        case 0:  // NIL
            result.type = VARTYPE_NULL;
            break;
        case 1:  // BOOL
            result.type = VARTYPE_BOOL;
            result.value.b = (bool)value ? 1 : 0;
            break;
        case 2:  // INT
            result.type = VARTYPE_INT;
            result.value.i = (int)value;
            break;
        case 3:  // FLOAT
            result.type = VARTYPE_FLOAT;
            result.value.f = (float)(double)value;
            break;
        case 4:   // STRING (Godot 3)
        case 21:  // STRING (Godot 4)
            result.type = VARTYPE_STRING;
            snprintf(result.value.s, sizeof(result.value.s), "%s", ((godot::String)value).utf8().get_data());
            break;
        case 5:  // VECTOR2
        {
            godot::Vector2 v = value;
            result.type = VARTYPE_VECTOR2;
            result.value.vec2.x = v.x;
            result.value.vec2.y = v.y;
            break;
        }
        case 6:  // VECTOR3
        {
            godot::Vector3 v = value;
            result.type = VARTYPE_VECTOR3;
            result.value.vec3.x = v.x;
            result.value.vec3.y = v.y;
            result.value.vec3.z = v.z;
            break;
        }
        case 29:  // COLOR (Godot 4)
        {
            godot::Color c = value;
            result.type = VARTYPE_COLOR;
            result.value.color.r = c.r;
            result.value.color.g = c.g;
            result.value.color.b = c.b;
            result.value.color.a = c.a;
            break;
        }
        default:
            UtilityFunctions::print("godot_get_variant: unhandled type=", (int)value.get_type());
            result.type = VARTYPE_NULL;
            break;
    }
    return result;
}

const char* godot_get_type_name(int type) {
    switch (type) {
        case VARTYPE_NULL: return "NULL";
        case VARTYPE_BOOL: return "BOOL";
        case VARTYPE_INT: return "INT";
        case VARTYPE_FLOAT: return "FLOAT";
        case VARTYPE_STRING: return "STRING";
        case VARTYPE_VECTOR2: return "VECTOR2";
        case VARTYPE_VECTOR3: return "VECTOR3";
        case VARTYPE_COLOR: return "COLOR";
        default: return "UNKNOWN";
    }
}

long long godot_get_ticks_msec() {
    return godot::Time::get_singleton()->get_ticks_msec();
}

void godot_set_variant(void* node_ptr, const char* property, GDExtensionVariant variant) {
    if (!node_ptr) {
        UtilityFunctions::print("godot_set_variant: node is null");
        return;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::Variant value;
    
    switch (variant.type) {
        case VARTYPE_NULL:
            value = godot::Variant();
            break;
        case VARTYPE_BOOL:
            value = godot::Variant(variant.value.b != 0);
            break;
        case VARTYPE_INT:
            value = godot::Variant(variant.value.i);
            break;
        case VARTYPE_FLOAT:
            value = godot::Variant((double)variant.value.f);
            break;
        case VARTYPE_STRING:
            value = godot::Variant(godot::String(variant.value.s));
            break;
        case VARTYPE_VECTOR2:
            value = godot::Variant(godot::Vector2(variant.value.vec2.x, variant.value.vec2.y));
            break;
        case VARTYPE_VECTOR3:
            value = godot::Variant(godot::Vector3(variant.value.vec3.x, variant.value.vec3.y, variant.value.vec3.z));
            break;
        case VARTYPE_COLOR:
            value = godot::Variant(godot::Color(variant.value.color.r, variant.value.color.g, variant.value.color.b, variant.value.color.a));
            break;
        default:
            UtilityFunctions::print("godot_set_variant: unknown type ", variant.type);
            return;
    }
    
    node->set(property, value);
}

void* godot_instantiate(const char* scene_path) {
    if (!GDTinyCC::_current_instance) {
        UtilityFunctions::print("godot_instantiate: no current instance");
        return nullptr;
    }
    
    godot::ResourceLoader* loader = godot::ResourceLoader::get_singleton();
    godot::String path(scene_path);
    
    godot::Variant loaded = loader->load(path);
    if (loaded.get_type() == godot::Variant::NIL) {
        UtilityFunctions::print("godot_instantiate: failed to load scene: ", scene_path);
        return nullptr;
    }
    
    if (loaded.get_type() != godot::Variant::OBJECT) {
        UtilityFunctions::print("godot_instantiate: not an object: ", scene_path);
        return nullptr;
    }
    
    godot::Object* obj = (godot::Object*)loaded;
    godot::PackedScene* scene = godot::Object::cast_to<godot::PackedScene>(obj);
    if (!scene) {
        UtilityFunctions::print("godot_instantiate: not a packed scene: ", scene_path);
        return nullptr;
    }
    
    godot::Node* instance = scene->instantiate();
    if (!instance) {
        UtilityFunctions::print("godot_instantiate: instantiate returned null");
        return nullptr;
    }
    
    return static_cast<void*>(instance);
}

void godot_add_child(void* parent_ptr, void* child_ptr) {
    if (!parent_ptr || !child_ptr) {
        UtilityFunctions::print("godot_add_child: parent or child is null");
        return;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::Node* child = static_cast<godot::Node*>(child_ptr);
    
    parent->add_child(child);
}

void godot_add_child_deferred(void* parent_ptr, void* child_ptr) {
    if (!parent_ptr || !child_ptr) {
        UtilityFunctions::print("godot_add_child_deferred: parent or child is null");
        return;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::Node* child = static_cast<godot::Node*>(child_ptr);
    
    parent->call_deferred("add_child", child);
}

void* godot_create(const char* class_name) {
    godot::StringName class_name_sn(class_name);
    
    if (class_name_sn == godot::StringName("Node")) {
        return static_cast<void*>(memnew(godot::Node));
    }
    if (class_name_sn == godot::StringName("Control")) {
        return static_cast<void*>(memnew(godot::Control));
    }
    if (class_name_sn == godot::StringName("Label")) {
        return static_cast<void*>(memnew(godot::Label));
    }
    if (class_name_sn == godot::StringName("Button")) {
        return static_cast<void*>(memnew(godot::Button));
    }
    if (class_name_sn == godot::StringName("Sprite2D")) {
        return static_cast<void*>(memnew(godot::Sprite2D));
    }
    if (class_name_sn == godot::StringName("Sprite3D")) {
        return static_cast<void*>(memnew(godot::Sprite3D));
    }
    if (class_name_sn == godot::StringName("Camera2D")) {
        return static_cast<void*>(memnew(godot::Camera2D));
    }
    if (class_name_sn == godot::StringName("Camera3D")) {
        return static_cast<void*>(memnew(godot::Camera3D));
    }
    if (class_name_sn == godot::StringName("Node2D")) {
        return static_cast<void*>(memnew(godot::Node2D));
    }
    if (class_name_sn == godot::StringName("Node3D")) {
        return static_cast<void*>(memnew(godot::Node3D));
    }
    if (class_name_sn == godot::StringName("CharacterBody2D")) {
        return static_cast<void*>(memnew(godot::CharacterBody2D));
    }
    if (class_name_sn == godot::StringName("CharacterBody3D")) {
        return static_cast<void*>(memnew(godot::CharacterBody3D));
    }
    if (class_name_sn == godot::StringName("Area2D")) {
        return static_cast<void*>(memnew(godot::Area2D));
    }
    if (class_name_sn == godot::StringName("Area3D")) {
        return static_cast<void*>(memnew(godot::Area3D));
    }
    if (class_name_sn == godot::StringName("StaticBody2D")) {
        return static_cast<void*>(memnew(godot::StaticBody2D));
    }
    if (class_name_sn == godot::StringName("StaticBody3D")) {
        return static_cast<void*>(memnew(godot::StaticBody3D));
    }
    if (class_name_sn == godot::StringName("RigidBody2D")) {
        return static_cast<void*>(memnew(godot::RigidBody2D));
    }
    if (class_name_sn == godot::StringName("RigidBody3D")) {
        return static_cast<void*>(memnew(godot::RigidBody3D));
    }
    if (class_name_sn == godot::StringName("Timer")) {
        return static_cast<void*>(memnew(godot::Timer));
    }
    if (class_name_sn == godot::StringName("Image")) {
        return static_cast<void*>(memnew(godot::Image));
    }
    
    UtilityFunctions::print("godot_create: unsupported class: ", class_name);
    return nullptr;
}

godot::Variant variant_from_ext(const GDExtensionVariant& ext) {
    godot::Variant value;
    switch (ext.type) {
        case VARTYPE_NULL:
            value = godot::Variant();
            break;
        case VARTYPE_BOOL:
            value = godot::Variant(ext.value.b != 0);
            break;
        case VARTYPE_INT:
            value = godot::Variant(ext.value.i);
            break;
        case VARTYPE_FLOAT:
            value = godot::Variant((double)ext.value.f);
            break;
        case VARTYPE_STRING:
            value = godot::Variant(godot::String(ext.value.s));
            break;
        case VARTYPE_VECTOR2:
            value = godot::Variant(godot::Vector2(ext.value.vec2.x, ext.value.vec2.y));
            break;
        case VARTYPE_VECTOR3:
            value = godot::Variant(godot::Vector3(ext.value.vec3.x, ext.value.vec3.y, ext.value.vec3.z));
            break;
        case VARTYPE_COLOR:
            value = godot::Variant(godot::Color(ext.value.color.r, ext.value.color.g, ext.value.color.b, ext.value.color.a));
            break;
        default:
            break;
    }
    return value;
}

GDExtensionVariant variant_to_ext(const godot::Variant& value) {
    GDExtensionVariant result = {VARTYPE_NULL, {0}};
    
    switch ((int)value.get_type()) {
        case 0:  // NIL
            result.type = VARTYPE_NULL;
            break;
        case 1:  // BOOL
            result.type = VARTYPE_BOOL;
            result.value.b = (bool)value ? 1 : 0;
            break;
        case 2:  // INT
            result.type = VARTYPE_INT;
            result.value.i = (int)value;
            break;
        case 3:  // FLOAT
            result.type = VARTYPE_FLOAT;
            result.value.f = (float)(double)value;
            break;
        case 4:   // STRING
        case 21:  // STRING (Godot 4)
            result.type = VARTYPE_STRING;
            snprintf(result.value.s, sizeof(result.value.s), "%s", ((godot::String)value).utf8().get_data());
            break;
        case 5:  // VECTOR2
        {
            godot::Vector2 v = value;
            result.type = VARTYPE_VECTOR2;
            result.value.vec2.x = v.x;
            result.value.vec2.y = v.y;
            break;
        }
        case 6:  // VECTOR3
        {
            godot::Vector3 v = value;
            result.type = VARTYPE_VECTOR3;
            result.value.vec3.x = v.x;
            result.value.vec3.y = v.y;
            result.value.vec3.z = v.z;
            break;
        }
        case 29:  // COLOR (Godot 4)
        {
            godot::Color c = value;
            result.type = VARTYPE_COLOR;
            result.value.color.r = c.r;
            result.value.color.g = c.g;
            result.value.color.b = c.b;
            result.value.color.a = c.a;
            break;
        }
        default:
            UtilityFunctions::print("godot_call: unhandled return type=", (int)value.get_type());
            break;
    }
    return result;
}

GDExtensionVariant godot_call(void* node_ptr, const char* method_name, int arg_count, GDExtensionVariant* args) {
    GDExtensionVariant result = {VARTYPE_NULL, {0}};
    
    if (!node_ptr) {
        UtilityFunctions::print("godot_call: node is null");
        return result;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    
    godot::Variant ret;
    if (arg_count == 0) {
        ret = node->call(method_name);
    } else if (args) {
        godot::Variant* variant_args = new godot::Variant[arg_count];
        for (int i = 0; i < arg_count; i++) {
            variant_args[i] = variant_from_ext(args[i]);
        }
        ret = node->call(method_name, variant_args, arg_count);
        delete[] variant_args;
    } else {
        UtilityFunctions::print("godot_call: args is null but arg_count > 0");
        return result;
    }
    
    return variant_to_ext(ret);
}

void godot_queue_free(void* node_ptr) {
    if (!node_ptr) {
        return;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    node->queue_free();
}

void godot_emit_signal(void* node_ptr, const char* signal_name, int arg_count, GDExtensionVariant* args) {
    if (!node_ptr) {
        UtilityFunctions::print("godot_emit_signal: node is null");
        return;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::StringName signal_name_sn(signal_name);
    
    if (arg_count == 0) {
        node->emit_signal(signal_name_sn);
    } else if (args) {
        godot::Variant* variant_args = new godot::Variant[arg_count];
        for (int i = 0; i < arg_count; i++) {
            variant_args[i] = variant_from_ext(args[i]);
        }
        node->emit_signal(signal_name_sn, variant_args, arg_count);
        delete[] variant_args;
    } else {
        UtilityFunctions::print("godot_emit_signal: args is null but arg_count > 0");
    }
}

bool GDTinyCC::connect_signal(void* node_ptr, const char* signal_name, void* callback_func, void* user_data) {
    if (!node_ptr || !callback_func) {
        UtilityFunctions::print("godot_connect: invalid parameters");
        return false;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::String signal_sn(signal_name);
    
    godot::SignalHandler* handler = memnew(godot::SignalHandler);
    handler->callback_func = callback_func;
    handler->user_data = user_data;
    
    signal_handlers.push_back(handler);
    
    bool result = node->connect(signal_sn, godot::Callable(handler, "_on_signal_callback"));
    
    return result;
}

void GDTinyCC::disconnect_all_signals() {
    for (auto* handler : signal_handlers) {
        memdelete(handler);
    }
    signal_handlers.clear();
}

void godot_connect(void* node_ptr, const char* signal_name, void* callback_func, void* user_data) {
    if (GDTinyCC::_current_instance) {
        GDTinyCC::_current_instance->connect_signal(node_ptr, signal_name, callback_func, user_data);
    }
}

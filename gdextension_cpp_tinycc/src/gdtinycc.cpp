
#include "gdtinycc.h"

#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <array>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
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
#include <godot_cpp/classes/csg_box3d.hpp>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/mesh_instance2d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_server2d.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/classes/directional_light3d.hpp>
#include <godot_cpp/classes/omni_light3d.hpp>
#include <godot_cpp/classes/spot_light3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event.hpp>   
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/gradient.hpp>
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/godot.hpp>


#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <cstring>
#include <cstdio>
#include <cmath>

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif



using namespace godot;



typedef struct TCCState TCCState;
//godot::GDTinyCC* godot::GDTinyCC::_current_instance = nullptr;

static GDTinyCC* g_current_compiling_instance = nullptr;

void tcc_error_callback(void *opaque, const char *msg);
void godot_print(const char *format, ...);
//void* godot_get_parent(void* node_ptr);
void* godot_get_node(void* self, const char *path);
//GDExtensionVariant godot_get_variant(void* node, const char *property);
void godot_get_variant(void* node, const char *property, GDExtensionVariant *result);
void godot_set_variant(void* node, const char *property, GDExtensionVariant variant);
void* godot_instantiate(void* self, const char* scene_path);
void* godot_create(const char* class_name);
void godot_add_child(void* parent, void* child);
void godot_add_child_deferred(void* parent, void* child);
void godot_remove_child(void* parent, void* child);
void godot_remove_child_deferred(void* parent, void* child);
int godot_get_children_count(void* parent);
void* godot_get_child_at(void* parent_ptr, int index);
void* godot_find_node(void* parent, const char* name, int recursive);
void godot_call(void* node_ptr, 
                const char* method_name, 
                int arg_count, 
                GDExtensionVariant* args,
                GDExtensionVariant* result);
void godot_call_pod(void* pod_ptr, 
                    int type_id,
                    const char* method_name, 
                    int arg_count, 
                    GDExtensionVariant* args,
                    GDExtensionVariant* result);
//GDExtensionVariant godot_call(void* node_ptr, const char* method_name, int arg_count, GDExtensionVariant* args);
//GDExtensionVariant godot_call_object(void* node_ptr, const char* method_name, int arg_count, GDExtensionVariant* args);
void godot_queue_free(void* node_ptr);
const char* godot_get_type_name(int type);
void godot_emit_signal(void* node_ptr, const char* signal_name, int arg_count, GDExtensionVariant* args);
long long godot_get_ticks_msec();
void godot_delay_msec(int msec);
void* godot_get_rendering_server();
void* godot_get_engine();
void* godot_get_tree(void* self);
void* godot_get_display_server();
void* godot_get_physics_server2D();
void* godot_get_physics_server3D();  
void* godot_get_os();
void* godot_get_input();
void godot_print_int(int value);
void godot_connect(void* self, void* node_ptr, const char* signal_name, void* callback_func, void* user_data);
float godot_randf();
int godot_randi();
float godot_randf_range(float a, float b);
int godot_randi_range(int a, int b);
void godot_randomize();
void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled);
void godot_draw_circle(void* canvas_item_ptr, float x, float y, float radius,
                              float r, float g, float b, float a, int filled);
void godot_draw_line(void* canvas_item_ptr, float x1, float y1, float x2, float y2,
                     float r, float g, float b, float a, float thickness);
void godot_draw_string(void* canvas_item_ptr, const char* font, float x, float y, const char* text,
                       float r, float g, float b, float a, float font_size);
void* godot_get_drawingnode(void* self);
void* godot_get_drawingcanvas(void* self);
int godot_is_pressed(void* evt);
int godot_eventcode(void* event_ptr);
GDExtensionVariant godot_get_global_mouse_position(void* self);
void* godot_load_resource(const char* path, const char* type_hint);
const char* godot_get_class_name(void* obj);
int godot_check_collision(void* area_ptr, void* other_ptr);
int godot_check_collision_3d(void* area_ptr, void* other_ptr);
void godot_setup_collision_shape(void* collision_shape, const char* shape_type, float param1, float param2, float param3);
void godot_call_deferred(void* node_ptr, const char* method_name, int arg_count, GDExtensionVariant* args);
void* godot_file_open(const char* path, const char* mode);
int godot_file_read(void* handle, char* buffer, int size);
int godot_file_write(void* handle, const char* buffer, int size);
void godot_file_close(void* handle);
void godot_file_seek(void* handle, long position);
long godot_file_get_position(void* handle);
int godot_file_eof(void* handle);
long godot_file_get_size(void* handle);
int godot_file_exists(const char* path);
int godot_directory_exists(const char* path);
int godot_make_dir(const char* path);
int godot_remove_file(const char* path);
int godot_remove_dir(const char* path);
float godot_clamp_float(float value, float min_val, float max_val);
float godot_lerp_float(float from, float to, float weight);
float godot_lerp_angle(float from, float to, float weight);
int godot_clamp_int(int value, int min_val, int max_val);

void godot_free_variant(GDExtensionVariant* variant) {
    if (!variant || !variant->ptr) {
        return;
    }
    
    switch (variant->type) {
        case VARTYPE_DICTIONARY:
            memdelete(static_cast<godot::Dictionary*>(variant->ptr));
            break;
        case VARTYPE_ARRAY:
            memdelete(static_cast<godot::Array*>(variant->ptr));
            break;
        case VARTYPE_PACKED_BYTE_ARRAY:
            memdelete(static_cast<godot::PackedByteArray*>(variant->ptr));
            break;
        //case VARTYPE_PACKED_VECTOR3_ARRAY:
        //    memdelete(static_cast<godot::PackedVector3Array*>(variant->ptr));
        //    break;
        case VARTYPE_STRING_NAME:
            memdelete(static_cast<godot::StringName*>(variant->ptr));
            break;
        default:
            break;
    }
    variant->ptr = nullptr;
}

float godot_clamp_float(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

float godot_lerp_float(float from, float to, float weight) {
    return from + (to - from) * weight;
}

float godot_lerp_angle(float from, float to, float weight) {
    float diff = to - from;
    while (diff < -180.0f) diff += 360.0f;
    while (diff > 180.0f) diff -= 360.0f;
    return from + diff * weight;
}

int godot_clamp_int(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

static std::vector<godot::Variant> g_loaded_resources;
static std::vector<godot::Variant> g_created_refs;



void GDTinyCC::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_source_file", "path"), &GDTinyCC::set_source_file);
    ClassDB::bind_method(D_METHOD("get_source_file"), &GDTinyCC::get_source_file);
    ClassDB::bind_method(D_METHOD("compile_file"), &GDTinyCC::compile_file);
    ClassDB::bind_method(D_METHOD("recompile"), &GDTinyCC::recompile);

    ClassDB::add_property(
        "GDTinyCC", 
        PropertyInfo(Variant::STRING, "source_file", PROPERTY_HINT_FILE, "*.c"), 
        "set_source_file", 
        "get_source_file"
    );
    ClassDB::bind_method(D_METHOD("set_output_object_file", "path"), &GDTinyCC::set_output_object_file);
    ClassDB::bind_method(D_METHOD("get_output_object_file"), &GDTinyCC::get_output_object_file);
    ClassDB::add_property(
        "GDTinyCC",
        PropertyInfo(Variant::STRING, "output_object_file", PROPERTY_HINT_FILE, "*.o"),
        "set_output_object_file",
        "get_output_object_file"
    );
    ClassDB::bind_method(D_METHOD("set_input_object_file", "path"), &GDTinyCC::set_input_object_file);
    ClassDB::bind_method(D_METHOD("get_input_object_file"), &GDTinyCC::get_input_object_file);

    ClassDB::add_property(
        "GDTinyCC",
        PropertyInfo(Variant::STRING, "input_object_file", PROPERTY_HINT_FILE, "*.o"),
        "set_input_object_file",
        "get_input_object_file"
    );

    ClassDB::bind_method(D_METHOD("set_enable_2d_drawing", "enabled"), &GDTinyCC::set_enable_2d_drawing);
    ClassDB::bind_method(D_METHOD("get_enable_2d_drawing"), &GDTinyCC::get_enable_2d_drawing);
    ClassDB::add_property("GDTinyCC",
        PropertyInfo(Variant::BOOL, "enable_2d_drawing"), 
        "set_enable_2d_drawing", "get_enable_2d_drawing");
    
    ClassDB::bind_method(D_METHOD("get_compile_error_count"), &GDTinyCC::get_compile_error_count);
    ClassDB::bind_method(D_METHOD("get_compile_warning_count"), &GDTinyCC::get_compile_warning_count);
    ClassDB::bind_method(D_METHOD("get_last_compile_error"), &GDTinyCC::get_last_compile_error);
    ClassDB::bind_method(D_METHOD("get_last_compile_warning"), &GDTinyCC::get_last_compile_warning);
    ClassDB::bind_method(D_METHOD("get_compile_errors"), &GDTinyCC::get_compile_errors);
    ClassDB::bind_method(D_METHOD("get_compile_warnings"), &GDTinyCC::get_compile_warnings);
    ClassDB::bind_method(D_METHOD("clear_compile_messages"), &GDTinyCC::clear_compile_messages);
}


GDTinyCC::GDTinyCC() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    //_current_instance = this;
    tcc_state = nullptr;
    UtilityFunctions::print("GDTinyCC started.");
    set_process_input(true);
}

GDTinyCC::~GDTinyCC() {
    //if (_current_instance == this) {
    //    _current_instance = nullptr;
    //}
    disconnect_all_signals();

    if (tcc_state) {
        tcc_delete((TCCState*)tcc_state);
        tcc_state = nullptr;
    }
}


void GDTinyCC::_ready() {
    if (!input_object_file.is_empty()) {
        load_object_file();
    } else {
        compile_file();
    }
    
    if (tcc_state) {
        using ReadyFunc = void(*)(void*);
        ReadyFunc ready_func = (ReadyFunc)tcc_get_symbol((TCCState*)tcc_state, "_ready");

        if (ready_func) {
            ready_func(this);
        }
    }
}

void GDTinyCC::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (tcc_state) {
        using ProcessFunc = void(*)(void*, double);
        ProcessFunc process_func = (ProcessFunc)tcc_get_symbol((TCCState*)tcc_state, "_process");
        if (process_func) {
            process_func(this, delta);
        }
    }
}

void GDTinyCC::_physics_process(double delta) {
   if (tcc_state) {
        using PPFunc = void(*)(void*, double);
        PPFunc pprocess_func = (PPFunc)tcc_get_symbol((TCCState*)tcc_state, "_physics_process");
        if (pprocess_func) {
            pprocess_func(this, delta);
        }
    }
}

void GDTinyCC::_input(const Ref<InputEvent> &event) {
    if (tcc_state) {
        using InputFunc = void(*)(void*, void*);
        InputFunc input_func = (InputFunc)tcc_get_symbol((TCCState*)tcc_state, "_input");

        if (input_func) {
            input_func(this, (void*)event.ptr());
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

    compile_error_count = 0;
    compile_warning_count = 0;
    last_compile_error = "";
    last_compile_warning = "";
    compile_errors.clear();
    compile_warnings.clear();
    
    g_current_compiling_instance = this;

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
    //tcc_add_library_path(s, "/usr/lib/x86_64-linux-gnu");
#endif
    if (p) *p = '\0';

    TCCState *s = tcc_new();
    if (!s) return;
#ifdef _WIN32
#else
    tcc_add_library_path(s, "/usr/lib/x86_64-linux-gnu");
#endif
    tcc_set_error_func(s, nullptr, tcc_error_callback);
    tcc_set_lib_path(s, dll_path);
    
    if (!output_object_file.is_empty()) {
        UtilityFunctions::print("compile_file - output_type to TCC_OUTPUT_OBJ");
        tcc_set_output_type(s, TCC_OUTPUT_OBJ);
    } else {
        tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    }
    
    tcc_add_include_path(s, dll_path);
    tcc_add_sysinclude_path(s, dll_path);
    
    String include_path = String(dll_path) + PATH_SEPARATOR + "include";
    tcc_add_include_path(s, include_path.utf8().get_data());
    tcc_add_sysinclude_path(s, include_path.utf8().get_data());
    
#ifdef _WIN32
    String tinycc_include = String(dll_path) + PATH_SEPARATOR ".." PATH_SEPARATOR ".." PATH_SEPARATOR "src" PATH_SEPARATOR "tinycc-mob" PATH_SEPARATOR "win32" PATH_SEPARATOR "include";
#else
    String tinycc_include = String(dll_path) + PATH_SEPARATOR ".." PATH_SEPARATOR ".." PATH_SEPARATOR "src" PATH_SEPARATOR "tinycc-mob" PATH_SEPARATOR "include";
#endif
    tcc_add_sysinclude_path(s, tinycc_include.utf8().get_data());
    
    tcc_add_library_path(s, dll_path);

    tcc_add_symbol(s, "godot_print", (void*)godot_print);
    //tcc_add_symbol(s, "godot_get_parent", (void*)godot_get_parent);
    tcc_add_symbol(s, "godot_get_node", (void*)godot_get_node);
    tcc_add_symbol(s, "godot_get_variant", (void*)godot_get_variant);
    tcc_add_symbol(s, "godot_set_variant", (void*)godot_set_variant);
    tcc_add_symbol(s, "godot_instantiate", (void*)godot_instantiate);
    tcc_add_symbol(s, "godot_create", (void*)godot_create);
    tcc_add_symbol(s, "godot_add_child", (void*)godot_add_child);
    tcc_add_symbol(s, "godot_add_child_deferred", (void*)godot_add_child_deferred);
    tcc_add_symbol(s, "godot_remove_child", (void*)godot_remove_child);
    tcc_add_symbol(s, "godot_remove_child_deferred", (void*)godot_remove_child_deferred);
    tcc_add_symbol(s, "godot_get_children_count", (void*)godot_get_children_count);
tcc_add_symbol(s, "godot_get_child_at", (void*)godot_get_child_at);
    tcc_add_symbol(s, "godot_find_node", (void*)godot_find_node);
    tcc_add_symbol(s, "godot_call", (void*)godot_call);
    tcc_add_symbol(s, "godot_call_pod", (void*)godot_call_pod);
    //tcc_add_symbol(s, "godot_call_object", (void*)godot_call_object);
    tcc_add_symbol(s, "godot_queue_free", (void*)godot_queue_free);
    tcc_add_symbol(s, "godot_get_type_name", (void*)godot_get_type_name);
    tcc_add_symbol(s, "godot_get_ticks_msec", (void*)godot_get_ticks_msec);
    tcc_add_symbol(s, "godot_delay_msec", (void*)godot_delay_msec);
    tcc_add_symbol(s, "godot_get_rendering_server", (void*)godot_get_rendering_server);
    tcc_add_symbol(s, "godot_get_engine", (void*)godot_get_engine);
    tcc_add_symbol(s, "godot_get_tree", (void*)godot_get_tree);
    tcc_add_symbol(s, "godot_get_display_server", (void*)godot_get_display_server);
    tcc_add_symbol(s, "godot_get_os", (void*)godot_get_os);
    tcc_add_symbol(s, "godot_get_input", (void*)godot_get_input);
    tcc_add_symbol(s, "godot_emit_signal", (void*)godot_emit_signal);
    tcc_add_symbol(s, "godot_connect", (void*)godot_connect);
    tcc_add_symbol(s, "godot_randf", (void*)godot_randf);
    tcc_add_symbol(s, "godot_randi", (void*)godot_randi);
    tcc_add_symbol(s, "godot_randf_range", (void*)godot_randf_range);
    tcc_add_symbol(s, "godot_randi_range", (void*)godot_randi_range);
    tcc_add_symbol(s, "godot_randomize", (void*)godot_randomize);
    tcc_add_symbol(s, "godot_draw_rect", (void*)godot_draw_rect);
    tcc_add_symbol(s, "godot_draw_circle", (void*)godot_draw_circle);
    tcc_add_symbol(s, "godot_draw_line", (void*)godot_draw_line);
    tcc_add_symbol(s, "godot_draw_string", (void*)godot_draw_string);
    tcc_add_symbol(s, "godot_get_drawingnode", (void*)godot_get_drawingnode);
    tcc_add_symbol(s, "godot_get_drawingcanvas", (void*)godot_get_drawingcanvas);
    tcc_add_symbol(s, "godot_is_pressed", (void*)godot_is_pressed);
    tcc_add_symbol(s, "godot_eventcode",(void*)godot_eventcode);
    tcc_add_symbol(s, "godot_get_global_mouse_position", (void*)godot_get_global_mouse_position);
    tcc_add_symbol(s, "godot_load_resource", (void*)godot_load_resource);
    tcc_add_symbol(s, "godot_get_class_name", (void*)godot_get_class_name);
    tcc_add_symbol(s, "godot_free_variant", (void*)godot_free_variant);
    tcc_add_symbol(s, "godot_check_collision", (void*)godot_check_collision);
    tcc_add_symbol(s, "godot_check_collision_3d", (void*)godot_check_collision_3d);
    tcc_add_symbol(s, "godot_setup_collision_shape", (void*)godot_setup_collision_shape);
    tcc_add_symbol(s, "godot_get_physics_server2D", (void*)godot_get_physics_server2D);
    tcc_add_symbol(s, "godot_get_physics_server3D", (void*)godot_get_physics_server3D);
    tcc_add_symbol(s, "godot_call_deferred", (void*)godot_call_deferred);
    tcc_add_symbol(s, "godot_file_open", (void*)godot_file_open);
    tcc_add_symbol(s, "godot_file_read", (void*)godot_file_read);
    tcc_add_symbol(s, "godot_file_write", (void*)godot_file_write);
    tcc_add_symbol(s, "godot_file_close", (void*)godot_file_close);
    tcc_add_symbol(s, "godot_file_seek", (void*)godot_file_seek);
    tcc_add_symbol(s, "godot_file_get_position", (void*)godot_file_get_position);
    tcc_add_symbol(s, "godot_file_eof", (void*)godot_file_eof);
    tcc_add_symbol(s, "godot_file_get_size", (void*)godot_file_get_size);
    tcc_add_symbol(s, "godot_file_exists", (void*)godot_file_exists);
    tcc_add_symbol(s, "godot_directory_exists", (void*)godot_directory_exists);
    tcc_add_symbol(s, "godot_make_dir", (void*)godot_make_dir);
    tcc_add_symbol(s, "godot_remove_file", (void*)godot_remove_file);
    tcc_add_symbol(s, "godot_remove_dir", (void*)godot_remove_dir);

    tcc_add_symbol(s, "sin", (void*)static_cast<double(*)(double)>(std::sin));
    tcc_add_symbol(s, "cos", (void*)static_cast<double(*)(double)>(std::cos));
    tcc_add_symbol(s, "tan", (void*)static_cast<double(*)(double)>(std::tan));
    tcc_add_symbol(s, "atan", (void*)static_cast<double(*)(double)>(std::atan));
    tcc_add_symbol(s, "atan2", (void*)static_cast<double(*)(double, double)>(std::atan2));
    tcc_add_symbol(s, "sqrt", (void*)static_cast<double(*)(double)>(std::sqrt));
    tcc_add_symbol(s, "pow", (void*)static_cast<double(*)(double, double)>(std::pow));
    tcc_add_symbol(s, "floor", (void*)static_cast<double(*)(double)>(std::floor));
    tcc_add_symbol(s, "ceil", (void*)static_cast<double(*)(double)>(std::ceil));
    tcc_add_symbol(s, "fabs", (void*)static_cast<double(*)(double)>(std::fabs));
    tcc_add_symbol(s, "fmod", (void*)static_cast<double(*)(double, double)>(std::fmod));
    tcc_add_symbol(s, "abs", (void*)static_cast<double(*)(double)>(std::abs));

    tcc_add_symbol(s, "log", (void*)static_cast<double(*)(double)>(std::log));
    tcc_add_symbol(s, "log10", (void*)static_cast<double(*)(double)>(std::log10));
    tcc_add_symbol(s, "exp", (void*)static_cast<double(*)(double)>(std::exp));
    tcc_add_symbol(s, "asin", (void*)static_cast<double(*)(double)>(std::asin));
    tcc_add_symbol(s, "acos", (void*)static_cast<double(*)(double)>(std::acos));
    tcc_add_symbol(s, "sinh", (void*)static_cast<double(*)(double)>(std::sinh));
    tcc_add_symbol(s, "cosh", (void*)static_cast<double(*)(double)>(std::cosh));
    tcc_add_symbol(s, "tanh", (void*)static_cast<double(*)(double)>(std::tanh));
    tcc_add_symbol(s, "asinh", (void*)static_cast<double(*)(double)>(std::asinh));
    tcc_add_symbol(s, "acosh", (void*)static_cast<double(*)(double)>(std::acosh));
    tcc_add_symbol(s, "atanh", (void*)static_cast<double(*)(double)>(std::atanh));
    tcc_add_symbol(s, "fmin", (void*)static_cast<double(*)(double, double)>(std::fmin));
    tcc_add_symbol(s, "fmax", (void*)static_cast<double(*)(double, double)>(std::fmax));
    tcc_add_symbol(s, "round", (void*)static_cast<double(*)(double)>(std::round));
    tcc_add_symbol(s, "trunc", (void*)static_cast<double(*)(double)>(std::trunc));

    tcc_add_symbol(s, "godot_clamp_float", (void*)godot_clamp_float);
    tcc_add_symbol(s, "godot_lerp_float", (void*)godot_lerp_float);
    tcc_add_symbol(s, "godot_lerp_angle", (void*)godot_lerp_angle);
    tcc_add_symbol(s, "godot_clamp_int", (void*)godot_clamp_int);

    tcc_add_symbol(s, "snprintf", (void*)snprintf);
    tcc_add_symbol(s, "memset", (void*)memset);
    tcc_add_symbol(s, "memcpy", (void*)memcpy);
    tcc_add_symbol(s, "malloc", (void*)malloc);
    tcc_add_symbol(s, "free", (void*)free);

#ifdef _WIN32
    String libtcc1_path = String(dll_path) + PATH_SEPARATOR "lib" PATH_SEPARATOR "libtcc1.a";
    if (tcc_add_file(s, libtcc1_path.utf8().get_data()) < 0) {
        UtilityFunctions::print("error: compile_file - could not load libtcc1.a");
    }
 #else
    String full_path = String(dll_path) + "/lib/libtcc1.a";
    if (tcc_add_file(s, full_path.utf8().get_data()) < 0) {
        UtilityFunctions::print("error: could not load libtcc1.a");
    }
    String lib_path = String(dll_path) + "/lib";
    tcc_add_library_path(s, lib_path.utf8().get_data());
 #endif
    PackedStringArray files_array = source_file.split(",");
    bool first_file = true;

    for (int i = 0; i < files_array.size(); i++) {
        String file = files_array[i].strip_edges();
        if (file.is_empty()) continue;

        String full_path = file;
        if (!file.begins_with("res://")) {
            full_path = "res://" + file;
        }

        if (first_file) {
            if (files_array.size() > 1) {
                UtilityFunctions::print("=== Compiling ", files_array.size(), " files ===");
            } else {
                UtilityFunctions::print("=== Compiling ", full_path, " ===");
            }
            first_file = false;
        }
        
        current_compile_file = full_path;
        
        if (files_array.size() > 1) {
            UtilityFunctions::print("  [", i + 1, "/", files_array.size(), "] Compiling: ", full_path);
        }

        Ref<FileAccess> fa = FileAccess::open(full_path, FileAccess::READ);
        if (fa.is_null()) {
            UtilityFunctions::print("error: compile_file - file open error: ", full_path);
            tcc_delete(s);
            g_current_compiling_instance = nullptr;
            return;
        }

        String source_code = fa->get_as_text();

        if (tcc_compile_string(s, source_code.utf8().get_data()) < 0) {
            tcc_delete(s);
            g_current_compiling_instance = nullptr;
            UtilityFunctions::print("=== FAILED: ", compile_error_count, " error(s), ", compile_warning_count, " warning(s) ===");
            return;
        }
    }

    if (!output_object_file.is_empty()) {
        String path = output_object_file;

        if (!path.begins_with("res://") && !path.begins_with("user://")) {
            path = "res://" + path;
        }

        String abs_path = ProjectSettings::get_singleton()->globalize_path(path);

        if (tcc_output_file(s, abs_path.utf8().get_data()) < 0) {
            UtilityFunctions::print("error: could not write object file");
        } else {
            UtilityFunctions::print("object file saved: ", abs_path);
        }

        tcc_delete(s);
        g_current_compiling_instance = nullptr;
        return;
    }

    if (tcc_relocate(s) < 0) {
        UtilityFunctions::print("error: compile_file - relocationerror!");
        tcc_delete(s);
        g_current_compiling_instance = nullptr;
        return;
    }

    tcc_state = s;
    //shared_tcc_state = tcc_state;
    //shared_drawer->init(tcc_state);
 

    using MainFunc = void(*)();
    MainFunc main_func = (MainFunc)tcc_get_symbol(s, "main");

    if (main_func) {
        main_func();
    } else {
        UtilityFunctions::print("error: compile_file - main-function not found!");
    }
    
    g_current_compiling_instance = nullptr;
    
    if (compile_error_count > 0) {
        UtilityFunctions::print("=== FAILED: ", compile_error_count, " error(s), ", compile_warning_count, " warning(s) ===");
    } else if (compile_warning_count > 0) {
        UtilityFunctions::print("=== SUCCESS: 0 error(s), ", compile_warning_count, " warning(s) ===");
    } else {
        UtilityFunctions::print("=== SUCCESS ===");
    }
}

void GDTinyCC::set_output_object_file(const String &p_path) {
    output_object_file = p_path;
}

String GDTinyCC::get_output_object_file() const {
    return output_object_file;
}

void GDTinyCC::set_input_object_file(const String &p_path) {
    input_object_file = p_path;
}

String GDTinyCC::get_input_object_file() const {
    return input_object_file;
}
void GDTinyCC::compile_to_object(const String &output_file) {

    if (source_file.is_empty()) {
        UtilityFunctions::print("no source file!");
        return;
    }

    TCCState *s = tcc_new();
    if (!s) return;

    tcc_set_error_func(s, nullptr, tcc_error_callback);

    tcc_set_output_type(s, TCC_OUTPUT_OBJ);

    char dll_path[1024];
#ifdef _WIN32
    HMODULE hModule;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&godot_print, &hModule);
    GetModuleFileNameA(hModule, dll_path, sizeof(dll_path));
#else
    Dl_info info;
    dladdr((void*)godot_print, &info);
    strncpy(dll_path, info.dli_fname, sizeof(dll_path) - 1);
#endif

    tcc_add_include_path(s, dll_path);

    String full_path = source_file;
    if (!source_file.begins_with("res://"))
        full_path = "res://" + source_file;

    Ref<FileAccess> fa = FileAccess::open(full_path, FileAccess::READ);
    if (fa.is_null()) {
        UtilityFunctions::print("source open error");
        tcc_delete(s);
        return;
    }

    String source_code = fa->get_as_text();

    if (tcc_compile_string(s, source_code.utf8().get_data()) < 0) {
        UtilityFunctions::print("compile error");
        tcc_delete(s);
        return;
    }

    if (tcc_output_file(s, output_file.utf8().get_data()) < 0) {
        UtilityFunctions::print("failed to write object file");
    }

    tcc_delete(s);

    UtilityFunctions::print("compiled object saved: ", output_file);
}


void GDTinyCC::load_object(const String &object_file) {

    TCCState *s = tcc_new();
    if (!s) return;

    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    // Godot API wieder registrieren

    tcc_add_symbol(s, "godot_print", (void*)godot_print);
    //tcc_add_symbol(s, "godot_get_parent", (void*)godot_get_parent);
    tcc_add_symbol(s, "godot_get_node", (void*)godot_get_node);
    tcc_add_symbol(s, "godot_get_variant", (void*)godot_get_variant);
    tcc_add_symbol(s, "godot_set_variant", (void*)godot_set_variant);
    tcc_add_symbol(s, "godot_instantiate", (void*)godot_instantiate);
    tcc_add_symbol(s, "godot_create", (void*)godot_create);
    tcc_add_symbol(s, "godot_add_child", (void*)godot_add_child);
    tcc_add_symbol(s, "godot_add_child_deferred", (void*)godot_add_child_deferred);
    tcc_add_symbol(s, "godot_remove_child", (void*)godot_remove_child);
    tcc_add_symbol(s, "godot_remove_child_deferred", (void*)godot_remove_child_deferred);
    tcc_add_symbol(s, "godot_get_children_count", (void*)godot_get_children_count);
tcc_add_symbol(s, "godot_get_child_at", (void*)godot_get_child_at);
    tcc_add_symbol(s, "godot_find_node", (void*)godot_find_node);
    tcc_add_symbol(s, "godot_call", (void*)godot_call);
    tcc_add_symbol(s, "godot_call_pod", (void*)godot_call_pod);
    //tcc_add_symbol(s, "godot_call_object", (void*)godot_call_object);
    tcc_add_symbol(s, "godot_queue_free", (void*)godot_queue_free);
    tcc_add_symbol(s, "godot_get_type_name", (void*)godot_get_type_name);
    tcc_add_symbol(s, "godot_get_ticks_msec", (void*)godot_get_ticks_msec);
    tcc_add_symbol(s, "godot_delay_msec", (void*)godot_delay_msec);
    tcc_add_symbol(s, "godot_get_rendering_server", (void*)godot_get_rendering_server);
    tcc_add_symbol(s, "godot_get_engine", (void*)godot_get_engine);
    tcc_add_symbol(s, "godot_get_tree", (void*)godot_get_tree);
    tcc_add_symbol(s, "godot_get_display_server", (void*)godot_get_display_server);
    tcc_add_symbol(s, "godot_get_os", (void*)godot_get_os);
    tcc_add_symbol(s, "godot_get_input", (void*)godot_get_input);
    tcc_add_symbol(s, "godot_emit_signal", (void*)godot_emit_signal);
    tcc_add_symbol(s, "godot_connect", (void*)godot_connect);
    tcc_add_symbol(s, "godot_randf", (void*)godot_randf);
    tcc_add_symbol(s, "godot_randi", (void*)godot_randi);
    tcc_add_symbol(s, "godot_randf_range", (void*)godot_randf_range);
    tcc_add_symbol(s, "godot_randi_range", (void*)godot_randi_range);
    tcc_add_symbol(s, "godot_randomize", (void*)godot_randomize);
    tcc_add_symbol(s, "godot_draw_rect", (void*)godot_draw_rect);
    tcc_add_symbol(s, "godot_draw_circle", (void*)godot_draw_circle);
    tcc_add_symbol(s, "godot_draw_line", (void*)godot_draw_line);
    tcc_add_symbol(s, "godot_draw_string", (void*)godot_draw_string);
    tcc_add_symbol(s, "godot_get_drawingnode", (void*)godot_get_drawingnode);
    tcc_add_symbol(s, "godot_get_drawingcanvas", (void*)godot_get_drawingcanvas);
    tcc_add_symbol(s, "godot_is_pressed", (void*)godot_is_pressed);
    tcc_add_symbol(s, "godot_eventcode",(void*)godot_eventcode);
    tcc_add_symbol(s, "godot_get_global_mouse_position", (void*)godot_get_global_mouse_position);
    tcc_add_symbol(s, "godot_load_resource", (void*)godot_load_resource);
    tcc_add_symbol(s, "godot_get_class_name", (void*)godot_get_class_name);
    tcc_add_symbol(s, "godot_free_variant", (void*)godot_free_variant);
    tcc_add_symbol(s, "godot_check_collision", (void*)godot_check_collision);
    tcc_add_symbol(s, "godot_check_collision_3d", (void*)godot_check_collision_3d);
    tcc_add_symbol(s, "godot_setup_collision_shape", (void*)godot_setup_collision_shape);
    tcc_add_symbol(s, "godot_get_physics_server2D", (void*)godot_get_physics_server2D);
    tcc_add_symbol(s, "godot_get_physics_server3D", (void*)godot_get_physics_server3D);
    tcc_add_symbol(s, "godot_call_deferred", (void*)godot_call_deferred);
    tcc_add_symbol(s, "godot_file_open", (void*)godot_file_open);
    tcc_add_symbol(s, "godot_file_read", (void*)godot_file_read);
    tcc_add_symbol(s, "godot_file_write", (void*)godot_file_write);
    tcc_add_symbol(s, "godot_file_close", (void*)godot_file_close);
    tcc_add_symbol(s, "godot_file_seek", (void*)godot_file_seek);
    tcc_add_symbol(s, "godot_file_get_position", (void*)godot_file_get_position);
    tcc_add_symbol(s, "godot_file_eof", (void*)godot_file_eof);
    tcc_add_symbol(s, "godot_file_get_size", (void*)godot_file_get_size);
    tcc_add_symbol(s, "godot_file_exists", (void*)godot_file_exists);
    tcc_add_symbol(s, "godot_directory_exists", (void*)godot_directory_exists);
    tcc_add_symbol(s, "godot_make_dir", (void*)godot_make_dir);
    tcc_add_symbol(s, "godot_remove_file", (void*)godot_remove_file);
    tcc_add_symbol(s, "godot_remove_dir", (void*)godot_remove_dir);

    tcc_add_symbol(s, "sin", (void*)static_cast<double(*)(double)>(std::sin));
    tcc_add_symbol(s, "cos", (void*)static_cast<double(*)(double)>(std::cos));
    tcc_add_symbol(s, "tan", (void*)static_cast<double(*)(double)>(std::tan));
    tcc_add_symbol(s, "atan", (void*)static_cast<double(*)(double)>(std::atan));
    tcc_add_symbol(s, "atan2", (void*)static_cast<double(*)(double, double)>(std::atan2));
    tcc_add_symbol(s, "sqrt", (void*)static_cast<double(*)(double)>(std::sqrt));
    tcc_add_symbol(s, "pow", (void*)static_cast<double(*)(double, double)>(std::pow));
    tcc_add_symbol(s, "floor", (void*)static_cast<double(*)(double)>(std::floor));
    tcc_add_symbol(s, "ceil", (void*)static_cast<double(*)(double)>(std::ceil));
    tcc_add_symbol(s, "fabs", (void*)static_cast<double(*)(double)>(std::fabs));
    tcc_add_symbol(s, "fmod", (void*)static_cast<double(*)(double, double)>(std::fmod));
    tcc_add_symbol(s, "abs", (void*)static_cast<double(*)(double)>(std::abs));

    tcc_add_symbol(s, "log", (void*)static_cast<double(*)(double)>(std::log));
    tcc_add_symbol(s, "log10", (void*)static_cast<double(*)(double)>(std::log10));
    tcc_add_symbol(s, "exp", (void*)static_cast<double(*)(double)>(std::exp));
    tcc_add_symbol(s, "asin", (void*)static_cast<double(*)(double)>(std::asin));
    tcc_add_symbol(s, "acos", (void*)static_cast<double(*)(double)>(std::acos));
    tcc_add_symbol(s, "sinh", (void*)static_cast<double(*)(double)>(std::sinh));
    tcc_add_symbol(s, "cosh", (void*)static_cast<double(*)(double)>(std::cosh));
    tcc_add_symbol(s, "tanh", (void*)static_cast<double(*)(double)>(std::tanh));
    tcc_add_symbol(s, "asinh", (void*)static_cast<double(*)(double)>(std::asinh));
    tcc_add_symbol(s, "acosh", (void*)static_cast<double(*)(double)>(std::acosh));
    tcc_add_symbol(s, "atanh", (void*)static_cast<double(*)(double)>(std::atanh));
    tcc_add_symbol(s, "fmin", (void*)static_cast<double(*)(double, double)>(std::fmin));
    tcc_add_symbol(s, "fmax", (void*)static_cast<double(*)(double, double)>(std::fmax));
    tcc_add_symbol(s, "round", (void*)static_cast<double(*)(double)>(std::round));
    tcc_add_symbol(s, "trunc", (void*)static_cast<double(*)(double)>(std::trunc));

    tcc_add_symbol(s, "snprintf", (void*)snprintf);
    tcc_add_symbol(s, "memset", (void*)memset);
    tcc_add_symbol(s, "memcpy", (void*)memcpy);
    tcc_add_symbol(s, "malloc", (void*)malloc);
    tcc_add_symbol(s, "free", (void*)free);

    if (tcc_add_file(s, object_file.utf8().get_data()) < 0) {
        UtilityFunctions::print("failed to load object file");
        tcc_delete(s);
        return;
    }

    if (tcc_relocate(s) < 0) {
        UtilityFunctions::print("relocation error");
        tcc_delete(s);
        return;
    }

    tcc_state = s;

    using MainFunc = void(*)();
    MainFunc main_func = (MainFunc)tcc_get_symbol(s, "main");

    if (main_func)
        main_func();
}


void GDTinyCC::load_object_file() {
    UtilityFunctions::print("load_object_file started");

    if (input_object_file.is_empty()) {
        UtilityFunctions::print("error: no input object file!");
        return;
    }

    TCCState *s = tcc_new();
    if (!s) {
        UtilityFunctions::print("error: tcc_new failed");
        return;
    }
    tcc_set_options(s, "-nostdlib");

    tcc_set_error_func(s, nullptr, tcc_error_callback);
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    char dll_path[1024];

#ifdef _WIN32
    HMODULE hModule;
    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                            (LPCSTR)&godot_print, &hModule)) {
        UtilityFunctions::print("error: load_object_file - no dll handle");
        return;
    }
    GetModuleFileNameA(hModule, dll_path, sizeof(dll_path));
    char *p = strrchr(dll_path, '\\');
#else
    Dl_info info;
    if (dladdr((void*)godot_print, &info) == 0) {
        UtilityFunctions::print("error: load_object_file - no so handle");
        return;
    }
    strncpy(dll_path, info.dli_fname, sizeof(dll_path) - 1);
    char *p = strrchr(dll_path, '/');
#endif

    if (p) *p = '\0';

    tcc_set_lib_path(s, dll_path);
    tcc_add_library_path(s, dll_path);

    tcc_add_symbol(s, "godot_print", (void*)godot_print);
    //tcc_add_symbol(s, "godot_get_parent", (void*)godot_get_parent);
    tcc_add_symbol(s, "godot_get_node", (void*)godot_get_node);
    tcc_add_symbol(s, "godot_get_variant", (void*)godot_get_variant);
    tcc_add_symbol(s, "godot_set_variant", (void*)godot_set_variant);
    tcc_add_symbol(s, "godot_instantiate", (void*)godot_instantiate);
    tcc_add_symbol(s, "godot_create", (void*)godot_create);
    tcc_add_symbol(s, "godot_add_child", (void*)godot_add_child);
    tcc_add_symbol(s, "godot_add_child_deferred", (void*)godot_add_child_deferred);
    tcc_add_symbol(s, "godot_remove_child", (void*)godot_remove_child);
    tcc_add_symbol(s, "godot_remove_child_deferred", (void*)godot_remove_child_deferred);
    tcc_add_symbol(s, "godot_get_children_count", (void*)godot_get_children_count);
tcc_add_symbol(s, "godot_get_child_at", (void*)godot_get_child_at);
    tcc_add_symbol(s, "godot_find_node", (void*)godot_find_node);
    tcc_add_symbol(s, "godot_call", (void*)godot_call);
    tcc_add_symbol(s, "godot_call_pod", (void*)godot_call_pod);
    //tcc_add_symbol(s, "godot_call_object", (void*)godot_call_object);
    tcc_add_symbol(s, "godot_queue_free", (void*)godot_queue_free);
    tcc_add_symbol(s, "godot_get_type_name", (void*)godot_get_type_name);
    tcc_add_symbol(s, "godot_get_ticks_msec", (void*)godot_get_ticks_msec);
    tcc_add_symbol(s, "godot_delay_msec", (void*)godot_delay_msec);
    tcc_add_symbol(s, "godot_get_rendering_server", (void*)godot_get_rendering_server);
    tcc_add_symbol(s, "godot_get_engine", (void*)godot_get_engine);
    tcc_add_symbol(s, "godot_get_tree", (void*)godot_get_tree);
    tcc_add_symbol(s, "godot_get_display_server", (void*)godot_get_display_server);
    tcc_add_symbol(s, "godot_get_os", (void*)godot_get_os);
    tcc_add_symbol(s, "godot_get_input", (void*)godot_get_input);
    tcc_add_symbol(s, "godot_emit_signal", (void*)godot_emit_signal);
    tcc_add_symbol(s, "godot_connect", (void*)godot_connect);
    tcc_add_symbol(s, "godot_randf", (void*)godot_randf);
    tcc_add_symbol(s, "godot_randi", (void*)godot_randi);
    tcc_add_symbol(s, "godot_randf_range", (void*)godot_randf_range);
    tcc_add_symbol(s, "godot_randi_range", (void*)godot_randi_range);
    tcc_add_symbol(s, "godot_randomize", (void*)godot_randomize);
    tcc_add_symbol(s, "godot_draw_rect", (void*)godot_draw_rect);
    tcc_add_symbol(s, "godot_draw_circle", (void*)godot_draw_circle);
    tcc_add_symbol(s, "godot_draw_line", (void*)godot_draw_line);
    tcc_add_symbol(s, "godot_draw_string", (void*)godot_draw_string);
    tcc_add_symbol(s, "godot_get_drawingnode", (void*)godot_get_drawingnode);
    tcc_add_symbol(s, "godot_get_drawingcanvas", (void*)godot_get_drawingcanvas);
    tcc_add_symbol(s, "godot_is_pressed", (void*)godot_is_pressed);
    tcc_add_symbol(s, "godot_eventcode",(void*)godot_eventcode);
    tcc_add_symbol(s, "godot_get_global_mouse_position", (void*)godot_get_global_mouse_position);
    tcc_add_symbol(s, "godot_load_resource", (void*)godot_load_resource);
    tcc_add_symbol(s, "godot_get_class_name", (void*)godot_get_class_name);
    tcc_add_symbol(s, "godot_free_variant", (void*)godot_free_variant);
    tcc_add_symbol(s, "godot_check_collision", (void*)godot_check_collision);
    tcc_add_symbol(s, "godot_check_collision_3d", (void*)godot_check_collision_3d);
    tcc_add_symbol(s, "godot_setup_collision_shape", (void*)godot_setup_collision_shape);
    tcc_add_symbol(s, "godot_get_physics_server2D", (void*)godot_get_physics_server2D);
    tcc_add_symbol(s, "godot_get_physics_server3D", (void*)godot_get_physics_server3D);
    tcc_add_symbol(s, "godot_call_deferred", (void*)godot_call_deferred);
    tcc_add_symbol(s, "godot_file_open", (void*)godot_file_open);
    tcc_add_symbol(s, "godot_file_read", (void*)godot_file_read);
    tcc_add_symbol(s, "godot_file_write", (void*)godot_file_write);
    tcc_add_symbol(s, "godot_file_close", (void*)godot_file_close);
    tcc_add_symbol(s, "godot_file_seek", (void*)godot_file_seek);
    tcc_add_symbol(s, "godot_file_get_position", (void*)godot_file_get_position);
    tcc_add_symbol(s, "godot_file_eof", (void*)godot_file_eof);
    tcc_add_symbol(s, "godot_file_get_size", (void*)godot_file_get_size);
    tcc_add_symbol(s, "godot_file_exists", (void*)godot_file_exists);
    tcc_add_symbol(s, "godot_directory_exists", (void*)godot_directory_exists);
    tcc_add_symbol(s, "godot_make_dir", (void*)godot_make_dir);
    tcc_add_symbol(s, "godot_remove_file", (void*)godot_remove_file);
    tcc_add_symbol(s, "godot_remove_dir", (void*)godot_remove_dir);

    tcc_add_symbol(s, "sin", (void*)static_cast<double(*)(double)>(std::sin));
    tcc_add_symbol(s, "cos", (void*)static_cast<double(*)(double)>(std::cos));
    tcc_add_symbol(s, "tan", (void*)static_cast<double(*)(double)>(std::tan));
    tcc_add_symbol(s, "atan", (void*)static_cast<double(*)(double)>(std::atan));
    tcc_add_symbol(s, "atan2", (void*)static_cast<double(*)(double, double)>(std::atan2));
    tcc_add_symbol(s, "sqrt", (void*)static_cast<double(*)(double)>(std::sqrt));
    tcc_add_symbol(s, "pow", (void*)static_cast<double(*)(double, double)>(std::pow));
    tcc_add_symbol(s, "floor", (void*)static_cast<double(*)(double)>(std::floor));
    tcc_add_symbol(s, "ceil", (void*)static_cast<double(*)(double)>(std::ceil));
    tcc_add_symbol(s, "fabs", (void*)static_cast<double(*)(double)>(std::fabs));
    tcc_add_symbol(s, "fmod", (void*)static_cast<double(*)(double, double)>(std::fmod));
    tcc_add_symbol(s, "abs", (void*)static_cast<double(*)(double)>(std::abs));

    tcc_add_symbol(s, "log", (void*)static_cast<double(*)(double)>(std::log));
    tcc_add_symbol(s, "log10", (void*)static_cast<double(*)(double)>(std::log10));
    tcc_add_symbol(s, "exp", (void*)static_cast<double(*)(double)>(std::exp));
    tcc_add_symbol(s, "asin", (void*)static_cast<double(*)(double)>(std::asin));
    tcc_add_symbol(s, "acos", (void*)static_cast<double(*)(double)>(std::acos));
    tcc_add_symbol(s, "sinh", (void*)static_cast<double(*)(double)>(std::sinh));
    tcc_add_symbol(s, "cosh", (void*)static_cast<double(*)(double)>(std::cosh));
    tcc_add_symbol(s, "tanh", (void*)static_cast<double(*)(double)>(std::tanh));
    tcc_add_symbol(s, "asinh", (void*)static_cast<double(*)(double)>(std::asinh));
    tcc_add_symbol(s, "acosh", (void*)static_cast<double(*)(double)>(std::acosh));
    tcc_add_symbol(s, "atanh", (void*)static_cast<double(*)(double)>(std::atanh));
    tcc_add_symbol(s, "fmin", (void*)static_cast<double(*)(double, double)>(std::fmin));
    tcc_add_symbol(s, "fmax", (void*)static_cast<double(*)(double, double)>(std::fmax));
    tcc_add_symbol(s, "round", (void*)static_cast<double(*)(double)>(std::round));
    tcc_add_symbol(s, "trunc", (void*)static_cast<double(*)(double)>(std::trunc));

    tcc_add_symbol(s, "snprintf", (void*)snprintf);
    tcc_add_symbol(s, "memset", (void*)memset);
    tcc_add_symbol(s, "memcpy", (void*)memcpy);
    tcc_add_symbol(s, "malloc", (void*)malloc);
    tcc_add_symbol(s, "free", (void*)free);

    UtilityFunctions::print("dll_path = ", dll_path);

    String libtcc1_path = String(dll_path) + PATH_SEPARATOR "lib" PATH_SEPARATOR "libtcc1.a";

    UtilityFunctions::print("libtcc1_path = ", libtcc1_path);

    FILE* f = fopen(libtcc1_path.utf8().get_data(), "rb");
    UtilityFunctions::print(f ? "FILE OK" : "FILE NOT FOUND");
    if (f) fclose(f);

    //int ret1 = tcc_add_file(s, libtcc1_path.utf8().get_data());
    //UtilityFunctions::print("tcc_add_file(libtcc1.a) returned: ", ret1);

    tcc_add_file(s, libtcc1_path.utf8().get_data());
    
    //tcc_add_symbol(s, "sin", (void*)(double(*)(double))std::sin);
    //tcc_add_symbol(s, "cos", (void*)(double(*)(double))std::cos);
    
    int libm_ret = tcc_add_library(s, "libm");
    UtilityFunctions::print("tcc_add_library(libm) returned: ", libm_ret);
    //if (ret1 < 0) {
    //    UtilityFunctions::print("error: could not load libtcc1.a");
    //    tcc_delete(s);
    //    return;
    //}

    PackedStringArray object_files = input_object_file.split(",");
    for (int i = 0; i < object_files.size(); i++) {
        String obj_file = object_files[i].strip_edges();
        if (obj_file.is_empty()) continue;

        String abs_path = ProjectSettings::get_singleton()->globalize_path(obj_file);
        abs_path = abs_path.replace("\\", "/");

        UtilityFunctions::print("object file = ", abs_path);

        int ret2 = tcc_add_file(s, abs_path.utf8().get_data());
        UtilityFunctions::print("tcc_add_file(object.o) returned: ", ret2);

        if (ret2 < 0) {
            UtilityFunctions::print("error loading object file");
            tcc_delete(s);
            return;
        }
    }

    UtilityFunctions::print("Relocating now...");
    int ret3 = tcc_relocate(s);
    UtilityFunctions::print("tcc_relocate returned: ", ret3);

    if (ret3 < 0) {
        UtilityFunctions::print("relocation error");
        tcc_delete(s);
        return;
    }

    tcc_state = s;

    using MainFunc = void(*)();
    MainFunc main_func = (MainFunc)tcc_get_symbol(s, "main");

    if (main_func) {
        UtilityFunctions::print("Calling main()...");
        main_func();
    } else {
        UtilityFunctions::print("main() not found");
    }
}

void tcc_error_callback(void *opaque, const char *msg) {
    if (g_current_compiling_instance) {
        GDTinyCC* inst = g_current_compiling_instance;
        godot::String s(msg);
        
        godot::String prefix = "[TCC] ";
        godot::String file_prefix = inst->current_compile_file;
        
        godot::String full_msg = prefix;
        if (s.contains("<string>")) {
            full_msg += file_prefix + ": " + s.replace("<string>", file_prefix);
        } else {
            full_msg += s;
        }
        
        UtilityFunctions::print(full_msg);
        
        if (s.contains("error")) {
            inst->compile_error_count++;
            inst->last_compile_error = full_msg;
            inst->compile_errors.push_back(full_msg);
        } else if (s.contains("warning")) {
            inst->compile_warning_count++;
            inst->last_compile_warning = full_msg;
            inst->compile_warnings.push_back(full_msg);
        } else {
            inst->compile_errors.push_back(full_msg);
        }
    } else {
        UtilityFunctions::print("[TCC] ", msg);
    }
}

void godot_print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    UtilityFunctions::print(buffer);
}



void* godot_get_node(void* self, const char* path) {
    if (!self) {
        UtilityFunctions::print("godot_connect: missing self pointer");
        return nullptr;
    }
    GDTinyCC* instance = static_cast<GDTinyCC*>(self);
    if (instance) {
        godot::NodePath node_path(path);
        godot::Node* node = instance->get_node<godot::Node>(node_path);
        if (!node){
            UtilityFunctions::print("error: godot_get_node - node not found ");
        }
        return static_cast<void*>(node);
    } else {
        UtilityFunctions::print("error: godot_get_node - no current instance found");
        return nullptr;
    }
}



const char* godot_get_type_name(int type) {
    switch (type) {
        case VARTYPE_NULL: return "NULL";
        case VARTYPE_BOOL: return "BOOL";
        case VARTYPE_INT: return "INT";
        case VARTYPE_FLOAT: return "FLOAT";
        case VARTYPE_STRING: return "STRING";
        case VARTYPE_STRING_NAME: return "STRING_NAME";
        case VARTYPE_VECTOR2: return "VECTOR2";
        case VARTYPE_VECTOR3: return "VECTOR3";
        case VARTYPE_VECTOR2I: return "VECTOR2I";
        case VARTYPE_VECTOR3I: return "VECTOR3I";
        case VARTYPE_COLOR: return "COLOR";
        case VARTYPE_PACKED_BYTE_ARRAY: return "PACKED_BYTE_ARRAY";
        //case VARTYPE_PACKED_VECTOR3_ARRAY: return "PACKED_VECTOR3_ARRAY";
        case VARTYPE_RECT2: return "RECT2";
        case VARTYPE_RECT2I: return "RECT2I";
        case VARTYPE_OBJECT: return "OBJECT";
        default: return "UNKNOWN";
    }
}

long long godot_get_ticks_msec() {
    return godot::Time::get_singleton()->get_ticks_msec();
}

void godot_delay_msec(int msec) {
    godot::OS::get_singleton()->delay_msec(msec);
}

void* godot_get_rendering_server() {
    return godot::RenderingServer::get_singleton();
}

void* godot_get_physics_server_2d() {
    return godot::PhysicsServer2D::get_singleton();
}

void* godot_get_physics_server_3d() {
    return godot::PhysicsServer3D::get_singleton();
}

void* godot_get_engine() {
    return godot::Engine::get_singleton();
}

void* godot_get_tree(void* self) {
    if (!self) return nullptr;
    godot::Node* node = static_cast<godot::Node*>(self);
    return static_cast<void*>(node->get_tree());
}

void* godot_get_display_server() {
    return godot::DisplayServer::get_singleton();
}

void* godot_get_physics_server2D() {
    return godot::PhysicsServer2D::get_singleton();
}

void* godot_get_physics_server3D() {
    return godot::PhysicsServer3D::get_singleton();
}


void* godot_get_os() {
    return godot::OS::get_singleton();
}

void* godot_get_input() {
    return godot::Input::get_singleton();
}

void godot_set_variant(void* node_ptr, const char* property, GDExtensionVariant variant) {
    if (!node_ptr) {
        UtilityFunctions::print("godot_set_variant: node is null, property: %s", property);
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
        case VARTYPE_VECTOR2I:
            value = godot::Variant(godot::Vector2i(variant.value.vec2i.x, variant.value.vec2i.y));
            break;
        case VARTYPE_VECTOR3I:
            value = godot::Variant(godot::Vector3i(variant.value.vec3i.x, variant.value.vec3i.y, variant.value.vec3i.z));
            break;
        case VARTYPE_PACKED_BYTE_ARRAY:
            if (variant.ptr) {
                value = godot::Variant(*static_cast<godot::PackedByteArray*>(variant.ptr));
            }
            break;
        //case VARTYPE_PACKED_VECTOR3_ARRAY:
        //    if (variant.ptr) {
        //        value = godot::Variant(*static_cast<godot::PackedVector3Array*>(variant.ptr));
        //    }
        //    break;
        case VARTYPE_RECT2I:
            value = godot::Variant(godot::Rect2i(variant.value.rect2i.position.x, variant.value.rect2i.position.y, variant.value.rect2i.size.x, variant.value.rect2i.size.y));
            break;
        case VARTYPE_COLOR: {
            godot::Color c;
            c.r = variant.value.color.r;
            c.g = variant.value.color.g;
            c.b = variant.value.color.b;
            c.a = variant.value.color.a;
            value = godot::Variant(c);
            break;
        }
        case VARTYPE_OBJECT:
            //UtilityFunctions::print("set_variant: VARTYPE_OBJECT, ptr=", (uint64_t)variant.ptr);
            if (variant.ptr) {
                value = godot::Variant(static_cast<godot::Object*>(variant.ptr));
            }
            break;
        case VARTYPE_DICTIONARY:
            if (variant.ptr) {
                value = godot::Variant(*static_cast<godot::Dictionary*>(variant.ptr));
            }
            break;
        case VARTYPE_ARRAY:
            if (variant.ptr) {
                value = godot::Variant(*static_cast<godot::Array*>(variant.ptr));
            }
            break;
        default:
            UtilityFunctions::print("godot_set_variant: unknown type ", variant.type);
            return;
    }
    
    node->set(property, value);
}

void* godot_instantiate(void* self, const char* scene_path) {
    if (!self) {
        UtilityFunctions::print("godot_instantiate: missing self pointer");
        return nullptr;
    }
    GDTinyCC* instance = static_cast<GDTinyCC*>(self);
    if (!instance) {
        UtilityFunctions::print("godot_instantiate: no current instance");
        return nullptr;
    }
    
    godot::String path(scene_path);
    if (!path.begins_with("res://") && !path.begins_with("user://")) {
        path = "res://" + path;
    }
    
    godot::Variant loaded = godot::ResourceLoader::get_singleton()->load(
        path, 
        "PackedScene",
        godot::ResourceLoader::CACHE_MODE_REUSE
    );
    
    g_loaded_resources.push_back(loaded);
    
    if (loaded.get_type() == godot::Variant::NIL) {
        UtilityFunctions::print("godot_instantiate: failed to load scene: ", scene_path);
        return nullptr;
    }
    
    if (loaded.get_type() == godot::Variant::OBJECT) {
        godot::Object* obj = loaded;
        if (!obj) {
            UtilityFunctions::print("godot_instantiate: obj is null");
            return nullptr;
        }
        
        godot::PackedScene* scene = godot::Object::cast_to<godot::PackedScene>(obj);
        if (!scene) {
            UtilityFunctions::print("godot_instantiate: not a packed scene: ", scene_path);
            return nullptr;
        }
        
        godot::Node* instancenew = scene->instantiate();
        if (!instancenew) {
            UtilityFunctions::print("godot_instantiate: instantiate returned null");
            return nullptr;
        }
        
        return static_cast<void*>(instancenew);
    }
    
    UtilityFunctions::print("godot_instantiate: not an object: ", scene_path);
    return nullptr;
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

void godot_remove_child(void* parent_ptr, void* child_ptr) {
    if (!parent_ptr || !child_ptr) {
        UtilityFunctions::print("godot_remove_child: parent or child is null");
        return;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::Node* child = static_cast<godot::Node*>(child_ptr);
    
    parent->remove_child(child);
}

void godot_remove_child_deferred(void* parent_ptr, void* child_ptr) {
    if (!parent_ptr || !child_ptr) {
        UtilityFunctions::print("godot_remove_child_deferred: parent or child is null");
        return;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::Node* child = static_cast<godot::Node*>(child_ptr);
    
    parent->call_deferred("remove_child", child);
}

int godot_get_children_count(void* parent_ptr) {
    if (!parent_ptr) {
        return 0;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::Array children = parent->get_children();
    
    return children.size();
}

void* godot_get_child_at(void* parent_ptr, int index) {
    if (!parent_ptr) {
        return nullptr;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::Array children = parent->get_children();
    
    if (index < 0 || index >= children.size()) {
        return nullptr;
    }
    
    godot::Variant child = children[index];
    if (child.get_type() == godot::Variant::OBJECT) {
        godot::Object* obj = child;
        godot::Node* node = godot::Object::cast_to<godot::Node>(obj);
        return static_cast<void*>(node);
    }
    
    return nullptr;
}

void* godot_find_node(void* parent_ptr, const char* name, int recursive) {
    if (!parent_ptr || !name) {
        return nullptr;
    }
    
    godot::Node* parent = static_cast<godot::Node*>(parent_ptr);
    godot::String name_str(name);
    godot::Node* found = parent->find_child(name_str, recursive != 0, false);
    
    return static_cast<void*>(found);
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
    if (class_name_sn == godot::StringName("CSGBox3D")) {
        return static_cast<void*>(memnew(godot::CSGBox3D));
    }
    if (class_name_sn == godot::StringName("CSGSphere3D")) {
        return static_cast<void*>(memnew(godot::CSGSphere3D));
    }
    if (class_name_sn == godot::StringName("TextureRect")) {
        return static_cast<void*>(memnew(godot::TextureRect));
    }
    if (class_name_sn == godot::StringName("ImageTexture")) {
        return static_cast<void*>(memnew(godot::ImageTexture));
    }
    if (class_name_sn == godot::StringName("StandardMaterial3D")) {
        return static_cast<void*>(memnew(godot::StandardMaterial3D));
    }
    if (class_name_sn == godot::StringName("ShaderMaterial")) {
        return static_cast<void*>(memnew(godot::ShaderMaterial));
    }
    if (class_name_sn == godot::StringName("Gradient")) {
        return static_cast<void*>(memnew(godot::Gradient));
    }
    if (class_name_sn == godot::StringName("Curve")) {
        return static_cast<void*>(memnew(godot::Curve));
    }
    if (class_name_sn == godot::StringName("ArrayMesh")) {
        return static_cast<void*>(memnew(godot::ArrayMesh));
    }
    if (class_name_sn == godot::StringName("BoxMesh")) {
        return static_cast<void*>(memnew(godot::BoxMesh));
    }
    if (class_name_sn == godot::StringName("SphereMesh")) {
        return static_cast<void*>(memnew(godot::SphereMesh));
    }
    if (class_name_sn == godot::StringName("Shader")) {
        return static_cast<void*>(memnew(godot::Shader));
    }
    if (class_name_sn == godot::StringName("CollisionShape2D")) {
        return static_cast<void*>(memnew(godot::CollisionShape2D));
    }
    if (class_name_sn == godot::StringName("CollisionShape3D")) {
        return static_cast<void*>(memnew(godot::CollisionShape3D));
    }
    if (class_name_sn == godot::StringName("MeshInstance2D")) {
        return static_cast<void*>(memnew(godot::MeshInstance2D));
    }
    if (class_name_sn == godot::StringName("MeshInstance3D")) {
        return static_cast<void*>(memnew(godot::MeshInstance3D));
    }
    if (class_name_sn == godot::StringName("AnimationPlayer")) {
        return static_cast<void*>(memnew(godot::AnimationPlayer));
    }
    if (class_name_sn == godot::StringName("AudioStreamPlayer")) {
        return static_cast<void*>(memnew(godot::AudioStreamPlayer));
    }
    if (class_name_sn == godot::StringName("AudioStreamPlayer3D")) {
        return static_cast<void*>(memnew(godot::AudioStreamPlayer3D));
    }
    if (class_name_sn == godot::StringName("AudioStreamPlayer2D")) {
        return static_cast<void*>(memnew(godot::AudioStreamPlayer2D));
    }
    if (class_name_sn == godot::StringName("Label3D")) {
        return static_cast<void*>(memnew(godot::Label3D));
    }
    if (class_name_sn == godot::StringName("DirectionalLight3D")) {
        return static_cast<void*>(memnew(godot::DirectionalLight3D));
    }
    if (class_name_sn == godot::StringName("OmniLight3D")) {
        return static_cast<void*>(memnew(godot::OmniLight3D));
    }
    if (class_name_sn == godot::StringName("SpotLight3D")) {
        return static_cast<void*>(memnew(godot::SpotLight3D));
    }
    if (class_name_sn == godot::StringName("PackedColorArray")) {
        return static_cast<void*>(memnew(godot::PackedColorArray));
    }
    //if (class_name_sn == godot::StringName("PackedVector3Array")) {
    //    return static_cast<void*>(memnew(godot::PackedVector3Array));
    //}

    // missing add here
    
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
            return godot::Vector2(ext.value.vec2.x, ext.value.vec2.y);
            //value = godot::Variant(godot::Vector2(ext.value.vec2.x, ext.value.vec2.y));
            break;

        case VARTYPE_VECTOR2I:
            //UtilityFunctions::print("variant_from_ext: VECTOR2I");
            //value = godot::Variant(godot::Vector2i(ext.value.vec2i.x, ext.value.vec2i.y));
            //UtilityFunctions::print("variant_from_ext: VECTOR2I done");
            return godot::Vector2i(ext.value.vec2i.x, ext.value.vec2i.y);
            break;
        
        case VARTYPE_RECT2:
            //value = godot::Variant(godot::Rect2(ext.value.rect2.position.x, ext.value.rect2.position.y, ext.value.rect2.size.width, ext.value.rect2.size.height ));
            return godot::Rect2(
                ext.value.rect2.position.x,
                ext.value.rect2.position.y,
                ext.value.rect2.size.x,
                ext.value.rect2.size.y
            );
            break;
        case VARTYPE_RECT2I:
            value = godot::Variant(godot::Rect2i(ext.value.rect2i.position.x, ext.value.rect2i.position.y, ext.value.rect2i.size.x, ext.value.rect2i.size.y));
            break;

        case VARTYPE_VECTOR3:
            value = godot::Variant(godot::Vector3(ext.value.vec3.x, ext.value.vec3.y, ext.value.vec3.z));
            break;
        case VARTYPE_VECTOR3I:
            value = godot::Variant(godot::Vector3i(ext.value.vec3i.x, ext.value.vec3i.y, ext.value.vec3i.z));
            break;

        case VARTYPE_PACKED_BYTE_ARRAY: {
            if (ext.ptr) {
                value = godot::Variant(*static_cast<godot::PackedByteArray*>(ext.ptr));
            } else {
                value = godot::Variant(godot::PackedByteArray());
            }
        } break;
        //case VARTYPE_PACKED_VECTOR3_ARRAY: {
        //    if (ext.ptr) {
        //        value = godot::Variant(*static_cast<godot::PackedVector3Array*>(ext.ptr));
        //    } else {
        //        value = godot::Variant(godot::PackedVector3Array());
        //    }
        //} break;
        case VARTYPE_COLOR: {
            /*
            godot::Color c;
            c.r = ext.value.color.r;
            c.g = ext.value.color.g;
            c.b = ext.value.color.b;
            c.a = ext.value.color.a;
            value = godot::Variant(c);
            
            break;
            */
            //UtilityFunctions::print("Converting COLOR: ", ext.value.color.r, " ", 
            //                        ext.value.color.g, " ", ext.value.color.b, " ", ext.value.color.a);
            return godot::Color(ext.value.color.r,
                                ext.value.color.g,
                                ext.value.color.b,
                                ext.value.color.a);
            break;
        }

        case VARTYPE_STRING_NAME: {
            if (ext.ptr) {
                godot::StringName* sn_ptr = static_cast<godot::StringName*>(ext.ptr);
                value = godot::Variant(*sn_ptr);
            } else {
                value = godot::Variant();
            }
        } break;

        case VARTYPE_OBJECT: {
            if (ext.ptr) {
                godot::Object* obj = static_cast<godot::Object*>(ext.ptr);
                value = godot::Variant(obj);
            } else {
                value = godot::Variant();
            }
        } break;
    
        case VARTYPE_DICTIONARY: {
            if (ext.ptr) {
                godot::Dictionary* dict_ptr = static_cast<godot::Dictionary*>(ext.ptr);
                value = godot::Variant(*dict_ptr);
            } else {
                value = godot::Variant();
            }
        } break;

        case VARTYPE_ARRAY: {
            if (ext.ptr) {
                godot::Array* arr_ptr = static_cast<godot::Array*>(ext.ptr);
                value = godot::Variant(*arr_ptr);
            } else {
                value = godot::Variant(Array());
            }
        } break;

        default:
            break;
    }
    return value;
}

GDExtensionVariant variant_to_ext(const godot::Variant& value) {
    GDExtensionVariant result = {VARTYPE_NULL, {0}};
    
    if (!value.get_type()) {
        //UtilityFunctions::print("variant_to_ext: NIL type, returning NULL");
        return result;
    }
    
    switch (value.get_type()) {
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
        case 4:  // STRING
        {
            result.type = VARTYPE_STRING;
            godot::String str = value;
            godot::PackedByteArray utf8 = str.to_utf8_buffer();
            int len = (int)MIN(sizeof(result.value.s) - 1, (size_t)utf8.size());
            if (len > 0) {
                memcpy(result.value.s, utf8.ptr(), len);
            }
            result.value.s[len] = '\0';
        } break;
        case 5:  // VECTOR2
        {
            godot::Vector2 v = value;
            result.type = VARTYPE_VECTOR2;
            result.value.vec2.x = v.x;
            result.value.vec2.y = v.y;
            break;
        }
        case 9:  // VECTOR3
        {
            godot::Vector3 v = value;
            result.type = VARTYPE_VECTOR3;
            result.value.vec3.x = v.x;
            result.value.vec3.y = v.y;
            result.value.vec3.z = v.z;
            break;
        }
        case 6:  // VECTOR2I
        {
            godot::Vector2i v = value;
            result.type = VARTYPE_VECTOR2I;
            result.value.vec2i.x = v.x;
            result.value.vec2i.y = v.y;
            break;
        }
        case 10:  // VECTOR3I
        {
            godot::Vector3i v = value;
            result.type = VARTYPE_VECTOR3I;
            result.value.vec3i.x = v.x;
            result.value.vec3i.y = v.y;
            result.value.vec3i.z = v.z;
            break;
        }
        case 29:  // PACKED_BYTE_ARRAY
        {
            result.type = VARTYPE_PACKED_BYTE_ARRAY;
            godot::PackedByteArray arr = value;
            result.ptr = memnew(godot::PackedByteArray(arr));
            break;
        }
        //case 36:  // PACKED_VECTOR3_ARRAY
        //{
        //    result.type = VARTYPE_PACKED_VECTOR3_ARRAY;
        //    godot::PackedVector3Array arr = value;
        //    result.ptr = memnew(godot::PackedVector3Array(arr));
        //    break;
        //}
        case 8:  // RECT2I
        {
            godot::Rect2i r = value;
            result.type = VARTYPE_RECT2I;
            result.value.rect2i.position.x = r.position.x;
            result.value.rect2i.position.y = r.position.y;
            result.value.rect2i.size.x = r.size.x;
            result.value.rect2i.size.y = r.size.y;
            break;
        }
        case 21:  // STRING_NAME
        {
            result.type = VARTYPE_STRING_NAME;
            godot::String str;

            if (value.get_type() == Variant::STRING) {
                str = value;
            } else if (value.get_type() == Variant::STRING_NAME) {
                str = godot::String(value);
            }
            godot::PackedByteArray utf8 = str.to_utf8_buffer();
            int len = (int)MIN(sizeof(result.value.s) - 1, (size_t)utf8.size());
            if (len > 0) {
                memcpy(result.value.s, utf8.ptr(), len);
            }
            result.value.s[len] = '\0';
        } break;

        case 20:  // COLOR
        {
            result.type = VARTYPE_COLOR;
            godot::Color c = value;
            result.value.color.r = c.r;
            result.value.color.g = c.g;
            result.value.color.b = c.b;
            result.value.color.a = c.a;
            break;
        }
        case 7:  // RECT2
        {
            godot::Rect2 r = value;
            result.type = VARTYPE_RECT2;
            result.value.rect2.position.x = r.position.x;
            result.value.rect2.position.y = r.position.y;
            result.value.rect2.size.x = r.size.x;
            result.value.rect2.size.y = r.size.y;
            break;
        }

        case 24:  // OBJECT
        {
            result.type = VARTYPE_OBJECT;
            godot::Object* obj = value;
            if (obj) {
                result.ptr = obj;
            }
        } break;

        case 27:  // DICTIONARY
        {
            result.type = VARTYPE_DICTIONARY;
            godot::Dictionary dict = value;
            result.ptr = memnew(godot::Dictionary(dict)); 
        } break;

        case 28:  // ARRAY
        {
            result.type = VARTYPE_ARRAY;
            godot::Array arr = value;
            result.ptr = memnew(godot::Array(arr));
        } break;

        default:
            UtilityFunctions::print("godot_call: unhandled return type=", (int)value.get_type());
            break;
    }
    return result;
}


void godot_get_variant(void* node, const char *property, GDExtensionVariant *result) {
    if (!result) return;
    *result = {VARTYPE_NULL, {0}, nullptr};

    if (!node) return;

    godot::Object* obj = static_cast<godot::Object*>(node);
    godot::Variant v = obj->get(godot::String(property));

    *result = variant_to_ext(v);
}


void godot_call(void* node_ptr,
                const char* method_name,
                int arg_count,
                GDExtensionVariant* args,
                GDExtensionVariant* result)
{
    if (!result) return;

    memset(result, 0, sizeof(GDExtensionVariant));
    result->type = VARTYPE_NULL;

    if (!node_ptr || !method_name) {
        return;
    }

    godot::Object* obj = static_cast<godot::Object*>(node_ptr);

    godot::Variant ret;

    if (arg_count == 0 || args == nullptr) {
        ret = obj->call(method_name);
    } 
    else {
        godot::Array call_args;
        for (int i = 0; i < arg_count; i++) {
            call_args.push_back(variant_from_ext(args[i]));
        }
        ret = obj->callv(method_name, call_args);
    }

    *result = variant_to_ext(ret);
}

void godot_call_pod(void* pod_ptr, 
                    int type_id,
                    const char* method_name, 
                    int arg_count, 
                    GDExtensionVariant* args,
                    GDExtensionVariant* result)
{
    if (!result) return;
    memset(result, 0, sizeof(GDExtensionVariant));
    result->type = VARTYPE_NULL;

    if (!pod_ptr || !method_name) {
        return;
    }

    if (type_id == 36) {  // PACKED_VECTOR3_ARRAY
        auto* arr = static_cast<godot::PackedVector3Array*>(pod_ptr);

        if (strcmp(method_name, "size") == 0) {
            result->type = VARTYPE_INT;
            result->value.i = (int)arr->size();
            return;
        }
        if (strcmp(method_name, "is_empty") == 0) {
            result->type = VARTYPE_BOOL;
            result->value.b = arr->is_empty() ? 1 : 0;
            return;
        }
        if (strcmp(method_name, "clear") == 0) {
            arr->clear();
            return;
        }
        if (strcmp(method_name, "resize") == 0 && arg_count == 1) {
            arr->resize(args[0].value.i);
            return;
        }

        if (strcmp(method_name, "push_back") == 0 && arg_count == 1) {
            if (args[0].type == VARTYPE_VECTOR3) {
                godot::Vector3 v = variant_from_ext(args[0]);
                arr->push_back(v);
                result->type = VARTYPE_INT;
                result->value.i = (int)arr->size();
            } else {
                UtilityFunctions::print("godot_call_pod: push_back expects Vector3");
            }
            return;
        }

        if (strcmp(method_name, "get") == 0 && arg_count == 1) {
            int idx = args[0].value.i;
            if (idx >= 0 && idx < (int)arr->size()) {
                godot::Vector3 v = (*arr)[idx];
                result->type = VARTYPE_VECTOR3;
                result->value.vec3.x = v.x;
                result->value.vec3.y = v.y;
                result->value.vec3.z = v.z;
            }
            return;
        }
    } 
    else if (type_id == 29) {  // PACKED_BYTE_ARRAY
        auto* arr = static_cast<godot::PackedByteArray*>(pod_ptr);

        if (strcmp(method_name, "size") == 0) {
            result->type = VARTYPE_INT;
            result->value.i = (int)arr->size();
            return;
        }
        if (strcmp(method_name, "is_empty") == 0) {
            result->type = VARTYPE_BOOL;
            result->value.b = arr->is_empty() ? 1 : 0;
            return;
        }
        if (strcmp(method_name, "clear") == 0) {
            arr->clear();
            return;
        }
        if (strcmp(method_name, "resize") == 0 && arg_count == 1) {
            arr->resize(args[0].value.i);
            return;
        }
        if (strcmp(method_name, "push_back") == 0 && arg_count == 1) {
            arr->push_back(static_cast<uint8_t>(args[0].value.i));
            result->type = VARTYPE_INT;
            result->value.i = (int)arr->size();
            return;
        }
    }

}

void godot_queue_free(void* node_ptr) {
    if (!node_ptr) {
        return;
    }
    
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    node->queue_free();
}

/*
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
*/
void godot_emit_signal(void* node_ptr, const char* signal_name, int arg_count, GDExtensionVariant* args) {
    if (!node_ptr) return;
    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::StringName sn(signal_name);

    if (arg_count == 0) {
        node->emit_signal(sn);
        return;
    }

    godot::Variant* variant_args = memnew_arr(godot::Variant, arg_count);
    for (int i = 0; i < arg_count; i++) {
        variant_args[i] = variant_from_ext(args[i]);
    }

    const godot::Variant** arg_ptrs = memnew_arr(const godot::Variant*, arg_count);
    for (int i = 0; i < arg_count; i++) arg_ptrs[i] = &variant_args[i];

    node->emit_signal(sn, arg_ptrs, arg_count);

    memdelete_arr(arg_ptrs);
    memdelete_arr(variant_args);
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

void godot_connect(void* self, void* node_ptr, const char* signal_name, void* callback_func, void* user_data) {
    if (!self) {
        UtilityFunctions::print("godot_connect: missing self pointer");
        return;
    }
    GDTinyCC* instance = static_cast<GDTinyCC*>(self);
    if (instance) {
        instance->connect_signal(node_ptr, signal_name, callback_func, user_data);
    }
}

void GDTinyCC::_enter_tree() {
    if (enable_2d_drawing && !drawing_node) {
        drawing_canvas = memnew(CanvasLayer);
        drawing_canvas->set_layer(100);
        this->add_child(drawing_canvas);

        drawing_node = memnew(GDTinyCCDrawer);
        static_cast<GDTinyCCDrawer*>(drawing_node)->parent_tcc = this;
        drawing_canvas->add_child(drawing_node);
        
        // opt: drawing_node->set_position(Vector2(0,0));
    }

    if (tcc_state) {
        using Func = void(*)();
        Func f = (Func)tcc_get_symbol((TCCState*)tcc_state, "_enter_tree");
        if (f) f();
    }
    //setup_drawing_layer();
}

void GDTinyCC::_exit_tree() {
    if (drawing_node) {
        drawing_node->queue_free();
        drawing_node = nullptr;
    }
    if (drawing_canvas) {
        drawing_canvas->queue_free();
        drawing_canvas = nullptr;
    }

    if (tcc_state) {
        using Func = void(*)();
        Func f = (Func)tcc_get_symbol((TCCState*)tcc_state, "_exit_tree");
        if (f) f();
    }
    disconnect_all_signals();
    Node::_exit_tree();
}

void GDTinyCC::_unhandled_input(const Ref<InputEvent> &event) {
    if (tcc_state) {
        using Func = void(*)(void*);
        Func f = (Func)tcc_get_symbol((TCCState*)tcc_state, "_unhandled_input");
        if (f) f((void*)event.ptr());
    }
}

void GDTinyCC::_notification(int what) {
    if (tcc_state) {
        using Func = void(*)(int);
        Func f = (Func)tcc_get_symbol((TCCState*)tcc_state, "_notification");
        if (f) f(what);
    }
}

float godot_randf() {
    return godot::UtilityFunctions::randf();
}

int godot_randi() {
    return godot::UtilityFunctions::randi();
}

float godot_randf_range(float a, float b) {
    return godot::UtilityFunctions::randf_range(a, b);
}

int godot_randi_range(int a, int b) {
    return godot::UtilityFunctions::randi_range(a, b);
}

void godot_randomize(){
    godot::UtilityFunctions::randomize();
}



int godot_is_pressed(void* evt) {
    if (!evt) return 0;
    auto* ev = static_cast<godot::InputEvent*>(evt);
    return ev->is_pressed() ? 1 : 0;
}

int godot_eventcode(void* event_ptr)
{
    if (!event_ptr) {
        return -1;
    }

    godot::Object* obj = static_cast<godot::Object*>(event_ptr);

    if (obj->is_class("InputEventKey"))
    {
        auto* key_event = static_cast<godot::InputEventKey*>(event_ptr);
        if (key_event->is_pressed() || key_event->is_released()) {
            return key_event->get_keycode();
        }
        return -1;
    }

    if (obj->is_class("InputEventMouseButton"))
    {
        auto* mb_event = static_cast<godot::InputEventMouseButton*>(event_ptr);
        if (mb_event->is_pressed() || mb_event->is_released()) {
            return mb_event->get_button_index();   // 1 = left, 2 = right, 3 = middle, ...
        }
        return -1;
    }

    return -1;
}

GDExtensionVariant godot_get_global_mouse_position(void* self) {
    GDExtensionVariant result = {VARTYPE_VECTOR2, {0}, nullptr};
    
    godot::Vector2 pos(0, 0);
    if (self) {
        GDTinyCC* instance = static_cast<GDTinyCC*>(self);
        if (instance) {
            godot::Viewport* viewport = instance->get_viewport();
            if (viewport) {
                pos = viewport->get_mouse_position();
            }
        }
    }
    
    result.value.vec2.x = pos.x;
    result.value.vec2.y = pos.y;
    return result;
}

void* godot_load_resource(const char* path, const char* type_hint) {
    godot::String s(path);
    if (!s.begins_with("res://") && !s.begins_with("user://")) {
        s = "res://" + s;
    }
    
    godot::Variant loaded;
    godot::String h(type_hint);
    
    loaded = godot::ResourceLoader::get_singleton()->load(s, h, godot::ResourceLoader::CACHE_MODE_REUSE);
    
    if (loaded.get_type() == godot::Variant::OBJECT) {
        godot::Object* obj = loaded;
        
        g_loaded_resources.push_back(loaded);
        
        return static_cast<void*>(obj);
    }
    return nullptr;
}


const char* godot_get_class_name(void* obj) {
    static char buf[64] = "";
    if (!obj) {
        buf[0] = '\0';
        return buf;
    }
    godot::Object* o = static_cast<godot::Object*>(obj);
    godot::String cls = o->get_class();
    snprintf(buf, sizeof(buf), "%s", cls.utf8().get_data());
    return buf;
}

int godot_check_collision(void* area_ptr, void* other_ptr)
{
    if (!area_ptr || !other_ptr) {
        return 0;
    }

    godot::Area2D* area = godot::Object::cast_to<godot::Area2D>(
        static_cast<godot::Object*>(area_ptr)
    );
    if (!area) {
        return 0;
    }

    godot::Object* other_obj = static_cast<godot::Object*>(other_ptr);

    godot::Area2D* other_area = godot::Object::cast_to<godot::Area2D>(other_obj);
    if (other_area) {
        return area->overlaps_area(other_area) ? 1 : 0;
    }

    godot::Node* other_node = godot::Object::cast_to<godot::Node>(other_obj);
    if (other_node) {
        return area->overlaps_body(other_node) ? 1 : 0;
    }

    return 0;
}

int godot_check_collision_3d(void* area_ptr, void* other_ptr) {
    if (!area_ptr || !other_ptr) {
        UtilityFunctions::print("godot_check_collision_3d: NULL ptr - area=%p other=%p", area_ptr, other_ptr);
        return 0;
    }
    
    godot::Area3D* area = static_cast<godot::Area3D*>(area_ptr);
    godot::Object* other = static_cast<godot::Object*>(other_ptr);
    
    godot::Area3D* other_area = godot::Object::cast_to<godot::Area3D>(other);
    if (other_area) {
        return area->overlaps_area(other_area) ? 1 : 0;
    }
    
    godot::Node* other_node = static_cast<godot::Node*>(other);
    return area->overlaps_body(other_node) ? 1 : 0;
}

void godot_setup_collision_shape(void* collision_shape_ptr, const char* shape_type, float param1, float param2, float param3) {
    if (!collision_shape_ptr) return;
    
    godot::CollisionShape3D* shape = static_cast<godot::CollisionShape3D*>(collision_shape_ptr);
    godot::String type(shape_type);
    
    if (type == "sphere" || type == "circle") {
        godot::SphereShape3D* sphere = memnew(godot::SphereShape3D);
        sphere->set_radius(param1);
        shape->set_shape(sphere);
    } else if (type == "box" || type == "rect") {
        godot::BoxShape3D* box = memnew(godot::BoxShape3D);
        box->set_size(godot::Vector3(param1, param2, param3));
        shape->set_shape(box);
    } else if (type == "capsule") {
        godot::CapsuleShape3D* capsule = memnew(godot::CapsuleShape3D);
        capsule->set_radius(param1);
        capsule->set_height(param2);
        shape->set_shape(capsule);
    }
}


void GDTinyCC::set_enable_2d_drawing(bool enabled) {
    if (enabled == enable_2d_drawing) {
        return;
    }

    enable_2d_drawing = enabled;

}

void* godot_get_drawingnode(void* self) {
    //GDTinyCC* tcc = static_cast<GDTinyCC*>(self);
    //return tcc ? tcc->get_drawingnode() : nullptr;
    GDTinyCC* instance = static_cast<GDTinyCC*>(self);
    return instance->get_drawingnode();
}

void* godot_get_drawingcanvas(void* self) {
    //GDTinyCC* tcc = static_cast<GDTinyCC*>(self);
    //return tcc ? tcc->get_drawingnode() : nullptr;
    GDTinyCC* instance = static_cast<GDTinyCC*>(self);
    return instance->get_drawingcanvas();
}

void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled) {
    if (!canvas_item_ptr) {
        return;
    }
    CanvasItem* ci = static_cast<CanvasItem*>(canvas_item_ptr);
    Rect2 rect(x, y, w, h);
    Color color(r, g, b, a);
    if (filled==1) {
        ci->draw_rect(rect, color, true);
    } else {
        ci->draw_rect(rect, color, false, 2.0f);
    }
}

void godot_draw_circle(void* canvas_item_ptr, float x, float y, float radius,
                       float r, float g, float b, float a, int filled) {
    if (!canvas_item_ptr) {
        return;
    }
    CanvasItem* ci = static_cast<CanvasItem*>(canvas_item_ptr);
    Vector2 center(x, y);
    Color color(r, g, b, a);
    if (filled == 1) {
        ci->draw_circle(center, radius, color);
    } else {
        ci->draw_arc(center, radius, 0, Math_TAU, 64, color, 2.0f);
    }
}

void godot_draw_line(void* canvas_item_ptr, float x1, float y1, float x2, float y2,
                     float r, float g, float b, float a, float thickness) {
    if (!canvas_item_ptr) {
        return;
    }
    CanvasItem* ci = static_cast<CanvasItem*>(canvas_item_ptr);
    Vector2 v1(x1, y1);
    Vector2 v2(x2, y2);
    Color color(r, g, b, a);
    ci->draw_line(v1, v2, color, thickness);
}

void godot_draw_string(void* canvas_item_ptr, const char* font, float x, float y, const char* text,
                       float r, float g, float b, float a, float font_size) {
    if (!canvas_item_ptr || !text) {
        return;
    }
    CanvasItem* ci = static_cast<CanvasItem*>(canvas_item_ptr);
    Vector2 pos(x, y);
    Color color(r, g, b, a);
    
    Ref<Font> font_ref;
    if (font != nullptr && strcmp(font, "default_font") == 0) {
        Ref<Theme> theme = ThemeDB::get_singleton()->get_default_theme();
        if (!theme.is_null()) {
            font_ref = theme->get_default_font();
        }
    } else if (font != nullptr && strcmp(font, "") != 0) {
        String font_path(font);
        if (!font_path.begins_with("res://")) {
            font_path = "res://" + font_path;
        }
        font_ref = ResourceLoader::get_singleton()->load(font_path);
    }
    
    ci->draw_string(font_ref, pos, String(text), HORIZONTAL_ALIGNMENT_LEFT, -1, font_size, color);
}

void godot_call_deferred(void* node_ptr, const char* method_name, 
                         int arg_count, GDExtensionVariant* args)
{
    if (!node_ptr || !method_name) {
        UtilityFunctions::print("godot_call_deferred: invalid parameters");
        return;
    }

    godot::Node* node = static_cast<godot::Node*>(node_ptr);
    godot::StringName method(method_name);

    if (arg_count == 0 || args == nullptr) {

        node->call_deferred(method);
        return;
    }

    godot::Array call_args;
    for (int i = 0; i < arg_count; i++) {
        call_args.push_back(variant_from_ext(args[i]));
    }

    node->call_deferred(method, call_args);
}

static std::vector<godot::Ref<godot::FileAccess>> g_open_files;

void* godot_file_open(const char* path, const char* mode) {
    godot::String p(path);
    if (!p.begins_with("res://") && !p.begins_with("user://") && !p.begins_with("tmp://")) {
        p = "user://" + p;
    }
    
    godot::FileAccess::ModeFlags m = godot::FileAccess::READ;
    if (strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0) {
        m = godot::FileAccess::WRITE;
    } else if (strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0) {
        m = godot::FileAccess::READ_WRITE;
    } else if (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0) {
        m = godot::FileAccess::READ;
    }
    
    godot::Ref<godot::FileAccess> fa = godot::FileAccess::open(p, m);
    if (fa.is_valid()) {
        g_open_files.push_back(fa);
        return (void*)fa.ptr();
    }
    return nullptr;
}

int godot_file_read(void* handle, char* buffer, int size) {
    if (!handle) return -1;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    return (int)fa->get_buffer((uint8_t*)buffer, size);
}

int godot_file_write(void* handle, const char* buffer, int size) {
    if (!handle) return -1;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    fa->store_buffer((const uint8_t*)buffer, size);
    return size;
}

void godot_file_close(void* handle) {
    if (!handle) return;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    for (auto it = g_open_files.begin(); it != g_open_files.end(); ++it) {
        if ((*it).ptr() == fa) {
            g_open_files.erase(it);
            break;
        }
    }
    fa->close();
}

void godot_file_seek(void* handle, long position) {
    if (!handle) return;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    fa->seek(position);
}

long godot_file_get_position(void* handle) {
    if (!handle) return 0;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    return fa->get_position();
}

int godot_file_eof(void* handle) {
    if (!handle) return 1;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    return fa->eof_reached() ? 1 : 0;
}

long godot_file_get_size(void* handle) {
    if (!handle) return 0;
    godot::FileAccess* fa = static_cast<godot::FileAccess*>(handle);
    return fa->get_length();
}

int godot_file_exists(const char* path) {
    godot::String p(path);
    if (!p.begins_with("res://") && !p.begins_with("user://") && !p.begins_with("tmp://")) {
        p = "user://" + p;
    }
    return godot::FileAccess::file_exists(p) ? 1 : 0;
}

int godot_directory_exists(const char* path) {
    godot::String p(path);
    if (!p.begins_with("res://") && !p.begins_with("user://") && !p.begins_with("tmp://")) {
        p = "user://" + p;
    }
    godot::Ref<godot::DirAccess> da = godot::DirAccess::open(p);
    if (da.is_valid()) {
        bool exists = da->dir_exists(p);
        return exists ? 1 : 0;
    }
    return 0;
}

int godot_make_dir(const char* path) {
    godot::String p(path);
    if (!p.begins_with("res://") && !p.begins_with("user://") && !p.begins_with("tmp://")) {
        p = "user://" + p;
    }
    godot::Ref<godot::DirAccess> da = godot::DirAccess::open(p.get_base_dir());
    if (da.is_valid()) {
        int err = da->make_dir(p);
        return err == godot::OK ? 0 : -1;
    }
    return -1;
}

int godot_remove_file(const char* path) {
    godot::String p(path);
    if (!p.begins_with("res://") && !p.begins_with("user://") && !p.begins_with("tmp://")) {
        p = "user://" + p;
    }
    godot::Ref<godot::DirAccess> da = godot::DirAccess::open(p.get_base_dir());
    if (da.is_valid()) {
        int err = da->remove(p);
        return err == godot::OK ? 0 : -1;
    }
    return -1;
}

int godot_remove_dir(const char* path) {
    godot::String p(path);
    if (!p.begins_with("res://") && !p.begins_with("user://") && !p.begins_with("tmp://")) {
        p = "user://" + p;
    }
    godot::Ref<godot::DirAccess> da = godot::DirAccess::open(p);
    if (da.is_valid()) {
        int err = da->remove(p);
        return err == godot::OK ? 0 : -1;
    }
    return -1;
}

Array GDTinyCC::get_compile_errors() const {
    Array result;
    for (const auto& err : compile_errors) {
        result.append(err);
    }
    return result;
}

Array GDTinyCC::get_compile_warnings() const {
    Array result;
    for (const auto& warn : compile_warnings) {
        result.append(warn);
    }
    return result;
}

void GDTinyCC::clear_compile_messages() {
    compile_error_count = 0;
    compile_warning_count = 0;
    last_compile_error = "";
    last_compile_warning = "";
    compile_errors.clear();
    compile_warnings.clear();
}

void GDTinyCC::recompile() {
    if (!tcc_state) {
        compile_file();
        return;
    }

    TCCState* old_s = (TCCState*)tcc_state;

    using GetVarsFunc = void*(*)();
    using SetVarsFunc = void(*)(void*);
    GetVarsFunc get_vars_func = (GetVarsFunc)tcc_get_symbol(old_s, "_get_hotreload_vars");
    SetVarsFunc set_vars_func = (SetVarsFunc)tcc_get_symbol(old_s, "_set_hotreload_vars");

    if (get_vars_func && set_vars_func) {
        void* var_state = get_vars_func();

        compile_error_count = 0;
        compile_warning_count = 0;
        last_compile_error = "";
        last_compile_warning = "";
        compile_errors.clear();
        compile_warnings.clear();

        tcc_delete(old_s);
        tcc_state = nullptr;

        compile_file();

        if (compile_error_count > 0 || !tcc_state) {
            compile_error_count = 0;
            compile_warning_count = 0;
            last_compile_error = "";
            last_compile_warning = "";
            compile_errors.clear();
            compile_warnings.clear();
            return;
        }

        TCCState* new_s = (TCCState*)tcc_state;
        SetVarsFunc new_set = (SetVarsFunc)tcc_get_symbol(new_s, "_set_hotreload_vars");
        if (new_set) {
            new_set(var_state);
        }
    } else {
        compile_error_count = 0;
        compile_warning_count = 0;
        last_compile_error = "";
        last_compile_warning = "";
        compile_errors.clear();
        compile_warnings.clear();

        tcc_delete(old_s);
        tcc_state = nullptr;

        compile_file();
    }

    UtilityFunctions::print("=== HOT-RELOAD ===");
}
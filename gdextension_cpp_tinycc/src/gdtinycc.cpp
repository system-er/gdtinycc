#include "gdtinycc.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/file_access.hpp>



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

typedef struct TCCState TCCState;
godot::GDTinyCC* godot::GDTinyCC::_current_instance = nullptr;

void tcc_error_callback(void *opaque, const char *msg);
void godot_print(const char *msg);
void* godot_get_node(const char *path);


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
    
    String tinycc_include = String(dll_path) + PATH_SEPARATOR ".." PATH_SEPARATOR ".." PATH_SEPARATOR "src" PATH_SEPARATOR "tinycc-mob" PATH_SEPARATOR "include";
    tcc_add_sysinclude_path(s, tinycc_include.utf8().get_data());
    
    tcc_add_library_path(s, dll_path);
    tcc_add_symbol(s, "godot_print", (void*)godot_print);
    tcc_add_symbol(s, "godot_get_node", (void*)godot_get_node);

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


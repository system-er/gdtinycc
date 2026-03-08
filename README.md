# GDTinyCC - gdextension for tinycc

integrates tinycc into Godot with gdextension c++.     
compile and execute C code directly.    
edit your c-files with the editor of your choice          
and choose the name of the .c-file in the GDTinyCC-node in inspector.      
    
GDTinyCC is a new node for godot that inherits from Node.     
WIP - programmed with godot 4.5, tinycc-mob   
- _ready, _process, _physics_process and _input work.
    

# docu:       
godot_print(string)    
godot_get_node(path)    
godot_instantiate(scenepath)    
godot_create(classname)    
godot_add_child(parent, child)    
godot_add_child_deferred(parent, child)    
godot_get_property(node, property)    
godot_get_variant(node, property)    
godot_set_variant(node, property, GDExtensionVariant)    
godot_queue_free(node)    
godot_emit_signal(node, signal_name, arg_count, args)    
godot_connect(node, signal_name, callback_func, user_data)    

new var-type GDExtensionVariant    
- VARTYPE_BOOL = 1,
- VARTYPE_INT = 2,    
- VARTYPE_FLOAT = 3,    
- VARTYPE_STRING = 4,    
- VARTYPE_VECTOR2 = 5,    
- VARTYPE_VECTOR3 = 6,     
- VARTYPE_COLOR = 7    

```
example: GDExtensionVariant v = godot_get_variant(testnode, "name");
```
  
```
//example: set labeltext
GDExtensionVariant v;
v.type = VARTYPE_STRING;
snprintf(v.value.s, sizeof(v.value.s), "hello world");
void *label = godot_get_node("/root/Main/Label");
godot_set_variant(label, "text", v);
```

```
//example: set vector2
GDExtensionVariant v;
v.type = VARTYPE_VECTOR2;
v.value.vec2.x = 100.0f;
v.value.vec2.y = 200.0f;
godot_set_variant(sprite, "position", v);
```

```
//example: set color
GDExtensionVariant v;
v.type = VARTYPE_COLOR;
v.value.color.r = 1.0f;
v.value.color.g = 0.5f;
v.value.color.b = 0.0f;
v.value.color.a = 1.0f;
```

# example src/main.c   
```
#include "stddef.h"
#include "gdtinycc_runtime.h"

void on_button_pressed(void* user_data, GDExtensionVariant arg) {
    godot_print("Button pressed!");
}
void main() {
    godot_print("hello world from GDTinyCC");
    void* testnode = godot_get_node("/root/Main");
    if (testnode != NULL) {
        godot_print("gdtinycc main: node found");
        GDExtensionVariant v = godot_get_variant(testnode, "name");
        godot_print(godot_get_type_name(v.type));
        godot_print(v.value.s);
    }
    else {
        godot_print("GDTinyCC main: node not found");
    }

    void *label = godot_create("Label");
    if (!label) {
        godot_print("failed to create label");
        return;
    }
    GDExtensionVariant v;
    v.type = VARTYPE_STRING;
    snprintf(v.value.s, sizeof(v.value.s), "this is a label from GDTinyCC");
    godot_set_variant(label, "text", v);
    
    void *main_node = godot_get_node("/root/Main");
    godot_add_child_deferred(main_node, label);

    void* button = godot_get_node("/root/Main/Button");
    godot_connect(button, "pressed", on_button_pressed, NULL);
}

void _ready() {
    godot_print("GDTinyCC _ready called!");
}

void _process(double delta) {
}

void _input(void* event_ptr)
{
    godot_print("input event!");
}
```

    
# build:    
copy tinycc-mob into src/tinycc-mob   
copy all files from src/tinycc-mob/include to demo/bin/include    
copy libtcc1.a into demo/bin/lib (compile from src/tinycc-mob/lib or copy from a tinycc-mob release...)    
copy godot-cpp version 4.5 to directory godot-cpp    
start scons    

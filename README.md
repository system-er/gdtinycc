# GDTinyCC - gdextension for tinycc

integrates tinycc into Godot with gdextension c++.     
compile and execute C code directly.    
edit your c-files with the editor of your choice          
and choose the name of the .c-file in the GDTinyCC-node in inspector.      
    
GDTinyCC is a new node for godot that inherits from Node.     
WIP - programmed with godot 4.5, tinycc-mob   
- _ready, _process, _physics_process and _input work.
    
	
three options:
- compile c-source-file .c (filename in inspector in "Source File")    
- save the compiled program in an objectfile .o (filename in inspector in "Output Object File")    
- load the saved program from an objectfile .o (filename in inspector in "Input Object File")    
  the input object-file must be in the bin-directory!     
   

	   
# docu:       
godot_print(string)    
godot_get_node(path)    
godot_instantiate(scenepath)    
godot_create(classname)    
godot_add_child(parent, child)    
godot_add_child_deferred(parent, child)    
godot_get_property(node, property)    
GDExtensionVariant godot_get_variant(node, property)    
godot_set_variant(node, property, GDExtensionVariant)    
GDExtensionVariant godot_call(node, method_name, arg_count, args)    
godot_queue_free(node)    
godot_emit_signal(node, signal_name, arg_count, args)    
godot_connect(node, signal_name, callback_func, user_data)    
godot_get_ticks_msec()    

new var-type GDExtensionVariant    
- VARTYPE_BOOL = 1,
- VARTYPE_INT = 2,    
- VARTYPE_FLOAT = 3,    
- VARTYPE_STRING = 4,    
- VARTYPE_VECTOR2 = 5,    
- VARTYPE_VECTOR3 = 6,     
- VARTYPE_COLOR = 7    


# example src/main.c   
```
// GDTinyCC test-c-program https://github.com/system-er/gdtinycc/tree/main
#include "stddef.h"
#include "gdtinycc_runtime.h"


// callfunction for button-signal
void on_button_pressed(void* user_data, GDExtensionVariant arg) {
    godot_print("Button pressed");
}

// callfunction for timer-signal
void on_timeout(void* user_data) {
    godot_print("callfunction timer");
}

// a little benchmark
int benchmark() {
	int sum = 0;
	for(int i=0; i<10000000; i++) {
		sum = sum + 1;
    }
	return sum;
}

// the c-main-function
void main() {
    godot_print("hello world from GDTinyCC.");

    // get the parentnode
    void* parent = godot_get_node("/root/Main");
    if (parent != NULL) {
        godot_print("GDTinyCC main: node found");
        GDExtensionVariant v = godot_get_variant(parent, "name");
        // show the type of the name
        godot_print(godot_get_type_name(v.type));
        // show the name of the parentnode
        godot_print(v.value.s);
    }
    else {
        godot_print("GDTinyCC main: node not found");
    }

    // create a label from code
    void *label = godot_create("Label");
    if (!label) {
        godot_print("failed to create label");
        return;
    }
    // set labeltext
    GDExtensionVariant v;
    v.type = VARTYPE_STRING;
    snprintf(v.value.s, sizeof(v.value.s), "this is a label from GDTinyCC");
    godot_set_variant(label, "text", v);
    // set labelposition
    GDExtensionVariant va;
    va.type = VARTYPE_VECTOR2;
    va.value.vec2.x = 200.0f;
    va.value.vec2.y = 20.0f;
    godot_set_variant(label, "position", va);
    godot_add_child_deferred(parent, label);

    // get an existing Button and connect the callfunction to the signal pressed
    void* button = godot_get_node("/root/Main/Button");
    godot_connect(button, "pressed", on_button_pressed, NULL);

    // create a timernode from code
    void* timer = godot_create("Timer");
    GDExtensionVariant vt1;
    vt1.type = VARTYPE_FLOAT;
    vt1.value.f = 3.0f;
    godot_set_variant(timer, "wait_time", vt1);
    GDExtensionVariant vt2;
    vt2.type = VARTYPE_BOOL;
    vt2.value.f = 1;
    godot_set_variant(timer, "autostart", vt2);
    godot_connect(timer, "timeout", on_timeout, NULL);
    godot_add_child_deferred(parent, timer);
}

// _ready-function is called from godot
void _ready() {
    godot_print("GDTinyCC _ready called!");

	// stop time for benchmark
    long start_time = godot_get_ticks_msec();
	benchmark();
	long end_time = godot_get_ticks_msec();
	long result = end_time - start_time;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%ld", result);
    godot_print("GDTinyCC time to run in ms:");
    godot_print(buffer);
}

void _process(double delta) {
}

void _physics_process(double delta) {
}

void _input(void* event_ptr)
{
    //godot_print("input event!");
}
```

other examples:    

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
    
# build:    
copy tinycc-mob into src/tinycc-mob   
copy all files from src/tinycc-mob/include to demo/bin/include    
copy libtcc1.a into demo/bin/lib (compile from src/tinycc-mob/lib or copy from a tinycc-mob release...)    
copy godot-cpp version 4.5 to directory godot-cpp    
start scons    

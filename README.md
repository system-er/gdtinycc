# GDTinyCC - gdextension C compiler

integrates tinycc(aka TCC) into Godot with gdextension c++.     
compile and execute C code directly (both very fast!).    
edit your c-files with the editor of your choice          
and choose the name of the .c-file in the GDTinyCC-node in inspector.      
    
GDTinyCC is a new node for godot that inherits from Node.     
WIP (work in progress) - programmed with godot 4.5, tinycc-mob       
- _ready, _process, _physics_process, _input and _unhandled_input
- _enter_tree, exit_tree, _notification     
- inputevents, signals           
- time to get out the old Kernighan/Ritchie's C-bible  ;-)

	
three options:
- compile and run c-source-file just in time (filename *.c in inspector in "Source File")    
- save the compiled program in an objectfile (filename *.o in inspector in "Output Object File" -    
  dont forget to press return)    
- load and run the saved program from an objectfile (filename *.o in inspector in "Input Object File" -
  dont forget to press return)       
  the input object-file must be in the bin-directory!    
  if the objectfile is chosen, export the project and it works.           
   
	   
# docu:       
_ready(self)    
_process(self, double delta)    
_physics_process(self, double delta)    
_input(self, event)    
_unhandled_input(event)     
_enter_tree()    
_exit_tree()    
_notification(what)     

- output:    
godot_print(string)
- nodes:        
godot_get_node(self, path)    
godot_instantiate(self, scenepath)    
godot_create(classname)    
godot_add_child(parent, child)    
godot_add_child_deferred(parent, child)    
godot_get_property(node, property)    
GDExtensionVariant godot_get_variant(node, property)    
godot_set_variant(node, property, GDExtensionVariant)    
GDExtensionVariant godot_call(node, method_name, arg_count, args)    
GDExtensionVariant godot_call(object, method_name, arg_count, args)    
godot_queue_free(node)    
  
- signals:    
godot_emit_signal(node, signal_name, arg_count, args)    
godot_connect(self, node, signal_name, callback_func, user_data)    
- godot random-commands:    
godot_randf()    
godot_randi()     
godot_randf_range(from, to)    
godot_randi_range(from, to)    
godot_randomize()
- time:    
godot_get_ticks_msec()     
- inputevent commands:    
godot_is_pressed(event)     
godot_eventcode(event) //if key gets the keycode, if mouse: 1=left, 2=right, 3=middle, ...     
- drawinglayer is on top of CanvasLayer:(*** out of order - under construction***)           
//godot_get_drawingnode()    
//godot_draw_rect(drawingnode, x, y, width, height, r, g, b, a, filled)    
//godot_draw_circle(drawingnode, x, y, radius, r, g, b, a, filled)    
- new var-type GDExtensionVariant:    
VARTYPE_BOOL = 1,    
VARTYPE_INT = 2,    
VARTYPE_FLOAT = 3,    
VARTYPE_STRING = 4,    
VARTYPE_VECTOR2 = 5,    
VARTYPE_VECTOR3 = 6,     
VARTYPE_COLOR = 7,    
VARTYPE_RECT2 = 8,    
VARTYPE_STRING_NAME = 21,    
VARTYPE_OBJECT = 24,     
VARTYPE_DICTIONARY = 27,    
VARTYPE_ARRAY = 28,        


# example src/main.c   
```
// GDTinyCC main.c https://github.com/system-er/gdtinycc/tree/main
#include "stddef.h"
#include "tgmath.h" // for sin, cos in process
#include "gdtinycc_runtime.h"


float timepassed = 0;
void* sprite = NULL;


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
    godot_print("hello world from GDTinyCC main.");

}

// _ready-function is called from godot
void _ready(void* self) {
    godot_print("GDTinyCC _ready called!");

    if(self){
        godot_print("self found");
    }
    
    // get the parentnode
    void* parent = godot_get_node(self, "/root/Main");
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
    snprintf(v.value.s, sizeof(v.value.s), "hey - this is a label from GDTinyCC");
    godot_set_variant(label, "text", v);
    // set labelposition
    GDExtensionVariant va;
    va.type = VARTYPE_VECTOR2;
    va.value.vec2.x = 200.0f;
    va.value.vec2.y = 20.0f;
    godot_set_variant(label, "position", va);
    godot_add_child_deferred(parent, label);

    // get an existing Button and connect the callfunction to the signal pressed
    void* button = godot_get_node(self, "/root/Main/Button");
    godot_connect(self, button, "pressed", on_button_pressed, NULL);

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
    godot_connect(self, timer, "timeout", on_timeout, NULL);
    godot_add_child_deferred(parent, timer);

    // stop time for benchmark
    long start_time = godot_get_ticks_msec();
	benchmark();
	long end_time = godot_get_ticks_msec();
	long result = end_time - start_time;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%ld", result);
    godot_print("GDTinyCC time to run in ms:");
    godot_print(buffer);

    // test sprite2d
    sprite = godot_get_node(self, "Sprite2D");

}

void _process(void* self, double delta) {
    timepassed += delta;
    GDExtensionVariant v;
    v.type = VARTYPE_VECTOR2;
    v.value.vec2.x = 10.0 + (10.0 * sin(timepassed * 2.0));
    v.value.vec2.y = 10.0 + (10.0 * cos(timepassed * 1.5));
    godot_set_variant(sprite, "position", v);
}

void _physics_process(void* self,double delta) {
}

void _input(void* self, void* event)
{
    if (!event) {
        return;
    }

    if(godot_is_pressed(event)){
        godot_print("input event:");
        int code = godot_eventcode(event);
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", code);
        godot_print(buffer);
    }
}


```

other examples:    

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

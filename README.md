# GDTinyCC - gdextension C compiler

integrates tinycc(aka TCC) into a Godot node with gdextension c++.     
compile and execute C code directly (both very fast!).    
WIP (work in progress) - programmed with godot 4.5, tinycc-mob     
time to get out the old Kernighan/Ritchie C-bible  ;-)    
    
    
edit your c-files with the editor of your choice          
and choose the name of the .c-file in the GDTinyCC-node in inspector.      
	
three options:
- compile and run c-source-file just in time (filename *.c in inspector in "Source File")    
  new for v0.2: more than one filename separated with comma, the main-file first (for example "res://src/main.c,res://src/add.c" - the add.h is included in add.c - projects with modularization possible )    
- save the compiled program in an objectfile (filename *.o in inspector in "Output Object File" -    
  dont forget to press return)    
- load and run the saved program from an objectfile (filename *.o in inspector in "Input Object File" -
  dont forget to press return)       
  the input object-file must be in the bin-directory (or use a path)        
  if the objectfile is chosen, export the project and it works.           
   
	   
# hello world:       
_ready(self)    
_process(self, double delta)    
_physics_process(self, double delta)    
_input(self, event)    
_unhandled_input(event)     
_enter_tree()    
_exit_tree()    
_notification(what)     
v02: _draw(self)    

- output:    
godot_print(string)
- nodes:        
godot_get_node(self, path)    
godot_instantiate(self, scenepath)    
godot_create(classname)    
godot_add_child(parent, child)    
godot_add_child_deferred(parent, child)       
GDExtensionVariant godot_get_variant(node, property)    
godot_set_variant(node, property, GDExtensionVariant)    
GDExtensionVariant godot_call(node, method_name, arg_count, args)    
GDExtensionVariant godot_call_object(object, method_name, arg_count, args)
godot_load_resource(path, type_hint)    
godot_queue_free(node)    
  
- signals:    
godot_emit_signal(node, signal_name, arg_count, args)    
godot_connect(self, node, signal_name, callback_func, user_data)    
- godot random:    
godot_randf()    
godot_randi()     
godot_randf_range(from, to)    
godot_randi_range(from, to)    
godot_randomize()
- time:    
godot_get_ticks_msec()     
- input:    
godot_is_pressed(event)     
godot_eventcode(event) //if key gets the keycode, if mouse: 1=left, 2=right, 3=middle, ...
godot_get_global_mouse_position(self)
- math:    
v0.2: sin()    
v0.2: cos()
- v02: 2D Drawing (if Checkbutton in GDTinyCC-node is enabled, a canvaslayer and a node2d are added to the node, then the _draw-method is working):    
v02: godot_get_drawingnode(self)    
v02: godot_draw_rect(drawingnode, x, y, width, height, r, g, b, a, filled)    
v02: godot_draw_circle(drawingnode, x, y, radius, r, g, b, a, filled)    
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

void print_int(int i){
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", i);
    godot_print(buffer);
}

void print_float(float f){
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.2f", f);
    godot_print(buffer);
}

// the c-main-function
void main() {
    godot_print("hello world from GDTinyCC main.");
}

// _ready-function is called from godot
void _ready(void* self) {
    godot_print("GDTinyCC _ready called!");
    
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
        godot_print("failed to create Label");
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

    // test sprite2d
    sprite = godot_create("Sprite2D");
    if (!sprite) {
        godot_print("failed to create Sprite2D");
        return;
    }
    godot_add_child_deferred(parent, sprite);
    //godot_print(godot_get_class_name(sprite));
    //sprite = godot_get_node(self, "Sprite2D");
    GDExtensionVariant texvar;
    texvar.type = VARTYPE_OBJECT;
    void* tex = godot_load_resource("res://icon.svg", "Texture2D");
    godot_print(godot_get_class_name(tex));
    texvar.ptr = tex;
    godot_set_variant(sprite, "texture", texvar);

}

void _process(void* self, double delta) {
    timepassed += delta;
    GDExtensionVariant v;
    v.type = VARTYPE_VECTOR2;
    v.value.vec2.x = 10.0f + (10.0f * sin(timepassed * 2.0f));
    v.value.vec2.y = 10.0f + (10.0f * cos(timepassed * 1.5f));
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
        print_int(code);

        if(code < 9){ //mousebutton
            GDExtensionVariant v;
            v.type = VARTYPE_VECTOR2;
            v = godot_get_global_mouse_position(self);
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "x: %.2f, y: %.2f", v.value.vec2.x, v.value.vec2.y);
            godot_print(buffer);
        }
    }
}

// check "Enable 2D Drawing" in GDTinyCC-node in inspector 
void _draw(void* self) {
    godot_print("started _draw()");
    void* drawingnode = godot_get_drawingnode(self);
    if(drawingnode) {
        godot_draw_rect(drawingnode, 500.0f, 200.0f, 200.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1);
        godot_draw_circle(drawingnode, 800.0f, 400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    }
    else {
        godot_print("error: drawingnode not found");
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
build tinycc for your OS...   
if libtcc1.a is after build in src/tinycc-mob copy to src/tinycc-mob/lib    
copy libtcc1.a into demo/bin/lib (from  build or copy from a tinycc-mob release...)    
copy godot-cpp version 4.5 to directory godot-cpp    
start scons    
(atm tested on ubuntu 24.4 and windows 11)    

    

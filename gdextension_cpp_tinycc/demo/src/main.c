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

// a little benchmark
int benchmark() {
	int sum = 0;
	for(int i=0; i<10000000; i++) {
		sum = sum + 1;
    }
	return sum;
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

    //godot_print("add 2+3:");
    //print_int(add(2, 3));
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
    sprite = godot_create("Sprite2D");
    if (!sprite) {
        godot_print("failed to create Sprite2D");
        return;
    }
    GDExtensionVariant off;
    off.type = VARTYPE_VECTOR2;
    off.value.vec2.x = 300;
    off.value.vec2.y = 200;
    godot_set_variant(sprite, "offset", off);
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

void _input(void* self,void* event)
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



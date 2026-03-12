// GDTinyCC main.c https://github.com/system-er/gdtinycc/tree/main
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
    godot_print("hello world from GDTinyCC main.");

}

// _ready-function is called from godot
void _ready() {
    godot_print("GDTinyCC _ready called!");

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

    // stop time for benchmark
    long start_time = godot_get_ticks_msec();
	benchmark();
	long end_time = godot_get_ticks_msec();
	long result = end_time - start_time;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%ld", result);
    godot_print("GDTinyCC time to run in ms:");
    godot_print(buffer);

    // test godot_rand random
    float randi1 = godot_randi();
    snprintf(buffer, sizeof(buffer), "%d", randi1);
    godot_print("randomnumber:");
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

void _draw() {
        void* drawingnode2d = godot_get_drawingnode();
    if (!drawingnode2d) {
        godot_print("No drawing node 2d available!\n");
        return;
    }
    godot_draw_rect(drawingnode2d, 200.0f, 300.0f, 200.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1);
}

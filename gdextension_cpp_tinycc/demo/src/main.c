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

// the c-main-function
void main() {
    godot_print("hello world from GDTinyCC main.");

    //godot_print("add 1+2 - result add:%d", add(1, 2));
}


// _ready-function is called from godot
void _ready(void* self) {

    godot_print("GDTinyCC _ready called!");

    // get the parentnode
    void* parent = godot_get_node(self, "/root/Main");

    if (parent != NULL) {
        GDExtensionVariant v;
        godot_print("GDTinyCC main: node found");
        godot_get_variant(parent, "name", &v);
        // show the name of the parentnode
        godot_print("parentname: %s", v.value.s);
    }
    else {
        godot_print("GDTinyCC main: node not found");
    }

    // test instantiate
    void* scene = godot_instantiate(self, "res://scene_label.tscn");
    godot_add_child_deferred(parent, scene);

    // create a label from code
    void *label = godot_create("Label");
    if (!label) {
        godot_print("failed to create Label");
        return;
    }

    // test godot_call
    GDExtensionVariant args[1];
    GDExtensionVariant ret;
    args[0].type = VARTYPE_STRING;
    snprintf(args[0].value.s, sizeof(args[0].value.s), "new_labelname_from_code");
    godot_call(label, "set_name", 1, args, &ret);
    GDExtensionVariant vl;
    vl.type = VARTYPE_STRING;
    //vl = godot_get_variant(label, "name");
    //godot_print("label name: %s", vl.value.s);

    // set labeltext
    GDExtensionVariant v;
    v.type = VARTYPE_STRING;
    snprintf(v.value.s, sizeof(v.value.s), "this is a label created from code");
    godot_set_variant(label, "text", v);
    // set labelposition
    GDExtensionVariant va;
    va.type = VARTYPE_VECTOR2;
    va.value.vec2.x = 600.0f;
    va.value.vec2.y = 20.0f;
    godot_set_variant(label, "position", va);
    godot_add_child_deferred(parent, label);
    // test color
    va.type = VARTYPE_COLOR;
    va.value.color.r = 0.0f;
    va.value.color.g = 1.0f;
    va.value.color.b = 0.0f;
    va.value.color.a = 1.0f;
    godot_set_variant(label, "modulate", va);


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

    // test godot_get_rendering_server
    void* rs = godot_get_rendering_server();
    args[0].type = VARTYPE_COLOR;
    args[0].value.color.r = 0.0f;
    args[0].value.color.g = 0.0f;
    args[0].value.color.b = 0.0f;
    args[0].value.color.a = 1.0f;
    godot_call(rs, "set_default_clear_color", 1, args, &ret);

    // test RefCounted object
    void* mat = godot_create("ImageTexture");
    godot_print(godot_get_class_name(mat));

    // test 2 godot_get_variant
    GDExtensionVariant p1;
    godot_get_variant(label, "position", &p1);
    godot_print("labelposition: %.2f %.2f", p1.value.vec2.x, p1.value.vec2.y);

    //godot_remove_child_deferred(parent, label);

    // test godot_find_node
    void* found = godot_find_node(parent, "Button", 1);
    if (found) {
        godot_print("Button node found");
    }

    // test godot_get_children
    int count = godot_get_children_count(parent);
    godot_print("children of parent: %d", count);
    for (int i = 0; i < count; i++) {
        void* child = godot_get_child_at(parent, i);
        godot_get_variant(child, "name", &v);
        godot_print("godot_get_child_at - childnr %d, childname: %s", i, v.value.s);
    }

    // test godot_call_deferred
    //godot_call_deferred(label, "queue_free", 0, NULL);

    // test file
    int existfile = godot_file_exists("res://icon.svg");
    if(existfile) {
        godot_print("file icon.svg exists");
    }
}


void _process(void* self, double delta) {
    timepassed += delta;
    GDExtensionVariant v;
    v.type = VARTYPE_VECTOR2;
    v.value.vec2.x = 10.0f + (10.0f * sin(timepassed * 2.0f));
    v.value.vec2.y = 10.0f + (10.0f * cos(timepassed * 1.5f));
    godot_set_variant(sprite, "position", v);

    // test input singleton with method is_key_pressed
    void* input = godot_get_input();
    GDExtensionVariant keycode;
    keycode.type = VARTYPE_INT;
    keycode.value.i = KEY_W;
    GDExtensionVariant result;
    godot_call(input, "is_key_pressed", 1, &keycode, &result);
    if (result.value.b) {
        godot_print("Key W is pressed");
    }
}


void _physics_process(void* self,double delta) {
}


void _input(void* self,void* event) {
    if (!event) {
        return;
    }

    if(godot_is_pressed(event)){
        int code = godot_eventcode(event);
        godot_print("input event: %d", code);
        
        if (code == KEY_ESCAPE) {
            GDExtensionVariant result;
            void* tree = godot_get_tree(self);
            godot_call(tree, "quit", 0, NULL, &result);
        }
        else if(code < 10){ //mousebutton
            if (code == MOUSE_BUTTON_LEFT) godot_print("mousebuttonleft");
            if (code == MOUSE_BUTTON_RIGHT) godot_print("mousebuttonright");
            if (code == MOUSE_BUTTON_MIDDLE) godot_print("mousebuttonmiddle");
            GDExtensionVariant v;
            v.type = VARTYPE_VECTOR2;
            v = godot_get_global_mouse_position(self);
            godot_print("mousepos: %.2f, %.2f", v.value.vec2.x, v.value.vec2.y);
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
        godot_draw_line(drawingnode, 600.0f, 200.0f, 700.0f, 600.0f, 1.0f, 0.0f, 0.0f, 1.0f, 6.0f);
        godot_draw_string(drawingnode, "default_font", 100.0f, 500.0f, "this is text from godot_draw_string", 1.0f, 1.0f, 1.0f, 1.0f, 22.0f);
    }
    else {
        godot_print("error: drawingnode not found");
    }
}


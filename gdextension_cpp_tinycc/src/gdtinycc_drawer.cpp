#include "gdtinycc_drawer.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


extern "C" {
    typedef struct TCCState TCCState;

    void* tcc_get_symbol(TCCState *s, const char *name);
}


using namespace godot;

void GDTinyCCDrawer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("request_redraw"), &GDTinyCCDrawer::request_redraw);
}

GDTinyCCDrawer::GDTinyCCDrawer() {
}

GDTinyCCDrawer::~GDTinyCCDrawer() {
}

void GDTinyCCDrawer::_draw() {
    draw_circle(Vector2(400, 300), 120, Color(1.0, 0.0, 0.0, 0.8));

    UtilityFunctions::print("GDTinyCCDrawer::_draw() wurde von Godot aufgerufen!");

    //if (!owner) {
    //    UtilityFunctions::print("godot_draw_: no owner");
    //    return;
    //}

    //TCCState* state = (TCCState*)owner->get_tcc_state();
    //if (!state) {
    //    UtilityFunctions::print("godot_draw: no TCCState");
    //    return;
    //}

    //using DrawFunc = void(*)();
    //DrawFunc draw_func = (DrawFunc)tcc_get_symbol(state, "_draw");

    //if (draw_func) {
    //    draw_func();
    //} else {
    //    UtilityFunctions::print("godot_draw: no _draw-symbol found");
    //}
}

void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled) {

    if (!canvas_item_ptr) {
        UtilityFunctions::print("godot_draw_rect: canvas_item_ptr is null");
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
        UtilityFunctions::print("godot_draw_circle: canvas_item_ptr is null");
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

void GDTinyCCDrawer::request_redraw() {
    queue_redraw();
}
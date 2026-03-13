#ifndef GDTINYCC_DRAWER_H
#define GDTINYCC_DRAWER_H


#include <godot_cpp/classes/node2d.hpp>


//extern "C" {
//    typedef struct TCCState TCCState;
//}


namespace godot {

class GDTinyCCDrawer : public Node2D {
    GDCLASS(GDTinyCCDrawer, Node2D)


private:
    void* tcc_state;
    void* raw_state;

protected:
    static void _bind_methods();


public:
    GDTinyCCDrawer();
    ~GDTinyCCDrawer();
    void init(void* r_state);
    //GDTinyCC* owner = nullptr;  
    void* get_tcc_state() const { return tcc_state; }
    void _draw() override;
    void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled);
    void godot_draw_circle(void* canvas_item_ptr, float x, float y, float radius,
                           float r, float g, float b, float a, int filled);
    void request_redraw();

};

} // namespace godot

#endif // GDTINYCC_DRAWER_H
#ifndef GDTINYCC_H
#define GDTINYCC_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class GDTinyCC : public Node {
    GDCLASS(GDTinyCC, Node)

private:
    String source_file;
    void *tcc_state;

protected:
    static void _bind_methods();

public:
    GDTinyCC();
    ~GDTinyCC();

    void _ready();
    void _process(double delta);
    void compile_file();
    void set_source_file(const String &p_path);
    String get_source_file() const;
};

}

#endif
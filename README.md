# GDTinyCC - gdextension for tinycc

integrates tinycc into Godot with gdextension c++.     
edit your c-files with the editor of your choice          
and choose the name of the .c-file in the GDTinyCC-node in inspector.      
    
GDTinyCC is a new node for godot that inherits from Node.     
WIP - programmed with godot 4.5, tinycc-mob    
    

# commands:       
godot_print(string)    
godot_get_node(path)    
godot_get_property(node, property)    
godot_get_variant(node, property)    

# example src/main.c   
```
#include "stddef.h"
#include "gdtinycc_runtime.h"

void main() {
    godot_print("hello world from gdtinycc");
    void* testnode = godot_get_node("/root/Main");
    if (testnode != NULL) {
        godot_print("gdtinycc main: node found");
        GDExtensionVariant v = godot_get_variant(testnode, "name");
        godot_print(godot_get_type_name(v.type));
        godot_print(v.value.s);
    }
    else {
        godot_print("gdtinycc main: node not found");
    }
}
    else {
        godot_print("gdtinycc main: node not found");
    }
}

void _ready() {
    godot_print("gdtinycc-_ready called!");
}

void _process(double delta) {
}
```

    
# build:    
copy tinycc-mob into src/tinycc-mob   
copy all files from src/tinycc-mob/include to demo/bin/include
copy libtcc1.a into demo/bin/lib (compile from src/tinycc-mob/lib or copy from a tinycc-mob release...)    
copy godot-cpp version 4.5 to directory godot-cpp    
start scons    

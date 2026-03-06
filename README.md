# GDTinyCC - gdextension for tinycc

integrates tinycc into Godot with gdextension c++.     
edit your c-files with the editor of your choice          
and choose the name of the .c-file in the GDTinyCC-node in inspector.      
    
GDTinyCC is a new node for godot that inherits from Node.     
WIP - programmed with godot 4.5, tinycc-mob    
    

# commands:       
godot_print(string)    
    

# example src/main.c   
```
#include "gdtinycc_runtime.h"

void main() {
    godot_print("hello world from gdtinycc");
}
```

    
# build:    
copy tinycc-mob into src/tinycc-mob   
copy libtcc1.a into demo/bin/lib (compile from src/tinycc-mob/lib or copy from a tinycc-mob release...)    
copy godot-cpp version 4.5 to directory godot-cpp    
start scons    

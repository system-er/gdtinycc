# GDTinyCC - gdextension for tinycc

integrates tinycc into Godot with gdextension c++.     
edit your c-files with the editor of your choic          
and type the name of the .c-file in the GDTinyCC-node in inspector.      
    
GDTinyCC is a new node for godot that inherits from Node.    
_ready, _process, _physics_process and _input functions work in squirrel.    
    
WIP - programmed with godot 4.5, tinycc-mob    
    
  

# commands:       
godot_print(string)    
    

# src/main.c   
```
#include "gdtinycc_runtime.h"

void main() {
    godot_print("hello world from gdtinycc");
}
```

    
# build:    
copy tiny-mob into src/tiny-mob   
copy godot-cpp version 4.5 to directory godot-cpp    
start scons    

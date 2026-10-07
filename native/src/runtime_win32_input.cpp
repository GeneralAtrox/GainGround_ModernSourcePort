#include "runtime_win32_internal.h"
namespace runtime_win32_detail {
void RuntimeWindow::edit_controls(){
        const bool resume=call([this]{const bool running=!paused && error.empty();if(running)toggle_pause();return running;});
        if(controls::edit(window,controllers.get,bindings)){
            post([this,chosen=bindings]{apply_bindings(chosen);});
            if(!controls_path.empty()){
                std::ofstream out(controls_path,std::ios::trunc);
                bindings.write(out);
            }
        }
        if(resume)post([this]{if(paused)toggle_pause();});
    }
}

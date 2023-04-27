#include <tiny_common.hpp>

static FILE* log_file;
void set_log_file(FILE* file){
    log_file = file;
}

FILE* get_log_file(void){
    return log_file;
}

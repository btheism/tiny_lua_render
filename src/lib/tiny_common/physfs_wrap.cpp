#include <tiny_common.hpp>

//使用PHYSFS读文件,并返回一个指向缓冲区的指针,该指针需要调用者使用delete []自行释放
file_buffer read_from_PHYSFS(const char* file_name){
    if(!PHYSFS_exists(file_name))
        fatal("cannot find file %s\n", file_name);
    PHYSFS_file* file = PHYSFS_openRead(file_name);
    size_t file_size = PHYSFS_fileLength(file);
    char *buffer = new char[PHYSFS_fileLength(file)];
    size_t length_read = PHYSFS_readBytes(file, buffer, file_size);
    PHYSFS_close(file);
    if(length_read!=file_size){
        delete []buffer;
        fatal("PHYSFS expected to read %zu bytes from file %s, but got %zu bytes\n", file_size, file_name, length_read);
    }
    return file_buffer{buffer, file_size};
}

//与上个函数相比在末尾补充了'\0', 从而正确结束字符串,返回的长度比字符串文件大1
file_buffer read_str_from_PHYSFS(const char* file_name){
    if(!PHYSFS_exists(file_name))
        fatal("cannot find file %s\n", file_name);
    PHYSFS_file* file = PHYSFS_openRead(file_name);
    size_t file_size = PHYSFS_fileLength(file);
    char *buffer = new char[PHYSFS_fileLength(file)+1];
    buffer[PHYSFS_fileLength(file)]='\0';
    size_t length_read = PHYSFS_readBytes(file, buffer, file_size);
    PHYSFS_close(file);
    if(length_read!=file_size){
        delete []buffer;
        fatal("PHYSFS expected to read %zu bytes from file %s, but got %zu bytes\n", file_size, file_name, length_read);
    }
    return file_buffer{buffer, file_size+1};
}



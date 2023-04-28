#ifndef __TINY_GLM_PRINTER
#define __TINY_GLM_PRINTER
//该文件定义了一些打印glm的矩阵和向量的函数,仅用于测试我自己的矩阵库是否和glm的行为一致,仅用在test程序中
#include <glm/glm.hpp>
#include <string>
#include <iostream>
template<typename T, size_t col, size_t row>
//列优先
std::string mat2str_col_major(const T& mat){
    std::string res;
    res.append("{");
    for(size_t c=0; c<col-1; c++){
        res.append("{");
        for(size_t r=0; r<row-1; r++){
            res.append(std::to_string(mat[c][r]));
            res.append(", ");
        }
        res.append(std::to_string(mat[c][row-1]));
        res.append("}, \n");
    }
    res.append("{");
    for(size_t r=0; r<row-1; r++){
        res.append(std::to_string(mat[col-1][r]));
        res.append(", ");
    }
    res.append(std::to_string(mat[col-1][row-1]));
    res.append("}");
    res.append("}");
    return res;
}

template<typename T, size_t col, size_t row>
//行优先
std::string mat2str(const T& mat){
    std::string res;
    res.append("{");
    for(size_t r=0; r<row-1; r++){
        res.append("{");
        for(size_t c=0; c<col-1; c++){
            res.append(std::to_string(mat[c][r]));
            res.append(", ");
        }
        res.append(std::to_string(mat[col-1][r]));
        res.append("}, \n");
    }
    res.append("{");
    for(size_t c=0; c<col-1; c++){
        res.append(std::to_string(mat[c][row-1]));
        res.append(", ");
    }
    res.append(std::to_string(mat[col-1][row-1]));
    res.append("}");
    res.append("}");
    return res;
}

template<typename T, size_t len>
std::string vec2str(const T& vec){
    std::string res;
    res.append("{");
    for(size_t i=0; i<len-1; i++){
        res.append(std::to_string(vec[i]));
        res.append(", ");
    }
    res.append(std::to_string(vec[len-1]));
    res.append("}");
    return res;
}
#endif

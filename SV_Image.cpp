#include "SV_Image.h"


template <typename T>
SV_Image<T>::SV_Image(T* data, int width, int height) : std::vector<T>(data, data+(width*height)){
    width_impl = width;
    height_impl = height;
}


template <typename T>
SV_Image<T>::SV_Image(int width, int height) : std::vector<T>(width*height){
    width_impl = width;
    height_impl = height;
}


template <typename T>
SV_Image<T>::SV_Image() {std::vector<T>();}


template <typename T>
int SV_Image<T>::width() {return width_impl;}


template <typename T>
int SV_Image<T>::height() {return height_impl;}


template class SV_Image<double>;
template class SV_Image<unsigned char>;


double clamp(double lower, double num, double upper) {
    return num <= lower ? lower : num >= upper ? upper : num;
}

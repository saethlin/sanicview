#ifndef SANICVIEW_SV_IMAGE_H
#define SANICVIEW_SV_IMAGE_H

#include <vector>
#include <stddef.h>

template <typename T>
class Image {
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    Image() {};
    Image(T* data_ptr, int width, int height) :
            data(std::vector<T>(data_ptr, data_ptr+(width*height))),
            width_impl(width),
            height_impl(height) {};
    Image(std::vector<T> data, int width, int height) :
            data(data.begin(), data.begin()+(width*height)),
            width_impl(width),
            height_impl(height) {};
    Image(int width, int height) :
            data(std::vector<T>(width*height)),
            width_impl(width),
            height_impl(height) {};
    int width() const {return width_impl;}
    int height() const {return height_impl;}
    size_t size() const {return data.size();}
    T operator()(int x, int y) const {return data[y*width_impl+x];}
    T& operator()(int x, int y) {return data[y*width_impl+x];}
    T operator[](int x) const {return data[x];}
    T& operator[](int x) {return data[x];}
    iterator begin() {return data.begin();}
    iterator end() {return data.end();}
    const_iterator begin() const {return data.begin();}
    const_iterator end() const {return data.end();}

private:
    std::vector<T> data;
    int width_impl = 0;
    int height_impl = 0;
};


inline double clamp(double lower, double num, double upper) {
    return num <= lower ? lower : num >= upper ? upper : num;
}


#endif

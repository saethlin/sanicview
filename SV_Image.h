#ifndef SANICVIEW_SV_IMAGE_H
#define SANICVIEW_SV_IMAGE_H

#include <vector>

template <typename T>
class SV_Image : public std::vector<T> {
public:
    SV_Image();
    SV_Image(T* data, int width, int height);
    SV_Image(int width, int height);
    int width() const;
    int height() const;
    T operator()(int x, int y) const {return std::vector<T>::operator[](y*width_impl+x);}
    T& operator()(int x, int y) {return std::vector<T>::operator[](y*width_impl+x);}

private:
    int width_impl = 0;
    int height_impl = 0;
};


double clamp(double, double, double);


#endif //SANICVIEW_SV_IMAGE_H

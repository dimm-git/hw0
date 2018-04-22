// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This is an example illustrating the use of the kkmeans object 
    and spectral_cluster() routine from the dlib C++ Library.

    The kkmeans object is an implementation of a kernelized k-means clustering 
    algorithm.  It is implemented by using the kcentroid object to represent 
    each center found by the usual k-means clustering algorithm.  

    So this object allows you to perform non-linear clustering in the same way 
    a svm classifier finds non-linear decision surfaces.  
    
    This example will make points from 3 classes and perform kernelized k-means 
    clustering on those points.  It will also do the same thing using spectral 
    clustering.

    The classes are as follows:
        - points very close to the origin
        - points on the circle of radius 10 around the origin
        - points that are on a circle of radius 4 but not around the origin at all
*/

#include <iostream>

#include "kfunc.h"

int main()
{
    samples_type samples;

    dlib::rand rnd;

    // we will make 50 points from each class
    const long num = 50;

    generate(samples, rnd, num, 0.5);
    generate(samples, rnd, num, 10);
    generate(samples, rnd, num, 4.0, 25, 25);

    for (auto& s : samples)
        std::cout << s(0) << ";" << s(1) << std::endl;

    return 0;
}

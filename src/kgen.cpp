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

void gen_example(samples_type& s, dlib::rand& rnd);
void gen_stars(samples_type& s, dlib::rand& rnd);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << "(0|1)" << std::endl << "1 - as a kkmeans example" << std::endl << "2 - as a starry sky" << std::endl;
        return 1;
    }
    samples_type samples;

    dlib::rand rnd;

    if (std::atoi(argv[1]) == 1)
        gen_stars(samples, rnd);
    else
        gen_example(samples, rnd);

    for (auto& s : samples)
        std::cout << s(0) << ";" << s(1) << std::endl;

    return 0;
}

void gen_example(samples_type& samples, dlib::rand& rnd)
{
    // we will make 50 points from each class
    const long num = 50;

    generate(samples, rnd, num, 0.5);
    generate(samples, rnd, num, 10);
    generate(samples, rnd, num, 4.0, 25, 25);
}

void gen_stars(samples_type& s, dlib::rand& rand)
{
    const long num = 200;
    sample_type m;
    const double width = 100;
    
    for (long i = 0; i < num; ++i)
    {
        m(0) = (2 * width) * rand.get_random_double() - width;
        m(1) = (2 * width) * rand.get_random_double() - width;
        s.push_back(m);
    }
}

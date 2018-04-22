#include "kfunc.h"

void generate(samples_type& s, dlib::rand& rand, long num, double radius, double shift_x, double shift_y)
{
    sample_type m;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rand.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2*radius*rand.get_random_double()-radius;
        m(1) = sign*sqrt(radius*radius - m(0)*m(0));

        // translate this point away from the origin
        m(0) += shift_x;
        m(1) += shift_y;

        // add this sample to our set of samples we will run k-means 
        s.push_back(m);
    }
}

bool read_sample(sample_type& s, std::istream& in)
{
    double val = 0.0;
    char c = 0;
    if ((in >> val))
    {
        s(0) = val;
        in >> c;
        if ((in >> val))
        {
            s(1) = val;
            return true;
        }
    }
    return false;
}

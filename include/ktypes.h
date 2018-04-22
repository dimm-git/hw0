#pragma once

#include <vector>

#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace std;
using namespace dlib;

// Here we declare that our samples will be 2 dimensional column vectors.  
// (Note that if you don't know the dimensionality of your vectors at compile time
// you can change the 2 to a 0 and then set the size at runtime)
typedef matrix<double,2,1> sample_type;

// Now we are making a typedef for the kind of kernel we want to use.  I picked the
// radial basis kernel because it only has one parameter and generally gives good
// results without much fiddling.
typedef radial_basis_kernel<sample_type> kernel_type;

typedef std::vector<sample_type> samples_type;

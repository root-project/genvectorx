// dictionary for std::vector of genvector objects
// problem on Windows: CINT cannot deal with  too long class name
// generated by an std::vector<ROOT::Experimental::Vector>

#pragma extra_include "vector";
#include <vector>

// conflict on solaris between template class T from std::vector and T().
#ifndef __sun
#pragma link C++ class std::vector<ROOT::Experimental::XYZTVector >+;
#pragma link C++ class std::vector<ROOT::Experimental::PtEtaPhiEVector >+;
#endif

#pragma link C++ class std::vector<ROOT::Experimental::XYZVector >+;
#pragma link C++ class std::vector<ROOT::Experimental::XYZPoint >+;

#pragma link C++ class std::vector<ROOT::Experimental::RhoEtaPhiVector >+;
#pragma link C++ class std::vector<ROOT::Experimental::RhoEtaPhiPoint >+;

// for floating types 

#pragma link C++ class std::vector<ROOT::Experimental::XYZVectorF >+;
#pragma link C++ class std::vector<ROOT::Experimental::XYZPointF >+;
#pragma link C++ class std::vector<ROOT::Experimental::XYZTVectorF >+;


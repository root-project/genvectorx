// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , FNAL MathLib Team                             *
  *                                                                    *
  *                                                                    *
  **********************************************************************/


// Header source file for function etaMax
//
// Created by: Mark Fischler  at Thu Jun 2 2005


#ifndef ROOT_Experimental_GenVector_etaMax
#define ROOT_Experimental_GenVector_etaMax  1

#include "SYCLMath/GenVector/MathUtil.h"

#include <limits>
#include <cmath>


namespace ROOT {

  namespace Experimental {

    /**
        The following function could be called to provide the maximum possible
        value of pseudorapidity for a non-zero rho.  This is log ( max/min )
        where max and min are the extrema of positive values for type
        long double.
     */
#if defined(ROOT_MATH_SYCL) or defined(ROOT_MATH_HIP)  
__roohost__ __roodevice__
     double etaMax_impl() {
      return mylog ( std::numeric_limits< double>::max()/256.0 ) -
             mylog ( std::numeric_limits< double>::denorm_min()*256.0 )
             + 16.0 * log(2.0);
    // Actual usage of etaMax() simply returns the number 22756, which is
    // the answer this would supply, rounded to a higher integer.
    }
#else
__roohost__ __roodevice__ 
     long double etaMax_impl() {
      return mylog ( std::numeric_limits<long double>::max()/256.0 ) -
             mylog ( std::numeric_limits<long double>::denorm_min()*256.0 )
             + 16.0 * log(2.0);
    // Actual usage of etaMax() simply returns the number 22756, which is
    // the answer this would supply, rounded to a higher integer.
    }
#endif

    /**
        Function providing the maximum possible value of pseudorapidity for
        a non-zero rho, in the Scalar type with the largest dynamic range.
     */
    template <class T>
    inline
  __roohost__ __roodevice__  T etaMax() {
      return static_cast<T>(22756.0);
    }

  } // namespace Experimental

} // namespace ROOT


#endif /* ROOT_Experimental_GenVector_etaMax  */

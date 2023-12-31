// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 ROOT FNAL MathLib Team                          *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for Boost
//
// Created by: Mark Fischler  Mon Nov 1  2005
//
// Last update: $Id$
//
#ifndef ROOT_Experimental_GenVector_Boost
#define ROOT_Experimental_GenVector_Boost 1

#include "SYCLMath/GenVector/MathUtil.h"

#include "SYCLMath/GenVector/LorentzVector.h"
#include "SYCLMath/GenVector/PxPyPzE4D.h"
#include "SYCLMath/GenVector/DisplacementVector3D.h"
#include "SYCLMath/GenVector/Cartesian3D.h"

#include "SYCLMath/GenVector/BoostX.h"
#include "SYCLMath/GenVector/BoostY.h"
#include "SYCLMath/GenVector/BoostZ.h"

namespace ROOT {

  namespace Experimental {

//__________________________________________________________________________________________
  /**
     Lorentz boost class with the (4D) transformation represented internally
     by a 4x4 orthosymplectic matrix.
     See also BoostX, BoostY and BoostZ for classes representing
     specialized Lorentz boosts.
     Also, the 3-D rotation classes can be considered to be special Lorentz
     transformations which do not mix space and time components.

     @ingroup GenVector

     @sa Overview of the @ref GenVector "physics vector library"
  */

class Boost {

public:

  typedef double Scalar;

  enum ELorentzRotationMatrixIndex {
      kLXX =  0, kLXY =  1, kLXZ =  2, kLXT =  3
    , kLYX =  4, kLYY =  5, kLYZ =  6, kLYT =  7
    , kLZX =  8, kLZY =  9, kLZZ = 10, kLZT = 11
    , kLTX = 12, kLTY = 13, kLTZ = 14, kLTT = 15
  };

  enum EBoostMatrixIndex {
      kXX =  0, kXY =  1, kXZ =  2, kXT =  3
     , kYY =  4, kYZ =  5, kYT =  6
     , kZZ =  7, kZT =  8
     , kTT =  9
  };

  // ========== Constructors and Assignment =====================

  /**
      Default constructor (identity transformation)
  */
  __roohost__ __roodevice__ Boost() { SetIdentity(); }

  /**
     Construct given a three Scalars beta_x, beta_y, and beta_z
   */
  __roohost__ __roodevice__ Boost(Scalar beta_x, Scalar beta_y, Scalar beta_z)
   { SetComponents(beta_x, beta_y, beta_z); }

  /**
     Construct given a beta vector (which must have methods x(), y(), z())
   */
  template <class Avector>
  __roohost__ __roodevice__ explicit
  Boost(const Avector & beta) { SetComponents(beta); }

  /**
     Construct given a pair of pointers or iterators defining the
     beginning and end of an array of three Scalars to use as beta_x, _y, and _z
   */
  template<class IT>
  __roohost__ __roodevice__
  Boost(IT begin, IT end) { SetComponents(begin,end); }

   /**
      copy constructor
   */
   __roohost__ __roodevice__ Boost(Boost const & b) {
      *this = b;
   }

  /**
     Construct from an axial boost
  */

  __roohost__ __roodevice__ explicit Boost( BoostX const & bx ) {SetComponents(bx.BetaVector());}
  __roohost__ __roodevice__ explicit Boost( BoostY const & by ) {SetComponents(by.BetaVector());}
  __roohost__ __roodevice__ explicit Boost( BoostZ const & bz ) {SetComponents(bz.BetaVector());}


  // The compiler-generated copy ctor, copy assignment, and dtor are OK.

   /**
      Assignment operator
    */
  __roohost__ __roodevice__ Boost &
   operator=(Boost const & rhs ) {
    for (unsigned int i=0; i < 10; ++i) {
       fM[i] = rhs.fM[i];
    }
    return *this;
   }

  /**
     Assign from an axial pure boost
  */

  __roohost__ __roodevice__ Boost &
  operator=( BoostX const & bx ) { return operator=(Boost(bx)); }
  __roohost__ __roodevice__ Boost &
  operator=( BoostY const & by ) { return operator=(Boost(by)); }
  __roohost__ __roodevice__ Boost &
  operator=( BoostZ const & bz ) { return operator=(Boost(bz)); }

  /**
     Re-adjust components to eliminate small deviations from a perfect
     orthosyplectic matrix.
   */
  __roohost__ __roodevice__ void Rectify();

  // ======== Components ==============

  /**
     Set components from beta_x, beta_y, and beta_z
  */
  //void
  //SetComponents (Scalar beta_x, Scalar beta_y, Scalar beta_z);
__roohost__ __roodevice__ void SetComponents (Scalar bx, Scalar by, Scalar bz) {
   // set the boost beta as 3 components
   Scalar bp2 = bx*bx + by*by + bz*bz;
   if (bp2 >= 1) {
      //GenVector::Throw (
      //                        "Beta Vector supplied to set Boost represents speed >= c");
      SetIdentity();
      return;
   }
   Scalar gamma = 1.0 / mysqrt(1.0 - bp2);
   Scalar bgamma = gamma * gamma / (1.0 + gamma);
   fM[kXX] = 1.0 + bgamma * bx * bx;
   fM[kYY] = 1.0 + bgamma * by * by;
   fM[kZZ] = 1.0 + bgamma * bz * bz;
   fM[kXY] = bgamma * bx * by;
   fM[kXZ] = bgamma * bx * bz;
   fM[kYZ] = bgamma * by * bz;
   fM[kXT] = gamma * bx;
   fM[kYT] = gamma * by;
   fM[kZT] = gamma * bz;
   fM[kTT] = gamma;
}
  /**
     Get components into beta_x, beta_y, and beta_z
  */
  __roohost__ __roodevice__ void
  GetComponents (Scalar& beta_x, Scalar& beta_y, Scalar& beta_z) const;

  /**
     Set components from a beta vector
  */
  template <class Avector>
  __roohost__ __roodevice__ void
  SetComponents (const Avector & beta)
   { SetComponents(beta.x(), beta.y(), beta.z()); }

  /**
     Set given a pair of pointers or iterators defining the beginning and end of
     an array of three Scalars to use as beta_x,beta _y, and beta_z
   */
  template<class IT>
  __roohost__ __roodevice__ void SetComponents(IT begin, IT end) {
    IT a = begin; IT b = ++begin; IT c = ++begin;
    (void)end;
    assert (++begin==end);
    SetComponents (*a, *b, *c);
  }

  /**
     Get given a pair of pointers or iterators defining the beginning and end of
     an array of three Scalars into which to place beta_x, beta_y, and beta_z
   */
  template<class IT>
  __roohost__ __roodevice__ void GetComponents(IT begin, IT end) const {
    IT a = begin; IT b = ++begin; IT c = ++begin;
    (void)end;
    assert (++begin==end);
    GetComponents (*a, *b, *c);
  }

  /**
     Get given a pointer or an iterator defining the beginning of
     an array into which to place beta_x, beta_y, and beta_z
   */
  template<class IT>
  __roohost__ __roodevice__ void GetComponents(IT begin ) const {
     double bx,by,bz = 0;
     GetComponents (bx,by,bz);
     *begin++ = bx;
     *begin++ = by;
     *begin = bz;
  }

  /**
     The beta vector for this boost
   */
  typedef  DisplacementVector3D<Cartesian3D<double>, DefaultCoordinateSystemTag > XYZVector;
  __roohost__ __roodevice__ XYZVector BetaVector() const;

  /**
     Get elements of internal 4x4 symmetric representation, into a data
     array suitable for direct use as the components of a LorentzRotation
     Note -- 16 Scalars will be written into the array; if the array is not
     that large, then this will lead to undefined behavior.
  */
  __roohost__ __roodevice__ void
  GetLorentzRotation (Scalar r[]) const;

  // =========== operations ==============

  /**
     Lorentz transformation operation on a Minkowski ('Cartesian')
     LorentzVector
  */

  //LorentzVector< PxPyPzE4D<double> >
  //operator() (const LorentzVector< PxPyPzE4D<double> > & v) const;
  __roohost__ __roodevice__ LorentzVector< PxPyPzE4D<double> >
  operator() (const LorentzVector< PxPyPzE4D<double> > & v) const {
   // apply bosost to a PxPyPzE LorentzVector
   Scalar x = v.Px();
   Scalar y = v.Py();
   Scalar z = v.Pz();
   Scalar t = v.E();
   return LorentzVector< PxPyPzE4D<double> >
      ( fM[kXX]*x + fM[kXY]*y + fM[kXZ]*z + fM[kXT]*t
        , fM[kXY]*x + fM[kYY]*y + fM[kYZ]*z + fM[kYT]*t
        , fM[kXZ]*x + fM[kYZ]*y + fM[kZZ]*z + fM[kZT]*t
        , fM[kXT]*x + fM[kYT]*y + fM[kZT]*z + fM[kTT]*t );
}

  /**
     Lorentz transformation operation on a LorentzVector in any
     coordinate system
   */
  template <class CoordSystem>
  __roohost__ __roodevice__ LorentzVector<CoordSystem>
  operator() (const LorentzVector<CoordSystem> & v) const {
    LorentzVector< PxPyPzE4D<double> > xyzt(v);
    LorentzVector< PxPyPzE4D<double> > r_xyzt = operator()(xyzt);
    return LorentzVector<CoordSystem> ( r_xyzt );
  }

  /**
     Lorentz transformation operation on an arbitrary 4-vector v.
     Preconditions:  v must implement methods x(), y(), z(), and t()
     and the arbitrary vector type must have a constructor taking (x,y,z,t)
   */
  template <class Foreign4Vector>
  __roohost__ __roodevice__ Foreign4Vector
  operator() (const Foreign4Vector & v) const {
    LorentzVector< PxPyPzE4D<double> > xyzt(v);
    LorentzVector< PxPyPzE4D<double> > r_xyzt = operator()(xyzt);
    return Foreign4Vector ( r_xyzt.X(), r_xyzt.Y(), r_xyzt.Z(), r_xyzt.T() );
  }

  /**
     Overload operator * for boost on a vector
   */
  template <class A4Vector>
  __roohost__ __roodevice__
  A4Vector operator* (const A4Vector & v) const
  {
    return operator()(v);
  }

  /**
      Invert a Boost in place
   */
  __roohost__ __roodevice__ void Invert();

  /**
      Return inverse of  a boost
   */
  __roohost__ __roodevice__ Boost Inverse() const;

  /**
     Equality/inequality operators
   */
  __roohost__ __roodevice__ bool operator == (const Boost & rhs) const {
    for (unsigned int i=0; i < 10; ++i) {
      if( fM[i] != rhs.fM[i] )  return false;
    }
    return true;
  }
  __roohost__ __roodevice__ bool operator != (const Boost & rhs) const {
    return ! operator==(rhs);
  }

protected:

  //void SetIdentity();

__roohost__ __roodevice__ void SetIdentity() {
   // set identity boost
   fM[kXX] = 1.0;  fM[kXY] = 0.0; fM[kXZ] = 0.0; fM[kXT] = 0.0;
   fM[kYY] = 1.0; fM[kYZ] = 0.0; fM[kYT] = 0.0;
   fM[kZZ] = 1.0; fM[kZT] = 0.0;
   fM[kTT] = 1.0;
}

private:

  Scalar fM[10];

};  // Boost

// ============ Class Boost ends here ============

/**
   Stream Output and Input
 */
  // TODO - I/O should be put in the manipulator form

//std::ostream & operator<< (std::ostream & os, const Boost & b);


} //namespace Experimental
} //namespace ROOT







#endif /* ROOT_Experimental_GenVector_Boost  */

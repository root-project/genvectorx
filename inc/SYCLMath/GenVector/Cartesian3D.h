// @(#)root/mathcore:$Id: 2fd203872f434b1e4e74933903abb3429494ea6f $
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG ROOT MathLib Team                         *
  *                    & FNAL LCG ROOT Mathlib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for class Cartesian3D
//
// Created by: Lorenzo Moneta  at Mon May 30 11:16:56 2005
// Major revamp:  M. FIschler  at Wed Jun  8 2005
//
// Last update: $ID: $
//
#ifndef ROOT_Experimental_GenVector_Cartesian3D
#define ROOT_Experimental_GenVector_Cartesian3D  1

#undef __MAKECINT__

#include "SYCLMath/GenVector/Polar3Dfwd.h"

#include <limits>
#include <cmath>

#include "SYCLMath/GenVector/MathUtil.h"

namespace ROOT {

namespace Experimental {

//__________________________________________________________________________________________
  /**
      Class describing a 3D cartesian coordinate system
      (x, y, z coordinates)

      @ingroup GenVector

      @sa Overview of the @ref GenVector "physics vector library"
  */

template <class T = double>
class Cartesian3D {

public :

   typedef T Scalar;

   /**
      Default constructor  with x=y=z=0
   */
   Cartesian3D() : fX(0.0), fY(0.0), fZ(0.0) {  }

   /**
      Constructor from x,y,z coordinates
   */
   Cartesian3D(Scalar xx, Scalar yy, Scalar zz) : fX(xx), fY(yy), fZ(zz) {  }

   /**
      Construct from any Vector or coordinate system implementing
      X(), Y() and Z()
   */
   template <class CoordSystem>
   explicit Cartesian3D(const CoordSystem & v)
      : fX(v.X()), fY(v.Y()), fZ(v.Z()) {  }

   // for g++  3.2 and 3.4 on 32 bits found that the compiler generated copy ctor and assignment are much slower
   // re-implement them ( there is no no need to have them with g++4)
   /**
      copy constructor
    */
   Cartesian3D(const Cartesian3D & v) :
      fX(v.X()), fY(v.Y()), fZ(v.Z()) {  }

   /**
      assignment operator
    */
   Cartesian3D & operator= (const Cartesian3D & v) {
      fX = v.x();
      fY = v.y();
      fZ = v.z();
      return *this;
   }

   /**
      Set internal data based on an array of 3 Scalar numbers
   */
   void SetCoordinates( const Scalar src[] ) { fX=src[0]; fY=src[1]; fZ=src[2]; }

   /**
      get internal data into an array of 3 Scalar numbers
   */
   void GetCoordinates( Scalar dest[] ) const
   { dest[0] = fX; dest[1] = fY; dest[2] = fZ; }

   /**
      Set internal data based on 3 Scalar numbers
   */
   void SetCoordinates(Scalar  xx, Scalar  yy, Scalar  zz) { fX=xx; fY=yy; fZ=zz; }

   /**
      get internal data into 3 Scalar numbers
   */
   void GetCoordinates(Scalar& xx, Scalar& yy, Scalar& zz) const {xx=fX; yy=fY; zz=fZ;}

   Scalar X()     const { return fX;}
   Scalar Y()     const { return fY;}
   Scalar Z()     const { return fZ;}
   Scalar Mag2()  const { return fX*fX + fY*fY + fZ*fZ;}
   Scalar Perp2() const { return fX*fX + fY*fY ;}
   Scalar Rho() const { return mysqrt(Perp2()); }
   Scalar R() const { return mysqrt(Mag2()); }
   Scalar Theta() const { return myatan2(Rho(), Z()); }
   Scalar Phi() const { return myatan2(fY, fX); }

   // pseudorapidity
   Scalar Eta() const {
      return Eta_FromRhoZ( Rho(), fZ );
   }

   /**
       set the x coordinate value keeping y and z constant
   */
   void SetX(Scalar xx) { fX = xx; }

   /**
       set the y coordinate value keeping x and z constant
   */
   void SetY(Scalar yy) { fY = yy; }

   /**
       set the z coordinate value keeping x and y constant
   */
   void SetZ(Scalar zz) { fZ = zz; }

   /**
       set all values using cartesian coordinates
   */
   void SetXYZ(Scalar xx, Scalar yy, Scalar zz) {
      fX=xx;
      fY=yy;
      fZ=zz;
   }

   /**
      scale the vector by a scalar quantity a
   */
   void Scale(Scalar a)
   {
      fX *= a;
      fY *= a;
      fZ *= a;
   }

   /**
      negate the vector
   */
   void Negate() { fX = -fX; fY = -fY;  fZ = -fZ; }

   /**
      Assignment from any class implementing x(),y() and z()
      (can assign from any coordinate system)
   */
   template <class CoordSystem>
   Cartesian3D & operator = (const CoordSystem & v) {
      fX = v.x();
      fY = v.y();
      fZ = v.z();
      return *this;
   }

   /**
      Exact equality
   */
   bool operator == (const Cartesian3D & rhs) const {
      return fX == rhs.fX && fY == rhs.fY && fZ == rhs.fZ;
   }
   bool operator != (const Cartesian3D & rhs) const {return !(operator==(rhs));}


   // ============= Compatibility section ==================

   // The following make this coordinate system look enough like a CLHEP
   // vector that an assignment member template can work with either
   T x() const { return X();}
   T y() const { return Y();}
   T z() const { return Z(); }

   // ============= Overloads for improved speed ==================

   template <class T2>
   explicit Cartesian3D( const Polar3D<T2> & v ) : fZ (v.Z())
   {
      const T rho = v.Rho();
      // re-using this instead of calling v.X() and v.Y()
      // is the speed improvement
      fX = rho * mycos(v.Phi());
      fY = rho * mysin(v.Phi());
   }
   // Technical note:  This works even though only Polar3Dfwd.h is
   // included (and in fact, including Polar3D.h would cause circularity
   // problems). It works because any program **using** this ctor must itself
   // be including Polar3D.h.

   template <class T2>
   Cartesian3D & operator = (const Polar3D<T2> & v)
   {
      const T rho = v.Rho();
      fX          = rho * mycos(v.Phi());
      fY          = rho * mysin(v.Phi());
      fZ = v.Z();
      return *this;
   }



#if defined(__MAKECINT__) || defined(G__DICTIONARY)

   // ====== Set member functions for coordinates in other systems =======

   void SetR(Scalar r);

   void SetTheta(Scalar theta);

   void SetPhi(Scalar phi);

   void SetRho(Scalar rho);

   void SetEta(Scalar eta);

#endif


private:

   T fX;  // x coordinate
   T fY;  // y coordinate
   T fZ;  // z coordinate
};


  } // end namespace Experimental

} // end namespace ROOT


#if defined(__MAKECINT__) || defined(G__DICTIONARY)
// need to put here setter methods to resolve nasty cyclical dependencies
// I need to include other coordinate systems only when Cartesian is already defined
// since they depend on it
#include "SYCLMath/GenVector/GenVector_exception.h"
#include "SYCLMath/GenVector/CylindricalEta3D.h"
#include "SYCLMath/GenVector/Polar3D.h"

  // ====== Set member functions for coordinates in other systems =======

namespace ROOT {

  namespace Experimental {

template <class T>
void Cartesian3D<T>::SetR(Scalar r) {
   GenVector_exception e("Cartesian3D::SetR() is not supposed to be called");
   throw e;
   Polar3D<Scalar> v(*this); v.SetR(r); *this = Cartesian3D<Scalar>(v);
}

template <class T>
void Cartesian3D<T>::SetTheta(Scalar theta) {
   GenVector_exception e("Cartesian3D::SetTheta() is not supposed to be called");
   throw e;
   Polar3D<Scalar> v(*this); v.SetTheta(theta); *this = Cartesian3D<Scalar>(v);
}

template <class T>
void Cartesian3D<T>::SetPhi(Scalar phi) {
   GenVector_exception e("Cartesian3D::SetPhi() is not supposed to be called");
   throw e;
   Polar3D<Scalar> v(*this); v.SetPhi(phi); *this = Cartesian3D<Scalar>(v);
}

template <class T>
void Cartesian3D<T>::SetRho(Scalar rho) {
   GenVector_exception e("Cartesian3D::SetRho() is not supposed to be called");
   throw e;
   CylindricalEta3D<Scalar> v(*this); v.SetRho(rho);
   *this = Cartesian3D<Scalar>(v);
}

template <class T>
void Cartesian3D<T>::SetEta(Scalar eta) {
   GenVector_exception e("Cartesian3D::SetEta() is not supposed to be called");
   throw e;
   CylindricalEta3D<Scalar> v(*this); v.SetEta(eta);
    *this = Cartesian3D<Scalar>(v);
}



  } // end namespace Experimental

} // end namespace ROOT

#endif




#endif /* ROOT_Experimental_GenVector_Cartesian3D  */

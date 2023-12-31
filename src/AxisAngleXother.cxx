// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG ROOT FNAL MathLib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for multiplications of AxisAngle by a rotation
//
// Created by: Mark Fischler Tues July 5  2005
//

#include "SYCLMath/GenVector/AxisAngle.h"

#include <cmath>

#include "SYCLMath/GenVector/Quaternion.h"
#include "SYCLMath/GenVector/Cartesian3D.h"
#include "SYCLMath/GenVector/RotationX.h"
#include "SYCLMath/GenVector/RotationY.h"
#include "SYCLMath/GenVector/RotationZ.h"

#include "SYCLMath/GenVector/Rotation3Dfwd.h"
#include "SYCLMath/GenVector/EulerAnglesfwd.h"

namespace ROOT {

namespace Experimental {


AxisAngle AxisAngle::operator * (const Rotation3D & r) const {
   // combination with a Rotation3D
   return operator* ( Quaternion(r) );

}

AxisAngle AxisAngle::operator * (const AxisAngle   & a) const {
   // combination with another AxisAngle
   return operator* ( Quaternion(a) );
}

AxisAngle AxisAngle::operator * (const EulerAngles & e) const {
   // combination with EulerAngles
   return operator* ( Quaternion(e) );
}

AxisAngle AxisAngle::operator * (const RotationZYX & r) const {
   // combination with RotationZYX
   return operator* ( Quaternion(r) );
}

AxisAngle AxisAngle::operator * (const Quaternion  & q) const {
   // combination with Quaternion
   return AxisAngle ( Quaternion(*this) * q );
}

#ifdef MOVE

AxisAngle AxisAngle::operator * (const Quaternion  & q) const {
   // combination with quaternion (not used)
   const Scalar s1 = mysin(fAngle/2);
   const Scalar au = mycos(fAngle/2);
   const Scalar ai = s1 * fAxis.X();
   const Scalar aj = s1 * fAxis.Y();
   const Scalar ak = s1 * fAxis.Z();
   Scalar aqu = au*q.U() - ai*q.I() -  aj*q.J() -  ak*q.K();
   Scalar aqi = au*q.I() + ai*q.U() +  aj*q.K() -  ak*q.J();
   Scalar aqj = au*q.J() - ai*q.K() +  aj*q.U() +  ak*q.I();
   Scalar aqk = au*q.K() + ai*q.J() -  aj*q.I() +  ak*q.U();
   Scalar r = mysqrt(aqi*aqi + aqj*aqj + aqk*aqk);
   if (r > 1) r = 1;
   if (aqu < 0) { aqu = - aqu; aqi = - aqi; aqj = - aqj, aqk = - aqk; }
   const Scalar angle = 2*myasin ( r );
   DisplacementVector3D< Cartesian3D<double> > axis ( aqi, aqj, aqk );
   if ( r == 0 ) {
      axis.SetCoordinates(0,0,1);
   } else {
      axis /= r;
   }
   return AxisAngle ( axis, angle );
}

#endif

AxisAngle AxisAngle::operator * (const RotationX & rx) const {
   // combination with RotationX

   const Scalar s1 = mysin(fAngle/2);
   const Scalar au = mycos(fAngle/2);
   const Scalar ai = s1 * fAxis.X();
   const Scalar aj = s1 * fAxis.Y();
   const Scalar ak = s1 * fAxis.Z();
   Scalar c  = rx.CosAngle();
   if ( c > 1  ) c =  1;
   if ( c < -1 ) c = -1;
   Scalar qu = mysqrt( .5*(1+c) );
   Scalar qi = mysqrt( .5*(1-c) );
   if ( rx.SinAngle() < 0 ) qi = -qi;
   Scalar aqu = au*qu - ai*qi;
   Scalar aqi = ai*qu + au*qi;
   Scalar aqj = aj*qu + ak*qi;
   Scalar aqk = ak*qu - aj*qi;
   Scalar r = mysqrt(aqi*aqi + aqj*aqj + aqk*aqk);
   if (r > 1) r = 1;
   if (aqu < 0) { aqu = - aqu; aqi = - aqi; aqj = - aqj, aqk = - aqk; }
   const Scalar angle = 2*myasin ( r );
   DisplacementVector3D< Cartesian3D<double> > axis ( aqi, aqj, aqk );
   if ( r == 0 ) {
      axis.SetCoordinates(0,0,1);
   } else {
      axis /= r;
   }
   return AxisAngle ( axis, angle );
}

AxisAngle AxisAngle::operator * (const RotationY & ry) const {
   // combination with RotationY

   const Scalar s1 = mysin(fAngle/2);
   const Scalar au = mycos(fAngle/2);
   const Scalar ai = s1 * fAxis.X();
   const Scalar aj = s1 * fAxis.Y();
   const Scalar ak = s1 * fAxis.Z();
   Scalar c  = ry.CosAngle();
   if ( c > 1  ) c =  1;
   if ( c < -1 ) c = -1;
   Scalar qu = mysqrt( .5*(1+c) );
   Scalar qj = mysqrt( .5*(1-c) );
   if ( ry.SinAngle() < 0 ) qj = -qj;
   Scalar aqu = au*qu - aj*qj;
   Scalar aqi = ai*qu - ak*qj;
   Scalar aqj = aj*qu + au*qj;
   Scalar aqk = ak*qu + ai*qj;
   Scalar r = mysqrt(aqi*aqi + aqj*aqj + aqk*aqk);
   if (r > 1) r = 1;
   if (aqu < 0) { aqu = - aqu; aqi = - aqi; aqj = - aqj, aqk = - aqk; }
   const Scalar angle = 2*myasin ( r );
   DisplacementVector3D< Cartesian3D<double> > axis ( aqi, aqj, aqk );
   if ( r == 0 ) {
      axis.SetCoordinates(0,0,1);
   } else {
      axis /= r;
   }
   return AxisAngle ( axis, angle );
}

AxisAngle AxisAngle::operator * (const RotationZ & rz) const {
   // combination with RotationZ

   const Scalar s1 = mysin(fAngle/2);
   const Scalar au = mycos(fAngle/2);
   const Scalar ai = s1 * fAxis.X();
   const Scalar aj = s1 * fAxis.Y();
   const Scalar ak = s1 * fAxis.Z();
   Scalar c  = rz.CosAngle();
   if ( c > 1  ) c =  1;
   if ( c < -1 ) c = -1;
   Scalar qu = mysqrt( .5*(1+c) );
   Scalar qk = mysqrt( .5*(1-c) );
   if ( rz.SinAngle() < 0 ) qk = -qk;
   Scalar aqu = au*qu - ak*qk;
   Scalar aqi = ai*qu + aj*qk;
   Scalar aqj = aj*qu - ai*qk;
   Scalar aqk = ak*qu + au*qk;
   Scalar r = mysqrt(aqi*aqi + aqj*aqj + aqk*aqk);
   if (r > 1) r = 1;
   if (aqu < 0) { aqu = - aqu; aqi = - aqi; aqj = - aqj, aqk = - aqk; }
   const Scalar angle = 2*myasin ( r );
   DisplacementVector3D< Cartesian3D<double> > axis ( aqi, aqj, aqk );
   if ( r == 0 ) {
      axis.SetCoordinates(0,0,1);
   } else {
      axis /= r;
   }
   return AxisAngle ( axis, angle );
}

AxisAngle operator*( RotationX const & r, AxisAngle const & a ) {
   return AxisAngle(r) * a;  // TODO: improve performance
}

AxisAngle operator*( RotationY const & r, AxisAngle const & a ) {
   return AxisAngle(r) * a;  // TODO: improve performance
}

AxisAngle operator*( RotationZ const & r, AxisAngle const & a ) {
   return AxisAngle(r) * a;  // TODO: improve performance
}


} //namespace Experimental
} //namespace ROOT

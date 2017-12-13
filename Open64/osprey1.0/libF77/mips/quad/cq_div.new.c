/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

  Further, this software is distributed without any warranty that it is
  free of the rightful claim of any third person regarding infringement 
  or the like.  Any license provided herein, whether implied or 
  otherwise, applies only to this software file.  Patent licenses, if 
  any, provided herein do not apply to combinations of this program with 
  other software, or any other product whatsoever.  

  You should have received a copy of the GNU General Public License along
  with this program; if not, write the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston MA 02111-1307, USA.

  Contact information:  Silicon Graphics, Inc., 1600 Amphitheatre Pky,
  Mountain View, CA 94043, or:

  http://www.sgi.com

  For further information regarding this notice, see:

  http://oss.sgi.com/projects/GenInfo/NoticeExplan

*/


/* ====================================================================
 * ====================================================================
 *
 *
 * Description:  source code for complex quad division
 *
 * ====================================================================
 * ====================================================================
 */

/* #include <sys/fpu.h>  -- replaced with ieeefp for a portable interface */

#if !defined(__alpha) /* FIXME: need a replacement for alpha-Tru64 */
# include <ieeefp.h>
#endif
#include "quad.h"

  /* complex *32 division */

/* ====================================================================
 *
 * FunctionName    cq_div
 *
 * Description    computes complex quotient of args
 *
 * ====================================================================
 */

qcomplex __cqdiv(long double xqreal, long double xqimag, long double yqreal, long double yqimag)
{
  qcomplex result;

#if defined(__alpha) /* FIXME: need a replacement for alpha-Tru64 */

  result.qreal = 0;
  result.qimag = 0;

#else

  long double tmp;
  unsigned int m, n;
  fp_except oldmask; 
  fp_except oldsticky;

  if ((xqreal != xqreal) || (xqimag != xqimag) ||
       (yqreal != yqreal) || (yqimag != yqimag)) {
    result.qreal = __libm_qnan_ld;
    result.qimag = __libm_qnan_ld;
    return result;
  }

  if ((yqreal == 0.0L) && (yqimag == 0.0L)) {
    result.qreal = xqreal/__libm_zero_ld;
    result.qimag = xqimag/__libm_zero_ld;
    return result;
  }

  if (yqreal == 0.0L) {
    result.qreal = xqimag/yqimag;
    result.qimag = -(xqreal/yqimag);
    return result;
  }

  if (yqimag == 0.0L) {
    result.qreal = xqreal/yqreal;
    result.qimag = xqimag/yqreal;
    return result;
  }

  if (__qabs(yqreal) <= __qabs(yqimag)) {
    /* turn off traps on underflow while computing
       yqreal/yqimag
    */

    /* record fpu exception handling state */
    oldmask = fpgetmask();
    oldsticky = fpgetsticky();

    /* suppress underflow exceptions */
    fpsetmask(oldmask & ~FP_X_UFL);

    tmp = yqreal/yqimag;

    /* restore underflow handling to its previous state */
    fpsetmask(oldmask);
    fpsetsticky(oldsticky);

    result.qreal = (xqimag + xqreal*tmp)/(yqimag + yqreal*tmp);
    result.qimag = (-xqreal + xqimag*tmp)/(yqimag + yqreal*tmp);
    return result;
  }

  /* turn off traps on underflow while computing
     yqimag/yqreal
  */
    /* record fpu exception handling state */
  oldmask = fpgetmask();
  oldsticky = fpgetsticky();

  /* suppress underflow exceptions */
  fpsetmask(oldmask & ~FP_X_UFL);

  tmp = yqimag/yqreal;

  /* restore underflow handling to its previous state */
  fpsetmask(oldmask);
  fpsetsticky(oldsticky);

  result.qreal = (xqreal + xqimag*tmp)/(yqreal + yqimag*tmp);
  result.qimag = (xqimag - xqreal*tmp)/(yqreal + yqimag*tmp);

#endif /* __alpha */

  return result;
}

void __cq_div(qcomplex *result, qcomplex *x, qcomplex *y)
{
  *result = __cqdiv(x->qreal, x->qimag, y->qreal, y->qimag);
}

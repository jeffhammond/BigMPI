/* LICENSE NOTE
 *
 * Depending on how aggressively one interprets the LGPL,
 * this file may be infected with LGPL, by virtue of the
 * author having looked at mpfr.h in order to know
 * the names of various preprocessor symbols and an
 * internal struct name.
 *
 * All code other than that which references mpfr.h
 * names is covered by the BSD-2 license. */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include <string.h> /* memcpy */

#include <mpi.h>

#include <mpfr.h>

/* mpfr_t consists of:
 * prec(ision)  - one short/int/long
 * sign         - one int
 * exp(onent)   - one short/int/long/intmax_t
 *
 */

/* I learned about these preprocessor symbols by
 * reading mpfr.h, which is LGPL.  If preprocessor
 * symbols carry the Stallman virus, we will replace
 * these checks later with a configure test that
 * blindly evaluates the width and signedness of
 * mpfr_prec_t and mpfr_exp_t directly. */
#ifndef _MPFR_PREC_FORMAT
# error _MPFR_PREC_FORMAT needed.
#endif

#if (_MPFR_PREC_FORMAT==1)
# define MPFR_PREC_TYPE MPI_SHORT
#elif (_MPFR_PREC_FORMAT==2)
# define MPFR_PREC_TYPE MPI_INT
#elif (_MPFR_PREC_FORMAT==3)
# define MPFR_PREC_TYPE MPI_LONG
#endif

#define MPFR_SIGN_TYPE MPI_INT

#ifndef _MPFR_EXP_FORMAT
# error _MPFR_EXP_FORMAT needed.
#endif

#if (_MPFR_EXP_FORMAT==1)
# define MPFR_EXP_TYPE MPI_SHORT
# define MPFR_EXP_COUNT 1
#elif (_MPFR_EXP_FORMAT==2)
# define MPFR_EXP_TYPE MPI_INT
# define MPFR_EXP_COUNT 1
#elif (_MPFR_EXP_FORMAT==3)
# define MPFR_EXP_TYPE MPI_LONG
# define MPFR_EXP_COUNT 1
#elif (_MPFR_EXP_FORMAT==4)
# define MPFR_EXP_TYPE MPI_BYTE
# define MPFR_EXP_COUNT sizeof(intmax_t)
#endif
/* end MPFR header preprocessor symbols */

MPI_Datatype make_mpfr_datatype(mpfr_t mpfr)
{
    MPI_Datatype mpfr_datatype = MPI_DATATYPE_NULL;

    MPI_Datatype mpfr_limb_datatype;
    MPI_Aint     mpfr_limb_offset;

    const int    n = 4;
    int          counts[4] = { 1, 1, MPFR_EXP_COUNT, 1};
    /* Cannot use mpfr_t here because not clean typedef.
     * Obviously, I had to look at the header to know the struct name. */
    MPI_Aint     displs[4] = { offsetof(__mpfr_struct,_mpfr_prec),
                               offsetof(__mpfr_struct,_mpfr_sign),
                               offsetof(__mpfr_struct,_mpfr_exp),
                               mpfr_limb_offset };
    MPI_Datatype dtypes[4] = { MPFR_PREC_TYPE, MPFR_SIGN_TYPE, MPFR_EXP_TYPE, mpfr_limb_datatype};
    MPI_Type_create_struct(n,counts,displs,dtypes,&mpfr_datatype);

    MPI_Type_commit(&mpfr_datatype);
    return mpfr_datatype;
}

int main(void)
{
    MPI_Init(NULL,NULL);

    MPI_Finalize();
    return 0;
}

#include "bigmpi_impl.h"

#define PASTE_MPI128_REDUCE_OP(OPNAME,OPMATH)                                           \
void MPI128_##OPNAME(void * invec, void * inoutvec, int * len, MPI_Datatype * type)     \
{                                                                                       \
    if ((*type)==MPIX_GCC_FLOAT128) {                                                   \
        /* derived from MPICH (src/include/oputil.h) */                                 \
              __float128 * restrict a = (__float128 *)inoutvec;                         \
        const __float128 * restrict b = (__float128 *)invec;                            \
        for (int i=0; i<(*len); i++ ) {                                                 \
            a[i] = OPMATH(a[i],b[i]);                                                   \
        }                                                                               \
        /* end from MPICH */                                                            \
    } else {                                                                            \
        MPI_Reduce_local(invec, inoutvec, *len, *type, MPI_##OPNAME);                   \
    }                                                                                   \
    return;                                                                             \
}

/* modification derived from MPICH */
#define MPI128_BAND(a,b) ((a)&(b))
#define MPI128_BOR(a,b) ((a)|(b))
#define MPI128_BXOR(a,b) ((a)^(b))
#define MPI128_LAND(a,b) ((a)&&(b))
#define MPI128_LOR(a,b) ((a)||(b))
#define MPI128_LXOR(a,b) (((a)&&(!b))||((!a)&&(b)))
#define MPI128_PROD(a,b) ((a)*(b))
#define MPI128_SUM(a,b) ((a)+(b))
#define MPI128_MIN(a,b) (((a)>(b))?(b):(a))
#define MPI128_MAX(a,b) (((b)>(a))?(b):(a))
/* end from MPICH */

/* Create a BigMPI_<op>_x for all built-in ops. */
PASTE_MPI128_REDUCE_OP(MAX, MPI128_MAX)
PASTE_MPI128_REDUCE_OP(MIN, MPI128_MIN)
PASTE_MPI128_REDUCE_OP(SUM, MPI128_SUM)
PASTE_MPI128_REDUCE_OP(PROD,MPI128_PROD)
PASTE_MPI128_REDUCE_OP(LAND,MPI128_LAND)
PASTE_MPI128_REDUCE_OP(LOR, MPI128_LOR)
PASTE_MPI128_REDUCE_OP(LXOR,MPI128_LXOR)
/* TODO
PASTE_MPI128_REDUCE_OP(MAXLOC,MPI128_LMAXLOC)
PASTE_MPI128_REDUCE_OP(MINLOC,MPI128_LMINLOC)
*/

#undef MPI128_BAND
#undef MPI128_BOR
#undef MPI128_BXOR
#undef MPI128_LAND
#undef MPI128_LOR
#undef MPI128_LXOR
#undef MPI128_PROD
#undef MPI128_SUM
#undef MPI128_MIN
#undef MPI128_MAX

#undef PASTE_MPI128_REDUCE_OP

int MPI128_Op_create(MPI_Op op, MPI_Op * bigop)
{
    MPI_User_function * bigfn = NULL;

    if      (op==MPI_MAX)    bigfn = MPI128_MAX;
    else if (op==MPI_MIN)    bigfn = MPI128_MIN;
    else if (op==MPI_SUM)    bigfn = MPI128_SUM;
    else if (op==MPI_PROD)   bigfn = MPI128_PROD;
    else if (op==MPI_LAND)   bigfn = MPI128_LAND;
    else if (op==MPI_LOR)    bigfn = MPI128_LOR;
    else if (op==MPI_LXOR)   bigfn = MPI128_LXOR;
    /* Bitwise ops are not valid on floating-point types. */
    /* TODO
    else if (op==MPI_MAXLOC) bigfn = MPI128_MAXLOC;
    else if (op==MPI_MINLOC) bigfn = MPI128_MINLOC;
    */
    else {
        BigMPI_Error("MPI128 does not support this op.  Sorry. \n");
    }

    int commute;
    MPI_Op_commutative(op, &commute);

    return MPI_Op_create(bigfn, commute, bigop);
}

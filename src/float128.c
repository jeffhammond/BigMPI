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
#define MPI128_LBAND(a,b) ((a)&(b))
#define MPI128_LBOR(a,b) ((a)|(b))
#define MPI128_LBXOR(a,b) ((a)^(b))
#define MPI128_LLAND(a,b) ((a)&&(b))
#define MPI128_LLOR(a,b) ((a)||(b))
#define MPI128_LLXOR(a,b) (((a)&&(!b))||((!a)&&(b)))
#define MPI128_LPROD(a,b) ((a)*(b))
#define MPI128_LSUM(a,b) ((a)+(b))
#define MPI128_LMIN(a,b) (((a)>(b))?(b):(a))
#define MPI128_LMAX(a,b) (((b)>(a))?(b):(a))
/* end from MPICH */

/* Create a BigMPI_<op>_x for all built-in ops. */
PASTE_MPI128_REDUCE_OP(MAX, MPI128_LMAX)
PASTE_MPI128_REDUCE_OP(MIN, MPI128_LMIN)
PASTE_MPI128_REDUCE_OP(SUM, MPI128_LSUM)
PASTE_MPI128_REDUCE_OP(PROD,MPI128_LPROD)
PASTE_MPI128_REDUCE_OP(LAND,MPI128_LLAND)
PASTE_MPI128_REDUCE_OP(LOR, MPI128_LLOR)
PASTE_MPI128_REDUCE_OP(LXOR,MPI128_LLXOR)
/* TODO
PASTE_MPI128_REDUCE_OP(MAXLOC,MPI128_LMAXLOC)
PASTE_MPI128_REDUCE_OP(MINLOC,MPI128_LMINLOC)
*/

#undef MPI128_LBAND
#undef MPI128_LBOR
#undef MPI128_LBXOR
#undef MPI128_LLAND
#undef MPI128_LLOR
#undef MPI128_LLXOR
#undef MPI128_LPROD
#undef MPI128_LSUM
#undef MPI128_LMIN
#undef MPI128_LMAX

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

int MPI128_Allreduce(const void *sendbuf, void *recvbuf, int count,
                     MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    if (likely (datatype==MPIX_GCC_FLOAT128)) {
        MPI_Op bigop;
        MPI128_Op_create(op, &bigop);
        MPI_Allreduce(sendbuf, recvbuf, count, datatype, bigop, comm);
        MPI_Op_free(&bigop);
    } else {
        MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
    }
    return MPI_SUCCESS;
}

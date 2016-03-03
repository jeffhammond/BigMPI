#!/usr/bin/python

import fileinput
import string
import sys
import os

large_count_namespace = 'MPIX_'

def make_pre(f):
    f.write('#ifndef MPI_C11_H_INCLUDED\n'
           +'#define MPI_C11_H_INCLUDED\n\n'
           +'//#include "bigmpi.h"\n\n'
           +'#if !defined(__cplusplus) && defined(__STDC_VERSION__) && (__STDC_VERSION__ == 201112L)\n\n')

def make_post(f):
    f.write('#endif // C11 supported\n\n'
           +'#endif // MPI_C11_H_INCLUDED\n')

def make_user_function(f):
    f.write('typedef void MPI_User_function( void *invec, void *inoutvec, int *len, MPI_Datatype *datatype);\n'
           +'typedef void '+large_count_namespace+'_User_function_x( void *invec, void *inoutvec, int *MPI_Count, MPI_Datatype *datatype);\n'
           #+'int MPI_Op_create(MPI_User_function *function, int commute, MPI_Op *op);\n'
           +'int '+large_count_namespace+'_Op_create_x(MPI_User_function_x *function, int commute, MPI_Op *op);\n'
           +'#define MPI_Op_create(function, commute, op)'
           +'        _Generic((function)                          \\\n'
           +'                 MPI_User_function*:   MPI_Op_create, \\\n'
           +'                 '+large_count_namespace+'_User_function_x*: '+large_count_namespace+'_Op_create_x)(function, commute, op)\n\n')

# int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count);
# int MPI_Get_elements(const MPI_Status *status, MPI_Datatype datatype, int *count);
# int MPI_Get_elements_x(const MPI_Status *status, MPI_Datatype datatype, MPI_Count *count);
# int MPI_Type_get_extent(MPI_Datatype datatype, MPI_Aint *lb, MPI_Aint *extent)
# int MPI_Type_get_extent_x(MPI_Datatype datatype, MPI_Count *lb, MPI_Count *extent)
def make_get_count_etc(f):
    f.write('int MPI_Get_count_x(const MPI_Status *status, MPI_Datatype datatype, MPI_Count *count);\n'
           +'#define MPI_Get_count(status,datatype,count)'
           +'        _Generic((count)                          \\\n'
           +'                 int*:       MPI_Get_count, \\\n'
           +'                 MPI_Count*: '+large_count_namespace+'_Get_count_x)(status,datatype,count)\n\n'
           +'#define MPI_Get_elements(status,datatype,count)'
           +'        _Generic((count)                          \\\n'
           +'                 int*:       MPI_Get_elements, \\\n'
           +'                 MPI_Count*: MPI_Get_elements_x)(status,datatype,count)\n\n'
           +'#define MPI_Type_size(datatype,count)'
           +'        _Generic((size)                          \\\n'
           +'                 int*:       MPI_Type_size, \\\n'
           +'                 MPI_Count*: MPI_Type_size_x)(datatype,size)\n\n'
           +'#define MPI_Type_get_extent(datatype,lb,extent)'
           +'        _Generic((lb,extent)                          \\\n'
           +'                 (int*,int*):             MPI_Type_get_extent, \\\n'
           +'                 (MPI_Count*,MPI_Count*): MPI_Type_get_extent_x)(datatype,lb,extent)\n\n'
           +'#define MPI_Type_get_true_extent(datatype,lb,extent)'
           +'        _Generic((lb,extent)                          \\\n'
           +'                 (int*,int*):             MPI_Type_get_true_extent, \\\n'
           +'                 (MPI_Count*,MPI_Count*): MPI_Type_get_true_extent_x)(datatype,lb,extent)\n\n')


def make_mpi_c11_decl1s(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count)                          \\\n'
           +'                 int:                 MPI_' + name +  ',   \\\n'
           +'                 MPI_Count:           '+large_count_namespace+name+'_x, \\\n'
           +'                 default:             MPI_' + name +  ')' + args + '\n\n')

def make_mpi_c11_decl1v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((counts)                          \\\n'
           +'                 int*:                 MPI_' + name +  ',   \\\n'
           +'                 MPI_Count*:           '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl2s(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scount,rcount)                           \\\n'
           +'                 (int,int):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count):       '+large_count_namespace+name+'_x, \\\n'
           +'                 (default,default):           MPI_' + name +  ')' + args + '\n\n')

def make_mpi_c11_decl2sca(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count,disp)                           \\\n'
           +'                 (int,int):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Aint):        '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl1s1v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count,vcount)                           \\\n'
           +'                 (int,int*):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count*):       '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl1sc1va(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count,disps)                           \\\n'
           +'                 (int,int*):                  MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Aint*):       '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl2v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scounts,rcounts)                           \\\n'
           +'                 (int*,int*):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count*,MPI_Count*):       '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl3v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((counts1,counts2,counts3)                           \\\n'
           +'                 (const int*,const int*,const int*):                   MPI_' + name +  ',   \\\n'
           +'                 (const MPI_Count*,const MPI_Count*,const MPI_Count*): '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl2vca(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((counts,disps)                           \\\n'
           +'                 (const int*,const int*):                   MPI_' + name +  ',   \\\n'
           +'                 (const MPI_Count*,const MPI_Aint*):        '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl1s2v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count,counts,displs)                           \\\n'
           +'                 (int,int*,int*):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count*,MPI_Aint*):  '+large_count_namespace+name+'_x)' + args + '\n\n')

def make_mpi_c11_decl3s(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((ocount,rcount,tcount)                      \\\n'
           +'                 (int,int,int):                     MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count,MPI_Count):   '+large_count_namespace+name+'_x, \\\n'
           +'                 (default,default,default):         MPI_' + name +  ')' + args + '\n\n')

def make_mpi_c11_decl4v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scounts,sdispls,rcounts,rdispls)                           \\\n'
           +'                 (int*,int*,int*,int*):                        MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count*,MPI_Aint*,MPI_Count*,MPI_Aint*):  '+large_count_namespace+name+'_x)' + args + '\n\n')

mpi_interfaces = [
                  # point-to-point
                  ['1s','Send',             '(buf,count,type,dst,tag,comm)'],
                  ['1s','Ssend',            '(buf,count,type,dst,tag,comm)'],
                  ['1s','Rsend',            '(buf,count,type,dst,tag,comm)'],
                  ['1s','Bsend',            '(buf,count,type,dst,tag,comm)'],
                  ['1s','Isend',            '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Issend',           '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Irsend',           '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Ibsend',           '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Recv',             '(buf,count,type,src,tag,comm,stat)'],
                  ['1s','Irecv',            '(buf,count,type,src,tag,comm,req)'],
                  ['1s','Mrecv',            '(buf,count,type,msg,stat)'],
                  ['1s','Imrecv',           '(buf,count,type,msg,req)'],
                  ['2s','Sendrecv',         '(sbuf,scount,stype,dst,stag,rbuf,rcount,rtype,src,rtag,comm,stat)'],
                  ['2s','Sendrecv_replace', '(buf,scount,stype,dst,stag,rcount,rtype,src,rtag,comm,stat)'],
                  ['1s','Buffer_attach',    '(buf,count)'],
                  ['1s','Buffer_detach',    '(buf,count)'],
                  # persistent point-to-point
                  ['1s','Send_init',        '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Ssend_init',       '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Rsend_init',       '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Bsend_init',       '(buf,count,type,dst,tag,comm,req)'],
                  ['1s','Recv_init',        '(buf,count,type,src,tag,comm,req)'],
                  # RMA
                  ['2s','Put',              '(buf,scount,stype,dst,disp,rcount,rtype,win)'],
                  ['2s','Rput',             '(buf,scount,stype,dst,disp,rcount,rtype,win,req)'],
                  ['2s','Get',              '(buf,scount,stype,dst,disp,rcount,rtype,win)'],
                  ['2s','Rget',             '(buf,scount,stype,dst,disp,rcount,rtype,win,req)'],
                  ['2s','Accumulate',       '(buf,scount,stype,dst,disp,rcount,rtype,op,win)'],
                  ['2s','Raccumulate',      '(buf,scount,stype,dst,disp,rcount,rtype,op,win,req)'],
                  ['3s','Get_accumulate',   '(obuf,ocount,otype,rbuf,rcount,rtype,dst,disp,tcount,ttype,op,win)'],
                  ['3s','Rget_accumulate',  '(obuf,ocount,otype,rbuf,rcount,rtype,dst,disp,tcount,ttype,op,win,req)'],
                  # collectives
                  ['1s','Bcast',                 '(buf,count,type,root,comm)'],
                  ['1s','Ibcast',                '(buf,count,type,root,comm,req)'],
                  ['2s','Gather',                '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm)'],
                  ['2s','Igather',               '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm,req)'],
                  ['2s','Allgather',             '(sbuf,scount,stype,rbuf,rcount,rtype,comm)'],
                  ['2s','Iallgather',            '(sbuf,scount,stype,rbuf,rcount,rtype,comm,req)'],
                  ['2s','Scatter',               '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm)'],
                  ['2s','Iscatter',              '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm,req)'],
                  ['2s','Alltoall',              '(sbuf,scount,stype,rbuf,rcount,rtype,comm)'],
                  ['2s','Ialltoall',             '(sbuf,scount,stype,rbuf,rcount,rtype,comm,req)'],
                  # reductions
                  ['1s','Reduce',                '(sbuf,rbuf,count,type,op,root,comm)'],
                  ['1s','Ireduce',               '(sbuf,rbuf,count,type,op,root,comm,req)'],
                  ['1s','Allreduce',             '(sbuf,rbuf,count,type,op,comm)'],
                  ['1s','Iallreduce',            '(sbuf,rbuf,count,type,op,comm,req)'],
                  ['1s','Reduce_scatter_block',  '(sbuf,rbuf,count,type,op,comm)'],
                  ['1s','Ireduce_scatter_block', '(sbuf,rbuf,count,type,op,comm,req)'],
                  ['1v','Reduce_scatter',        '(sbuf,rbuf,counts,type,op,comm)'],
                  ['1v','Ireduce_scatter',       '(sbuf,rbuf,counts,type,op,comm,req)'],
                  # vector collectives
                  ['1s2v','Scatterv',            '(sbuf,counts,displs,stype,rbuf,count,rtype,root,comm)'],
                  ['1s2v','Iscatterv',           '(sbuf,counts,displs,stype,rbuf,count,rtype,root,comm,req)'],
                  ['1s2v','Gatherv',             '(sbuf,count,stype,rbuf,counts,displs,rtype,root,comm)'],
                  ['1s2v','Igatherv',            '(sbuf,count,stype,rbuf,counts,displs,rtype,root,comm,req)'],
                  ['1s2v','Allgatherv',          '(sbuf,count,stype,rbuf,counts,displs,rtype,comm)'],
                  ['1s2v','Iallgatherv',         '(sbuf,count,stype,rbuf,counts,displs,rtype,comm,req)'],
                  ['4v','Alltoallv',             '(sbuf,scounts,sdispls,stype,rbuf,rcounts,rdispls,rtype,comm)'],
                  ['4v','Ialltoallv',            '(sbuf,scounts,sdispls,stype,rbuf,rcounts,rdispls,rtype,comm,req)'],
                  ['4v','Alltoallw',             '(sbuf,scounts,sdispls,stypes,rbuf,rcounts,rdispls,rtypes,comm)'],
                  ['4v','Ialltoallw',            '(sbuf,scounts,sdispls,stypes,rbuf,rcounts,rdispls,rtypes,comm,req)'],
                  # neighborhood collectives
                  ['2s','Neighbor_allgather',     '(sbuf,scount,stype,rbuf,rcount,rtype,comm)'],
                  ['2s','Neighbor_iallgather',    '(sbuf,scount,stype,rbuf,rcount,rtype,comm,req)'],
                  ['2s','Neighbor_alltoall',      '(sbuf,scount,stype,rbuf,rcount,rtype,comm)'],
                  ['2s','Neighbor_ialltoall',     '(sbuf,scount,stype,rbuf,rcount,rtype,comm,req)'],
                  ['1s2v','Neighbor_allgatherv',  '(sbuf,count,stype,rbuf,counts,displs,rtype,comm)'],
                  ['1s2v','Neighbor_iallgatherv', '(sbuf,count,stype,rbuf,counts,displs,rtype,comm,req)'],
                  ['4v','Neighbor_alltoallv',     '(sbuf,scounts,sdispls,stype,rbuf,rcounts,rdispls,rtype,comm)'],
                  ['4v','Neighbor_ialltoallv',    '(sbuf,scounts,sdispls,stype,rbuf,rcounts,rdispls,rtype,comm,req)'],
                  # neighbor_alltoallw already has Aint displs so no need to select there
                  ['2v','Neighbor_alltoallw',     '(sbuf,scounts,sdispls,stypes,rbuf,rcounts,rdispls,rtypes,comm)'],
                  ['2v','Neighbor_ialltoallw',    '(sbuf,scounts,sdispls,stypes,rbuf,rcounts,rdispls,rtypes,comm,req)'],
                  # datatypes
                  ['1s','Type_contiguous',               '(count,oldtype,newtype)'],
                  ['2sca','Type_vector',                 '(nblocks,count,disp,oldtype,newtype)'],
                  ['1s','Type_hvector',                  '(nblocks,count,disp,oldtype,newtype)'],
                  ['2vca','Type_indexed',                '(nblocks,counts,disps,oldtype,newtype)'],
                  ['1v','Type_hindexed',                 '(nblocks,counts,disps,oldtype,newtype)'],
                  ['1v','Type_struct',                   '(nblocks,counts,disps,oldtypes,newtype)'],
                  ['1v','Type_create_hindexed',          '(nblocks,counts,disps,oldtype,newtype)'],
                  ['1s','Type_create_hvector',           '(nblocks,count,disp,oldtype,newtype)'],
                  ['1sc1va','Type_create_indexed_block', '(nblocks,count,disps,oldtype,newtype)'],
                  ['1s','Type_create_hindexed_block',    '(nblocks,count,disps,oldtype,newtype)'],
                  ['1v','Type_create_struct',            '(nblocks,counts,disps,oldtypes,newtype)'],
                  ['3v','Type_create_subarray',          '(nblocks,counts1,counts2,counts3,order,oldtype,newtype)']
                  # TODO:
                  # MPI_Type_create_darray
                  # file I/O
                 ]

f = open('mpi_c11.h','w')
make_pre(f)
for (n,name,args) in mpi_interfaces:
    if   n=='1s':     make_mpi_c11_decl1s(f,name,args)
    elif n=='2s':     make_mpi_c11_decl2s(f,name,args)
    elif n=='3s':     make_mpi_c11_decl3s(f,name,args)
    elif n=='1v':     make_mpi_c11_decl1v(f,name,args)
    elif n=='2v':     make_mpi_c11_decl1v(f,name,args)
    elif n=='4v':     make_mpi_c11_decl4v(f,name,args)
    elif n=='1s1v':   make_mpi_c11_decl1s1v(f,name,args)
    elif n=='1s2v':   make_mpi_c11_decl1s2v(f,name,args)
    # ca = (Count,Aint) - for datatypes
    elif n=='1sc1va': make_mpi_c11_decl1sc1va(f,name,args)
    elif n=='2sca':   make_mpi_c11_decl2sca(f,name,args)
    elif n=='2vca':   make_mpi_c11_decl2vca(f,name,args)
    elif n=='3v':     make_mpi_c11_decl3v(f,name,args)
    else: print 'oops for '+n
make_get_count_etc(f)
make_user_function(f)
make_post(f)
f.close()





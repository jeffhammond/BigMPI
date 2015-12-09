#!/usr/bin/python

import fileinput
import string
import sys
import os

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
           +'typedef void MPI_User_function_x( void *invec, void *inoutvec, int *MPI_Count, MPI_Datatype *datatype);\n'
           #+'int MPI_Op_create(MPI_User_function *function, int commute, MPI_Op *op);\n'
           +'int MPI_Op_create_x(MPI_User_function_x *function, int commute, MPI_Op *op);\n'
           +'#define MPI_Op_create(function, commute, op)'
           +'        _Generic((function)                          \\\n'
           +'                 MPI_User_function*:   MPI_Op_create, \\\n'
           +'                 MPI_User_function_x*: MPI_Op_create_x)(function, commute, op)\n\n')

def make_mpi_c11_decl1s(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count)                          \\\n'
           +'                 short:               MPI_' + name +  ',   \\\n'
           +'                 int:                 MPI_' + name +  ',   \\\n'
           +'                 MPI_Count:           MPI_' + name +  '_x, \\\n'
           +'                 default:             MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl1v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((counts)                          \\\n'
           +'                 int*:                 MPI_' + name +  ',   \\\n'
           +'                 MPI_Count*:           MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl2s(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scount,rcount)                           \\\n'
           +'                 (short,short):               MPI_' + name +  ',   \\\n'
           +'                 (short,int):                 MPI_' + name +  ',   \\\n'
           +'                 (int,short):                 MPI_' + name +  ',   \\\n'
           +'                 (int,int):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count):       MPI_' + name +  '_x, \\\n'
           +'                 (default,default):           MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl1s1v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count,vcount)                           \\\n'
           +'                 (int,int*):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count*):       MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl2v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scounts,rcounts)                           \\\n'
           +'                 (int*,int*):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count*,MPI_Count*):       MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl1s2v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count,counts,displs)                           \\\n'
           +'                 (int,int*,int*):                   MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count*,MPI_Aint*):  MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl3s(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((ocount,rcount,tcount)                      \\\n'
           +'                 (short,short,short):               MPI_' + name +  ',   \\\n'
           +'                 (short,short,int):                 MPI_' + name +  ',   \\\n'
           +'                 (short,int,short):                 MPI_' + name +  ',   \\\n'
           +'                 (int,short,short):                 MPI_' + name +  ',   \\\n'
           +'                 (short,int,int):                   MPI_' + name +  ',   \\\n'
           +'                 (int,short,int):                   MPI_' + name +  ',   \\\n'
           +'                 (int,int,short):                   MPI_' + name +  ',   \\\n'
           +'                 (int,int,int):                     MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count,MPI_Count,MPI_Count):   MPI_' + name +  '_x, \\\n'
           +'                 (default,default,default):         MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl4v(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scounts,sdispls,rcounts,rdispls)                           \\\n'
           +'                 (int*,int*,int*,int*):                        MPI_' + name +  ',   \\\n'
           +'                 (MPI_Count*,MPI_Aint*,MPI_Count*,MPI_Aint*):  MPI_' + name +  '_x)' + args + '\n\n')

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
                  ['1s','Buffer_attach',    '(buf,size)'],
                  # RMA
                  ['2s','Put',              '(buf,scount,stype,dst,disp,rcount,rtype,win)'],
                  ['2s','Get',              '(buf,scount,stype,dst,disp,rcount,rtype,win)'],
                  ['2s','Accumulate',       '(buf,scount,stype,dst,disp,rcount,rtype,op,win)'],
                  ['3s','Get_accumulate',   '(obuf,ocount,otype,rbuf,rcount,rtype,dst,disp,tcount,ttype,op,win)'],
                  ['2s','Rput',             '(buf,scount,stype,dst,disp,rcount,rtype,win,req)'],
                  ['2s','Rget',             '(buf,scount,stype,dst,disp,rcount,rtype,win,req)'],
                  ['2s','Raccumulate',      '(buf,scount,stype,dst,disp,rcount,rtype,op,win,req)'],
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
                  # TODO:
                  #  file I/I
                 ]

f = open('test.h','w')
make_pre(f)
for (n,name,args) in mpi_interfaces:
    if   n=='1s':   make_mpi_c11_decl1s(f,name,args)
    elif n=='2s':   make_mpi_c11_decl2s(f,name,args)
    elif n=='3s':   make_mpi_c11_decl3s(f,name,args)
    elif n=='1v':   make_mpi_c11_decl1v(f,name,args)
    elif n=='2v':   make_mpi_c11_decl1v(f,name,args)
    elif n=='4v':   make_mpi_c11_decl4v(f,name,args)
    elif n=='1s1v': make_mpi_c11_decl1s1v(f,name,args)
    elif n=='1s2v': make_mpi_c11_decl1s2v(f,name,args)
    else: print 'oops for '+n
make_user_function(f)
make_post(f)
f.close()






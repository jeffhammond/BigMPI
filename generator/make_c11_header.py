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

def make_mpi_c11_decl(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((count)                          \\\n'
           +'                 short:               MPI_' + name +  '    \\\n'
           +'                 int:                 MPI_' + name +  '    \\\n'
           +'                 MPI_Count:           MPI_' + name +  '_x, \\\n'
           +'                 default:             MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl2(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((scount,rcount)                           \\\n'
           +'                 (short,short):               MPI_' + name +  '    \\\n'
           +'                 (short,int):                 MPI_' + name +  '    \\\n'
           +'                 (int,short):                 MPI_' + name +  '    \\\n'
           +'                 (int,int):                   MPI_' + name +  '    \\\n'
           +'                 (MPI_Count,MPI_Count):       MPI_' + name +  '_x, \\\n'
           +'                 (default,default):           MPI_' + name +  '_x)' + args + '\n\n')

def make_mpi_c11_decl3(f,name,args):
    f.write('#define MPI_' + name + args + '    \\\n'
           +'        _Generic((ocount,rcount,tcount)                      \\\n'
           +'                 (short,short,short):               MPI_' + name +  '    \\\n'
           +'                 (short,short,int):                 MPI_' + name +  '    \\\n'
           +'                 (short,int,short):                 MPI_' + name +  '    \\\n'
           +'                 (int,short,short):                 MPI_' + name +  '    \\\n'
           +'                 (short,int,int):                   MPI_' + name +  '    \\\n'
           +'                 (int,short,int):                   MPI_' + name +  '    \\\n'
           +'                 (int,int,short):                   MPI_' + name +  '    \\\n'
           +'                 (int,int,int):                     MPI_' + name +  '    \\\n'
           +'                 (MPI_Count,MPI_Count,MPI_Count):   MPI_' + name +  '_x, \\\n'
           +'                 (default,default,default):         MPI_' + name +  '_x)' + args + '\n\n')

mpi_interfaces = [
                  # P2P
                  [1,'Send',             '(buf,count,type,dst,tag,comm)'],
                  [1,'Ssend',            '(buf,count,type,dst,tag,comm)'],
                  [1,'Rsend',            '(buf,count,type,dst,tag,comm)'],
                  [1,'Bsend',            '(buf,count,type,dst,tag,comm)'],
                  [1,'Isend',            '(buf,count,type,dst,tag,comm,req)'],
                  [1,'Issend',           '(buf,count,type,dst,tag,comm,req)'],
                  [1,'Irsend',           '(buf,count,type,dst,tag,comm,req)'],
                  [1,'Ibsend',           '(buf,count,type,dst,tag,comm,req)'],
                  [1,'Recv',             '(buf,count,type,src,tag,comm,stat)'],
                  [1,'Irecv',            '(buf,count,type,src,tag,comm,req)'],
                  [1,'Mrecv',            '(buf,count,type,msg,stat)'],
                  [1,'Imrecv',           '(buf,count,type,msg,req)'],
                  [2,'Sendrecv',         '(sbuf,scount,stype,dst,stag,rbuf,rcount,rtype,src,rtag,comm,stat)'],
                  [2,'Sendrecv_replace', '(buf,scount,stype,dst,stag,rcount,rtype,src,rtag,comm,stat)'],
                  [1,'Buffer_attach',    '(buf,size)'],
                  # RMA
                  [2,'Put',              '(buf,scount,stype,dst,disp,rcount,rtype,win)'],
                  [2,'Get',              '(buf,scount,stype,dst,disp,rcount,rtype,win)'],
                  [2,'Accumulate',       '(buf,scount,stype,dst,disp,rcount,rtype,op,win)'],
                  [3,'Get_accumulate',   '(obuf,ocount,otype,rbuf,rcount,rtype,dst,disp,tcount,ttype,op,win)'],
                  [2,'Rput',              '(buf,scount,stype,dst,disp,rcount,rtype,win,req)'],
                  [2,'Rget',              '(buf,scount,stype,dst,disp,rcount,rtype,win,req)'],
                  [2,'Raccumulate',       '(buf,scount,stype,dst,disp,rcount,rtype,op,win,req)'],
                  [3,'Rget_accumulate',   '(obuf,ocount,otype,rbuf,rcount,rtype,dst,disp,tcount,ttype,op,win,req)'],
                  # COLL
                  [1,'Bcast',                 '(buf,count,type,root,comm)'],
                  [1,'Ibcast',                '(buf,count,type,root,comm,req)'],
                  [2,'Gather',                '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm)'],
                  [2,'Igather',               '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm,req)'],
                  [2,'Allgather',             '(sbuf,scount,stype,rbuf,rcount,rtype,comm)'],
                  [2,'Iallgather',            '(sbuf,scount,stype,rbuf,rcount,rtype,comm,req)'],
                  [2,'Scatter',               '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm)'],
                  [2,'Iscatter',              '(sbuf,scount,stype,rbuf,rcount,rtype,root,comm,req)'],
                  [2,'Alltoall',              '(sbuf,scount,stype,rbuf,rcount,rtype,comm)'],
                  [2,'Ialltoall',             '(sbuf,scount,stype,rbuf,rcount,rtype,comm,req)'],
                  [1,'Reduce',                '(sbuf,rbuf,count,type,op,root,comm)'],
                  [1,'Ireduce',               '(sbuf,rbuf,count,type,op,root,comm,req)'],
                  [1,'Allreduce',             '(sbuf,rbuf,count,type,op,comm)'],
                  [1,'Iallreduce',            '(sbuf,rbuf,count,type,op,comm,req)'],
                  [1,'Reduce_scatter_block',  '(sbuf,rbuf,count,type,op,comm)'],
                  [1,'Ireduce_scatter_block', '(sbuf,rbuf,count,type,op,comm,req)'],

                 ]

f = open('test.h','w')
make_pre(f)
for (n,name,args) in mpi_interfaces:
    if   n==1: make_mpi_c11_decl(f,name,args)
    elif n==2: make_mpi_c11_decl2(f,name,args)
    elif n==3: make_mpi_c11_decl3(f,name,args)
make_post(f)
f.close()






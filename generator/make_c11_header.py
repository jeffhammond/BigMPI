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
                  [1,'Send',             '(buf,count,type,dest,tag,comm)'],
                  [1,'Ssend',            '(buf,count,type,dest,tag,comm)'],
                  [1,'Rsend',            '(buf,count,type,dest,tag,comm)'],
                  [1,'Bsend',            '(buf,count,type,dest,tag,comm)'],
                  [1,'Isend',            '(buf,count,type,dest,tag,comm,req)'],
                  [1,'Issend',           '(buf,count,type,dest,tag,comm,req)'],
                  [1,'Irsend',           '(buf,count,type,dest,tag,comm,req)'],
                  [1,'Ibsend',           '(buf,count,type,dest,tag,comm,req)'],
                  [1,'Recv',             '(buf,count,type,src,tag,comm,stat)'],
                  [1,'Irecv',            '(buf,count,type,src,tag,comm,req)'],
                  [1,'Mrecv',            '(buf,count,type,msg,stat)'],
                  [1,'Imrecv',           '(buf,count,type,msg,req)'],
                  [2,'Sendrecv',         '(sbuf,scount,stype,dest,stag,rbuf,rcount,rtype,src,rtag,comm,stat)'],
                  [2,'Sendrecv_replace', '(buf,scount,stype,dest,stag,rcount,rtype,src,rtag,comm,stat)'],
                  [1,'Buffer_attach',    '(buf,size)'],
                 ]

f = open('test.h','w')
make_pre(f)
for (n,name,args) in mpi_interfaces:
    if   n==1: make_mpi_c11_decl(f,name,args)
    elif n==2: make_mpi_c11_decl2(f,name,args)
    elif n==3: make_mpi_c11_decl3(f,name,args)
make_post(f)
f.close()






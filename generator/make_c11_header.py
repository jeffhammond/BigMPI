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
    f.write('#define MPI_' + name + args + '    \\ \n'
           +'        _Generic((count)                          \\ \n'
           +'                 short:               MPI_' + name +  '    \\ \n'
           +'                 int:                 MPI_' + name +  '    \\ \n'
           +'                 long:                MPI_' + name +  '_x, \\ \n'
           +'                 long long:           MPI_' + name +  '_x, \\ \n'
           +'                 unsigned:            MPI_' + name +  '_x, \\ \n'
           +'                 unsigned long:       MPI_' + name +  '_x, \\ \n'
           +'                 unsigned long long:  MPI_' + name +  '_x, \\ \n'
           +'                 MPI_Aint:            MPI_' + name +  '_x, \\ \n'
           +'                 MPI_Count:           MPI_' + name +  '_x)' + args + ' \n\n')

mpi_interfaces = [
                  ['Send',  '(buf,count,type,dest,tag,comm)'],
                  ['Ssend', '(buf,count,type,dest,tag,comm)'],
                  ['Rsend', '(buf,count,type,dest,tag,comm)'],
                  ['Bsend', '(buf,count,type,dest,tag,comm)'],
                  ['Isend', '(buf,count,type,dest,tag,comm,req)'],
                  ['Issend','(buf,count,type,dest,tag,comm,req)'],
                  ['Irsend','(buf,count,type,dest,tag,comm,req)'],
                  ['Ibsend','(buf,count,type,dest,tag,comm,req)'],
                  ['Recv',  '(buf,count,type,src,tag,comm,stat)'],
                  ['Irecv', '(buf,count,type,src,tag,comm,req)'],
                  ['Mrecv', '(buf,count,type,msg,stat)'],
                  ['Imrecv','(buf,count,type,msg,req)'],
                  ['Buffer_attach','(buf,size)']]

f = open('test.h','w')
make_pre(f)
for (name,args) in mpi_interfaces:
    make_mpi_c11_decl(f,name,args)
make_post(f)
f.close()






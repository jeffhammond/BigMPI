      subroutine sum_real16(iv,iov,n)
        implicit none
        integer, intent(in) ::  n
        real*16, intent(in) :: iv(:)
        real*16, intent(inout) :: iov(:)
        integer :: i
        do i = 1,n
          iov(i) = iov(i) + iv(i)
        enddo
      end subroutine sum_real16
      subroutine reduce_sum_real16(iv, iov, n, dt)
        use, intrinsic ::  iso_c_binding, only : c_ptr
        use mpi_f08
        implicit none
        type(c_ptr), value ::  iv, iov
        integer ::  n
        type(MPI_Datatype) ::  dt
        if ( dt .eq. MPI_REAL16 ) then
            call sum_real16(iv,iov,n)
        endif
      end subroutine reduce_sum_real16
      program test_reduce_sum_real16
        use, intrinsic ::  iso_c_binding
        use mpi_f08
        implicit none
        integer, parameter ::  n = 10
        real*16 :: output(n)
        real*16 :: input(n)
        real*16 :: error
        integer :: me, np
        procedure(MPI_User_function) :: reduce_sum_real16
        type(MPI_Op) :: mysum
        integer :: i
        call MPI_Init()
        call MPI_Comm_rank(MPI_COMM_WORLD,me)
        call MPI_Comm_size(MPI_COMM_WORLD,np)
        output = 0.0
        input  = 1.0*me
        call MPI_Op_create(reduce_sum_real16,.true.,mysum)
        call MPI_Allreduce(input,output,n,MPI_REAL16,mysum,MPI_COMM_WORLD)
        error = 0.0
        do i = 1,n
          error = error + (output(i)-1.0*np)
        enddo
        if (error.gt.0.0) then
            print*,'SAD PANDA = ',error
            call MPI_Abort(MPI_COMM_SELF,1)
        endif
        call MPI_Op_free(mysum)
        call MPI_Finalize()
      end program test_reduce_sum_real16

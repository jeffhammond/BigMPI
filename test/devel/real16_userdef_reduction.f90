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

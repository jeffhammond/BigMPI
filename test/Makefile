CC =  /opt/ompi/largecount/gcc/bin/mpicc

all: test_allgather_x.x test_allreduce_x.x test_alltoall_x.x test_bcast_x.x test_gather_x.x test_irsend_irecv_x.x test_isend_irecv_x.x test_issend_irecv_x.x test_reduce_x.x test_rma2_x.x test_rma_x.x test_rsend_recv_x.x test_scatter_x.x test_send_recv_x.x test_sendrecv_x.x test_ssend_recv_x.x

%.x: %.c
	$(CC) $^ -o $@

clean:
	-rm -f *.o *.x

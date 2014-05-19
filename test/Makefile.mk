#
# Copyright (C) 2014. See LICENSE in top-level directory.
#

check_PROGRAMS += tests/test_bcast_x \
		  tests/test_allreduce_x \
		  tests/test_alltoall_x \
		  tests/test_assert_x \
		  tests/test_bcast_x \
		  tests/test_contig_x \
		  tests/test_gather_x \
		  tests/test_isend_irecv_x \
		  tests/test_reduce_x \
		  tests/test_rma_x \
		  tests/test_rma2_x \
		  tests/test_scatter_x \
		  tests/test_send_recv_x \
		  tests/test_sendrecv_x

TESTS          += tests/test_bcast_x         \
		  tests/test_allreduce_x \
		  tests/test_alltoall_x \
		  tests/test_assert_x \
		  tests/test_bcast_x \
		  tests/test_contig_x \
		  tests/test_gather_x \
		  tests/test_isend_irecv_x \
		  tests/test_reduce_x \
		  tests/test_rma_x \
		  tests/test_rma2_x \
		  tests/test_scatter_x \
		  tests/test_send_recv_x \
		  tests/test_sendrecv_x

XFAIL_TESTS    += tests/test_assert_x

tests_test_bcast_x_LDADD = libbigmpi.la
tests_test_allreduce_x_LDADD = libbigmpi.la
tests_test_alltoall_x_LDADD = libbigmpi.la
tests_test_assert_x_LDADD = libbigmpi.la
tests_test_contig_x_LDADD = libbigmpi.la
tests_test_gather_x_LDADD = libbigmpi.la
tests_test_isend_irecv_x_LDADD = libbigmpi.la
tests_test_reduce_x_LDADD = libbigmpi.la
tests_test_rma_x_LDADD = libbigmpi.la
tests_test_rma2_x_LDADD = libbigmpi.la
tests_test_scatter_x_LDADD = libbigmpi.la
tests_test_send_recv_x_LDADD = libbigmpi.la
tests_test_sendrecv_x_LDADD = libbigmpi.la

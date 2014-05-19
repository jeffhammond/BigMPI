#
# Copyright (C) 2014. See LICENSE in top-level directory.
#

check_PROGRAMS += tests/test_bcast_x         \
                  # end

TESTS          += tests/test_bcast_x         \
                  # end

XFAIL_TESTS    += tests/test_assert_x           \
                  # end

tests_test_bcast_x_LDADD = libbigmpi.la


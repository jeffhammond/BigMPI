#define SUM 0x1

#define MAKE_FOO(OP) void FOO_##OP##_fn(){ Bar(MPI_##OP);\
                                           return; }

MAKE_FOO(SUM);

int main(void)
{
    return;
}

#
# Taken from
# http://hpcf.umbc.edu/general-productivity/checking-which-cpus-are-used-by-your-program/
#

OBJS := hello_world.o

EXECUTABLE := hello_world_mpi

CC := mpicc

# choose flags:
# flags for Intel compiler icc on maya:
# CFLAGS := -std=c99 -Wall -O3 -openmp #-mt_mpi
# flags for Portland Group compiler pgcc on maya:
# CFLAGS := -O3 -c99 -Minform=warn -fastsse
# flags for GNU compiler gcc anywhere:
CFLAGS := # -O3 -std=c99 -Wall -Wno-unused-variable

DEFS :=
INCLUDES :=
LDFLAGS :=
LDFLAGS :=

%.o: %.c %.h
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) $(OBJS) -o $@ $(LDFLAGS)

clean:
	-rm -f *.o $(EXECUTABLE) *~
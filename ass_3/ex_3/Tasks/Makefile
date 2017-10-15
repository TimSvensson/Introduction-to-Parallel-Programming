CC = gcc
OBJS = timer.o
FLAGS = -fopenmp
DEBUGFLAG = -DDEBUG
SEQFLAGS = -O3 -lm -Wall

EXEC = Merge_sort Merge_sort_debug Merge_sortNP Merge_sortNP_debug Merge_sortP Merge_sortP_debug Merge_sortS Merge_sortS_debug

all: $(EXEC)

Merge_sort: Merge_sort.c $(OBJS)
	$(CC) $(SEQFLAGS)  -o $@ $< $(OBJS)

Merge_sort_debug:Merge_sort.c $(OBJS)
	$(CC) $(SEQFLAGS) $(DEBUGFLAG)  -o $@ $< $(OBJS)

Merge_sortNP: Merge_sortNP.c $(OBJS)
	$(CC) $(SEQFLAGS)  -o $@ $< $(OBJS)

Merge_sortNP_debug:Merge_sortNP.c $(OBJS)
	$(CC) $(SEQFLAGS) $(DEBUGFLAG)  -o $@ $< $(OBJS)

Merge_sortP: Merge_sortP.c $(OBJS)
	$(CC) $(SEQFLAGS)  -o $@ $< $(OBJS)

Merge_sortP_debug:Merge_sortP.c $(OBJS)
	$(CC) $(SEQFLAGS) $(DEBUGFLAG)  -o $@ $< $(OBJS)

Merge_sortS: Merge_sortS.c $(OBJS)
	$(CC) $(SEQFLAGS)  -o $@ $< $(OBJS)

Merge_sortS_debug:Merge_sortS.c $(OBJS)
	$(CC) $(SEQFLAGS) $(DEBUGFLAG)  -o $@ $< $(OBJS)

timer.o: timer.c
	$(CC) -O3 -o $@ -c $<

clean:
	rm -f $(OBJS) $(EXEC)

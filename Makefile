CC?= gcc

all: data-out.txt

fasta: fasta.c
	${CC} -pipe -Wall -O3 -fomit-frame-pointer -march=native -std=c99 -mfpmath=sse -msse3 fasta.c -o fasta

data-in.txt: fasta
	time ./fasta 25000000 > data-in.txt

revcomp: revcomp.c
	${CC} -pipe -Wall -O3 -fomit-frame-pointer -march=native -std=c99 -mfpmath=sse -msse3 revcomp.c -o revcomp

revcomp-test: revcomp revcomp-input.txt revcomp-output.txt
	time ./revcomp < revcomp-input.txt > revcomp.test.out
	diff revcomp-output.txt revcomp.test.out

data-out.txt: revcomp data-in.txt
	time ./revcomp < data-in.txt > data-out.txt

.PHONY: clean

clean:
	rm -f revcomp fasta data-in.txt data-out.txt revcomp.test.out

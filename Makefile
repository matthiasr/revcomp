CC?= gcc

all: revcomp-test revcomp-paul-test revcomp-time revcomp-paul-time

fasta: fasta.c
	${CC} -pipe -Wall -O3 -fomit-frame-pointer -std=c99 -mfpmath=sse -msse3 fasta.c -o fasta

data-in.txt: fasta
	time ./fasta 25000000 > data-in.txt

revcomp: revcomp.c
	${CC} -pipe -Wall -O3 -fomit-frame-pointer -std=c99 -mfpmath=sse -msse3 revcomp.c -o revcomp

revcomp-paul: revcomp-paul.c
	${CC} -pipe -Wall -O3 -fomit-frame-pointer -std=c99 -mfpmath=sse -msse3 revcomp-paul.c -o revcomp-paul

revcomp-test: revcomp revcomp-input.txt revcomp-output.txt
	time ./revcomp < revcomp-input.txt > revcomp.test.out
	diff revcomp-output.txt revcomp.test.out

revcomp-paul-test: revcomp-paul revcomp-input.txt revcomp-output.txt
	time ./revcomp-paul < revcomp-input.txt > revcomp-paul.test.out
	diff revcomp-output.txt revcomp-paul.test.out

revcomp-time: revcomp data-in.txt
	time ./revcomp < data-in.txt > /dev/null

revcomp-paul-time: revcomp-paul data-in.txt
	time ./revcomp-paul < data-in.txt > /dev/null

.PHONY: clean

clean:
	rm -f revcomp fasta data-in.txt revcomp.test.out revcomp-paul.test.out

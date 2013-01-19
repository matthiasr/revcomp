#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#define BLOCKLEN 1024
#define TITLELEN 132
#define LINELEN 60

typedef struct block_struct {
	int		len;
	char		text      [BLOCKLEN];
	struct block_struct *next;
}		Block;

/* Here to get them profiled */
int 
my_getc(FILE * stream)
{
	return (getc(stream));
}

int 
my_putc(int c, FILE * stream)
{
	return (putc(c, stream));
}


/* Initialised by calls to setComplement. Holds the complement codes. */
static char	complement[256];

void 
setComplement(char c1, char c2)
{
	complement[c1] = c2;
	complement[c2] = c1;
	complement[c1 | 0x20] = c2;
	complement[c2 | 0x20] = c1;
}



/* Reverse-complement block in-place */
void 
reverseComplementBlock(Block * block)
{
	char           *ptr1;
	char           *ptr2;
	char		c;

	ptr1 = &(block->text[0]);
	ptr2 = &(block->text[block->len - 1]);
	while (ptr1 <= ptr2) {
		c = complement[*ptr1];
		*ptr1 = complement[*ptr2];
		*ptr2 = c;
		ptr1++;
		ptr2--;
	}
}

/*
 * Read blocks up to either the next ">" or EOF. Create a chain of blocks in
 * reverse order of reading.
 */
Block          *
readBlocks()
{
	char		c;
	int		i;
	Block          *result;
	Block          *old;

	old = NULL;
	result = NULL;
	c = ' ';
	while (c != '>' && !feof(stdin)) {
		result = malloc(sizeof(Block));
		i = 0;
		while (i < BLOCKLEN && !feof(stdin)) {
			c = (char)my_getc(stdin);
			if (c == '>') {
				ungetc(c, stdin);
				break;
			}
			if (c >= ' ') {
				//Drop line breaks.
					result->text[i++] = c;
			}
		}
		result->len = i;
		result->next = old;
		old = result;
	}
	return result;
}


int 
process_sequence()
{

	char		title     [TITLELEN];
	Block          *ptr;
	Block          *old_ptr;
	int		i;
	int		cpos;

	if (!feof(stdin)) {
		fgets(title, TITLELEN, stdin);
	} else {
		return 0;
	}

	if (index(title, '\n') == NULL) {
		while (my_getc(stdin) != '\n') {
		}
	}
	ptr = readBlocks();

	printf("%s", title);

	cpos = 0;
	while (ptr != NULL) {
		reverseComplementBlock(ptr);
		for (i = 0; i < ptr->len; i++) {
			if (cpos++ == 60) {
				my_putc('\n', stdout);
				cpos = 1;
			}
			my_putc(ptr->text[i], stdout);
		}
		old_ptr = ptr;
		ptr = ptr->next;
		free(old_ptr);
		/*
		 * Beware; the obvious "for" loop would free the block and
		 * then dereference it to move to the next one.
		 */
	}

	my_putc('\n', stdout);
	return 1;
}

int 
main(int argc, char** argv)
{

	int		i;
	char		title     [TITLELEN];

	for (i = 0; i < 256; i++) {
		complement[i] = 0;
	}
	setComplement('A', 'T');
	setComplement('C', 'G');
	setComplement('M', 'K');
	setComplement('R', 'Y');
	setComplement('W', 'W');
	setComplement('S', 'S');
	setComplement('V', 'B');
	setComplement('H', 'D');
	setComplement('N', 'N');

	while (process_sequence()) {
	}
        return 0;
}

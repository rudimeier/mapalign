#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFSZ 1

static double *L;
static double *pL;
static size_t lenL;

// TODO transpose if matrix is not symmetric!
int parse_buf(char* buf, int size)
{
	char *p = buf;
	char *last = buf;
	char *end = buf + size;
	while (p < end) {
		if (*p == '\t' || *p == '\n' ) {
			double f;
			*p = '\0';
			f = atof(last);
			*pL = f;
			pL++;
			//printf("%s: %.2f\n", last, f);
			last = p + 1;
		}
		p++;
	}
	if (last < end) {
		return (end - last);
	} else {
		return 0;
	}
}

int read_csv(char * file, int n )
{
	int fd;
	char buf[2*BUFSZ];
	char *buf_start = buf;

	lenL = n;
	L = (double*) malloc(sizeof(double) * n * n);
	pL = L;

	fd = open( file, O_RDONLY );
	if (fd < 0) {
		perror("error, open");
		exit(1);
	}

	int over = 0;
	while(1) {
		int s = read(fd, buf_start, BUFSZ);
		if (s < 0) {
			perror("error, read");
			exit(1);
		} else if (s == 0) {;
			//fprintf(stderr, "file end\n");
			break;
		} else {
			//fprintf(stderr, "read %d\n", s);
			over = parse_buf(buf, s + over);
			//fprintf(stderr, "over %d, %c\n", over, *(buf_start +s - over));
			if (over > 0) {
				memcpy(buf, buf_start +s - over, over);
				buf_start = buf + over;
			}
		}
	}
}

static void print_matrix( int m, int n, double* a )
{
	int i, j;
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ ) {
			printf( "%.2f\t", a[(size_t)j * m + i] );
		}
		printf( "\n" );
	}
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		fprintf(stderr, "error: usage\n");
		exit(1);
	}

	read_csv( argv[1], atoi(argv[2]) );
	print_matrix(lenL, lenL, L);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFSZ 1024 * 1024


// TODO transpose if matrix is not symmetric!
int parse_buf(double** pL, char* buf, int size)
{
	char *p = buf;
	char *last = buf;
	char *end = buf + size;

	while (p < end) {
		if (*p == '\t' || *p == '\n' ) {
			double f;
			*p = '\0';
			f = atof(last);
			**pL = f;
			(*pL)++;
			//printf("row %zu %s: %.2f\n", pL - L, last, f);
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

int read_csv(char * file, double *L, int n)
{
	int fd;
	char buf_all[2 * BUFSZ];
	char *buf = buf_all + BUFSZ;
	double* pL = L;

	fd = open( file, O_RDONLY );
	if (fd < 0) {
		perror("error, open");
		exit(1);
	}

	int over = 0;
	while(1) {
		int s = read(fd, buf, BUFSZ);
		if (s < 0) {
			perror("error, read");
			exit(1);
			return -1;
		} else if (s == 0) {
			//fprintf(stderr, "file end\n");
			break;
		} else {
			//fprintf(stderr, "read %d\n", s);
			over = parse_buf(&pL, buf - over, s + over);
			if (over > 0) {
				memcpy(buf - over , buf + s - over, over);
			}
		}
	}
	return 0;
}

int read_csv_get_n(char * file)
{
	int fd;
	char buf[BUFSZ];
	char *end_buf = buf + BUFSZ;
	char *p;
	int n = 0;

	fd = open( file, O_RDONLY );
	if (fd < 0) {
		perror("error, open");
		exit(1);
	}

	while(1) {
		int s = read(fd, buf, BUFSZ);
		if (s < 0) {
			perror("error, read");
			exit(1);
			n = -1;
			break;
		} else if (s == 0) {;
			fprintf(stderr, "error: file end ... no newline found %d\n", n);
			exit(1);
			n = -1;
			break;
		} else {
			for (p = buf; p < end_buf; p++ ) {
				if (*p == '\t') {
					n++;
				} else if (*p == '\n') {
					n++;
					goto end;
				}
			}
		}
	}
end:
	close(fd);
	return n;
}

int mtx_column_sum(double *L, int n, double *d)
{
	int i, j;
	for( i = 0; i < n; i++ ) {
		d[i] = 0.0;
		for( j = 0; j < n; j++ ) {
			d[i] += L[(size_t)i * n + j];
		}
	}
}

static void print_matrix( int m, int n, double* a )
{
	int i, j;
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ ) {
			printf( "%.2f", a[(size_t)j * m + i] );
			if ( j != n - 1) {
				printf("\t");
			}
		}
		printf( "\n" );
	}
}

int main(int argc, char** argv)
{
	char* infile;
	int n;
	double *L;

	if (argc < 2) {
		fprintf(stderr, "error: usage\n");
		exit(1);
	}
	infile = argv[1];

	n = read_csv_get_n( infile );
	fprintf(stderr, "n is %d\n", n);
	L = malloc(sizeof(double) * n * n);

	read_csv( infile, L, n );
	print_matrix(n, n, L);
	printf("\n");

	double *d = malloc(sizeof(double) * n);
	mtx_column_sum(L, n, d);
	print_matrix(1, n, d);
}
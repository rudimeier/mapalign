#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFSZ 10

int parse_buf(char* buf, int size)
{
	char *p = buf;
	char *last = buf;
	char *end = buf + size;
	while (p < end) {
		if (*p == '\t' || *p == '\n' ) {
			*p = '\0';
			printf("%s\n", last);
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


int main(int argc, char** argv)
{
	int fd;
	char buf[2*BUFSZ];
	char *buf_start = buf;

	if (argc < 2) {
		fprintf(stderr, "error: usage\n");
		exit(1);
	}

	fd = open( argv[1], O_RDONLY );
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
			fprintf(stderr, "file end\n");
			break;
		} else {
			fprintf(stderr, "read %d\n", s);
			over = parse_buf(buf, s + over);
			fprintf(stderr, "over %d, %c\n", over, *(buf_start +s - over));
			if (over > 0) {
				memcpy(buf, buf_start +s - over, over);
				buf_start = buf + over;
			}
		}
	}
}
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// redirects ouptut
void redirect(int fd, int flag) {
	int nullfd = open("/dev/null", flag);
	close(fd);
	dup2(nullfd, fd);
	close(nullfd);
}


// converts int into char array suitable for write(). Returns number length.
int convert(int num, char* buff, int buff_size) {
	buff += buff_size - 1;
	*buff = '\0';
	buff--;

	int size = 1;
	while (num > 0) {
		*buff = num % 10 + 48;
		buff--;
		num /= 10;
		size++;
	}
	buff++;
	return size;
}

int main(int argc, char ** argv) {
	pid_t pid = fork();
	if (pid < 0)
		return 1;
	if (pid > 0) {
		char* buff = (char*)mmap(0, 100*sizeof(char), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		int size = convert(pid, buff, 100);
		write (1, buff + 100 - size, size);
	}
	else {
		int arg_i = 1;
		if (argv[1][0] == '-' && argv[1][1] == 'q') {
			redirect(1, O_WRONLY);
			redirect(2, O_WRONLY);
			arg_i++;
		}
		execvp(argv[arg_i], argv + arg_i);
	}
	return 0;
}

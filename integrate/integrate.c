#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>


double f(double x) {
	return x * x;
}

double integrate(double a, double b, double dx, double (*f)(double)) {
	double res = 0;
	double i;
	for(i = a; i < b; i += dx)
		res += ((f(i + dx) + f(i)) / 2) * dx;
	return res;
}

double mp_integrate(double a, double b, double dx, double (*f)(double), int processnums) {
	double delta = (b - a) / processnums;
	int i = 0;
	for (; i < processnums; ++i) {
		int pid = fork();
		if (pid == 0) {
			int fd = open("tmp", O_WRONLY | O_CREAT | O_APPEND, 0666);
			double res = integrate(a + delta * i, a + delta * (i + 1), dx, f);
			write(fd, &res, sizeof(res));
			close(fd);
			exit(0);
		} else {
			if (pid == -1) 
				perror;
		} 
	}
	double res_total = 0, res;
	int fd = open("tmp", O_RDONLY);
	for (i = 0; i < processnums; ++i) {
		while(read(fd, &res, sizeof(res)) != sizeof(res))
			sleep(1);
		res_total += res;
	}
	close(fd);
	unlink("tmp");
	return res_total;	
}

int main(int argc, char* argv[]) {
	double a = atof(argv[1]);
	double b = atof(argv[2]);
	double dx = atof(argv[3]);
	int processnums = atoi(argv[4]);
	double res_total = mp_integrate(a, b, dx, f, processnums);
	printf("%lf\n", res_total);
	return 0;
}

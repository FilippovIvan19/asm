/*long long TimeValue=0;

unsigned long long time_RDTSC()
{
	union ticks
	{
	    unsigned long long tx;

	    struct dblword
	    {
	    	long tl,th;
	    } dw; // little endian

	} t;

  asm("rdtsc\n": "=a"(t.dw.tl),"=d"(t.dw.th));

  return t.tx;

} // for x86 only!

void start_tact()
{
	TimeValue=time_RDTSC();
}

long long get_tact()
{
	return time_RDTSC()-TimeValue;
}





#include <sys/times.h>
#include <time.h>

struct tms tmsBegin, tmsEnd;

void start_time()
{
	times(&tmsBegin);
}

long get_time()
{
	times(&tmsEnd);
	return ((tmsEnd.tms_utime-tmsBegin.tms_utime)+
		(tmsEnd.tms_stime-tmsBegin.tms_stime))*1000/CLOCK_TAI;
}*/


#include <chrono>
#include <stdio.h>

auto begin = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();

void start_time()
{
	begin = std::chrono::high_resolution_clock::now();    
}

long long get_time()
{
	end = std::chrono::high_resolution_clock::now();
	long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	return ms;    
}

void print_duration(const char *str)
{
	printf("%s\n", str);
	long long dt = get_time();
	printf("        in %llds %lldms\n", dt / 1000, dt % 1000);
	start_time();
}
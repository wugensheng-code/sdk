//-----------------------------------------------------------------------------
// COPYRIGHT (C) 2020   CHIPS&MEDIA INC. ALL RIGHTS RESERVED
// 
// This file is distributed under BSD 3 clause and LGPL2.1 (dual license)
// SPDX License Identifier: BSD-3-Clause
// SPDX License Identifier: LGPL-2.1-only
// 
// The entire notice above must be reproduced on all authorized copies.
// 
// Description  : 
//-----------------------------------------------------------------------------
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#include <windows.h>
#endif

#include "jpuconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "jpulog.h"
#if defined(linux) || defined(__linux) || defined(ANDROID)
#include <time.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>   // for read()
#ifdef CNM_SIM_PLATFORM
#else
static struct termios initial_settings, new_settings;
static int peek_character = -1;
#endif
#endif

#define ANSI_COLOR_ERR      "\x1b[31m"       // RED
#define ANSI_COLOR_TRACE    "\x1b[32m"       // GREEN
#define ANSI_COLOR_WARN     "\x1b[33m"       // YELLOW
#define ANSI_COLOR_BLUE     "\x1b[34m"       // BLUE
#define ANSI_COLOR_INFO     ""
// For future
#define ANSI_COLOR_MAGENTA  "\x1b[35m"       // MAGENTA
#define ANSI_COLOR_CYAN     "\x1b[36m"       // CYAN
#define ANSI_COLOR_RESET    "\x1b[0m"        // RESET


static int log_colors[MAX_LOG_LEVEL] = {
	0,
	TERM_COLOR_R|TERM_COLOR_G|TERM_COLOR_B|TERM_COLOR_BRIGHT, 	//INFO
	TERM_COLOR_R|TERM_COLOR_B|TERM_COLOR_BRIGHT,	//WARN
	TERM_COLOR_R|TERM_COLOR_BRIGHT,	// ERR
	TERM_COLOR_R|TERM_COLOR_G|TERM_COLOR_B	//TRACE
};

static unsigned log_decor = LOG_HAS_TIME | LOG_HAS_FILE | LOG_HAS_MICRO_SEC |
			    LOG_HAS_NEWLINE |
			    LOG_HAS_SPACE | LOG_HAS_COLOR;
static int max_log_level = MAX_LOG_LEVEL;

static FILE *fpLog  = NULL;

#ifdef WIN32
static void term_restore_color();
static void term_set_color(int color);
#endif

BOOL InitLog(const char* path)
{
    if (path != NULL) {
        if ((fpLog=fopen(path, "w")) == NULL) {
            printf("Failed to open log file(%s)\n", path);
            return FALSE;
        }
    }

    return TRUE;
}

void DeInitLog()
{
    if (fpLog != NULL) {
        fclose(fpLog);//lint !e482
        fpLog = NULL;
    }
}

void SetLogColor(int level, int color)
{
	log_colors[level] = color;
}

int GetLogColor(int level)
{
	return log_colors[level];
}

void SetLogDecor(int decor)
{
	log_decor = decor;
}

int GetLogDecor()
{
	return log_decor;
}

void LogMsg(int level, const char *format, ...)
{
	va_list ptr;
	char logBuf[MAX_PRINT_LENGTH] = {0};
	char*   prefix = "";
    char*   postfix= "";
	char    logMsg[MAX_PRINT_LENGTH+32] = {0};

	if (level > max_log_level)
        return;
#if defined(SUPPORT_SW_UART) || defined(SUPPORT_SW_UART_V2)
    pthread_mutex_lock(&s_log_mutex);
#endif

    if ((log_decor & LOG_HAS_COLOR)) {
        postfix = ANSI_COLOR_RESET;
        switch (level) {
        case INFO:  prefix = ANSI_COLOR_INFO;  break;
        case ERR:   prefix = ANSI_COLOR_ERR "[ERROR]";   break;
        case TRACE: prefix = ANSI_COLOR_TRACE; break;
        case WARN:  prefix = ANSI_COLOR_WARN"[WARN ]";  break;
        default:    prefix = "";               break;
        }
    }

	va_start( ptr, format );

#if defined(WIN32) || defined(__MINGW32__)
	_vsnprintf( logBuf, MAX_PRINT_LENGTH, format, ptr );
#else
	vsnprintf( logBuf, MAX_PRINT_LENGTH, format, ptr );
#endif
	va_end(ptr);

#ifdef WIN32
	if (log_decor & LOG_HAS_COLOR)
		term_set_color(log_colors[level]);
#endif

#ifdef ANDROID
	if (level == ERR) 
	{
#ifdef LOGE
		LOGE("%s", logBuf);
#endif
#ifdef ALOGE
		ALOGE("%s", logBuf);
#endif
	}
	else
	{
#ifdef LOGI
		LOGI("%s", logBuf);
#endif
#ifdef ALOGI
		ALOGI("%s", logBuf);
#endif
	}
	fputs(logBuf, stderr);
#else
	sprintf(logMsg, "%s%s%s", prefix, logBuf, postfix);
#	ifdef CNM_SIM_PLATFORM
	 io_printf("%s", logBuf);
#	else
	printf(logMsg);
    // fprintf(stderr, logMsg);
    //fputs(logBuf, stderr);
#	endif
#endif

#ifdef SUPPORT_LOG_SAVE
	if ((log_decor & LOG_HAS_FILE) && fpLog)
	{
		fwrite(logBuf, strlen(logBuf), 1,fpLog);
		fflush(fpLog);
	}
#endif

#ifdef WIN32
	if (log_decor & LOG_HAS_COLOR)
		term_restore_color();
#endif
}//lint !e438

#ifdef WIN32
static void term_set_color(int color)
{
    unsigned short attr = 0;
    if (color & TERM_COLOR_R)
        attr |= FOREGROUND_RED;
    if (color & TERM_COLOR_G)
        attr |= FOREGROUND_GREEN;
    if (color & TERM_COLOR_B)
        attr |= FOREGROUND_BLUE;
    if (color & TERM_COLOR_BRIGHT)
        attr |= FOREGROUND_INTENSITY;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
}

static void term_restore_color(void)
{
	term_set_color(log_colors[4]);
}
#endif

static double timer_frequency_;

#if defined(_MSC_VER)
static LARGE_INTEGER initial_;
static LARGE_INTEGER frequency_;
static LARGE_INTEGER counter_;
#endif

#if defined(linux) || defined(__linux) || defined(ANDROID)
struct timeval tv_start;
struct timeval tv_end;

#endif


#if defined(linux) || defined(__linux) || defined(ANDROID)
#else
void timer_init()
{

#if defined(_MSC_VER)
	if (QueryPerformanceFrequency(&frequency_))
	{
		//printf("High:%d, Quad:%d, Low:%d\n", frequency_.HighPart, frequency_.QuadPart, frequency_.LowPart);
		timer_frequency_ = (double)((long double)((long)(frequency_.HighPart) >> 32) + frequency_.LowPart);//lint !e572
	}
	else
		printf("QueryPerformanceFrequency returned FAIL\n");
#endif
}
#endif

void timer_start()
{
#if defined(linux) || defined(__linux) || defined(ANDROID)
#else
	timer_init();
#endif
#if defined(linux) || defined(__linux) || defined(ANDROID)
	gettimeofday(&tv_start, NULL);
#else
#if defined(_MSC_VER)
	if (timer_frequency_ == 0)
		return;

	QueryPerformanceCounter(&initial_);
#endif
#endif
}

void timer_stop()
{
#if defined(linux) || defined(__linux) || defined(ANDROID)
	gettimeofday(&tv_end, NULL);
#else
#if defined(_MSC_VER)
	if (timer_frequency_ == 0)
		return;

	QueryPerformanceCounter(&counter_);
#endif
#endif
}

double timer_elapsed_ms()
{
	double ms;
	ms = timer_elapsed_us()/1000.0;
	return ms;
}

double timer_elapsed_us()
{
	double elapsed = 0;
#if defined(linux) || defined(__linux) || defined(ANDROID)
	double start_us = 0;
	double end_us = 0;
	end_us = tv_end.tv_sec*1000*1000 + tv_end.tv_usec;
	start_us = tv_start.tv_sec*1000*1000 + tv_start.tv_usec;
	elapsed =  end_us - start_us;
#else
#if defined(_MSC_VER)
	if (timer_frequency_ == 0)
		return 0;

	elapsed = (double)((long double)(counter_.QuadPart - initial_.QuadPart) / (long double)frequency_.QuadPart);
#endif
#endif
	return elapsed;

}

int timer_is_valid()
{
	return timer_frequency_ != 0;
}

double timer_frequency()
{
	return timer_frequency_;
}




#if defined(linux) || defined(__linux) || defined(ANDROID)

void init_keyboard()
{
#ifdef CNM_SIM_PLATFORM
#else
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
	peek_character = -1;
#endif
}

void close_keyboard()
{
#ifdef CNM_SIM_PLATFORM
#else
    tcsetattr(0, TCSANOW, &initial_settings);
	fflush(stdout);
#endif
}

int kbhit()
{
#ifdef CNM_SIM_PLATFORM
	return 0;
#else
	unsigned char ch;
	int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = (int)ch;
        return 1;
    }
    return 0;
#endif
}

int getch()
{
#ifdef CNM_SIM_PLATFORM
	return -1;
#else
	int val;
	char ch;
    if(peek_character != -1)
    {
    	val = peek_character;
        peek_character = -1;
        return val;
    }
    read(0,&ch,1);
    return ch;
#endif
}
#endif	//#if defined(linux) || defined(__linux) || defined(ANDROID)


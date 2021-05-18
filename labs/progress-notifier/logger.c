#include <stdio.h>
#include <stdlib.h>  
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

#define RESET		0
#define BRIGHT 		1
#define BLACK 		0
#define RED		    1
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define	WHITE		7

int initLogger(char *logType);
void textcolor(int attr, int fg, int bg);
int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);
int knd = 0;

int initLogger(char *logType) {
    if (strcmp(logType, "syslog") == 0) {
        knd = 1;
    }
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

void textcolor(int attr, int fg, int bg)
{	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...){
    printf("\n");
    textcolor(BRIGHT, BLUE, BLACK);	
    va_list arg;
    va_start (arg, format);
    if(knd == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);	
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }else{
        vprintf (format, arg);
        printf("\n");
    } 
    printf("\n");
    va_end (arg);
	textcolor(RESET, WHITE, BLACK);	
    return 0;
}

int warnf(const char *format, ...){
    printf("\n");
    textcolor(BRIGHT, YELLOW, BLACK);	
	va_list arg;
    va_start (arg, format);
    if(knd == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_WARNING, format, arg);
        closelog();
    }else{
        vprintf (format, arg);
        printf("\n");
    }
    printf("\n");
    va_end (arg);
	textcolor(RESET, WHITE, BLACK);	
    return 0;
}

int errorf(const char *format, ...){
    printf("\n");
    textcolor(BRIGHT, RED, BLACK);
	va_list arg;
    va_start (arg, format);
    if(knd == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_ERR, format, arg);
        closelog();
    }else{
        vprintf (format, arg);
        printf("\n");
    }
    printf("\n");
    va_end (arg);
	textcolor(RESET, WHITE, BLACK);	
    return 0;
}

int panicf(const char *format, ...){
    printf("\n");
    textcolor(BRIGHT, MAGENTA, BLACK);	
	va_list arg;
    va_start (arg, format);
    if(knd == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);	
        syslog(LOG_CRIT,format, arg);
        closelog ();
    }else {
        vprintf (format, arg);
        printf("\n");
    }
    printf("\n");
    va_end (arg);
	textcolor(RESET, WHITE, BLACK);	
    return 0;
}
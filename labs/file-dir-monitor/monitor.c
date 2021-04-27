#define _XOPEN_SOURCE 500
#define BUF_LEN (10*(sizeof(struct inotify_event)+100))

#include <stdio.h>
#include "logger.h"
#include <sys/inotify.h>
#include <ftw.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* subD[500], dec[500], lN;
int inotifyFd, md, fdLimit = 20;
struct inotify_event *event;
int f = FTW_CHDIR | FTW_DEPTH | FTW_MOUNT;

static void displayInotifyEvent(struct inotify_event *i){
    if(i->mask & IN_CREATE)  infof("Create %s\n", i -> name);
    if(i->mask & IN_DELETE)  infof("Delete %s\n", i -> name);
    if(i->mask & IN_MOVED_FROM)  infof("Rename %s\n", i -> name);
    if(i->mask & IN_MOVED_TO) infof("Move to %s\n", i -> name);
}

static int display_info(const char* fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if(tflag == FTW_D){
        md = inotify_add_watch(inotifyFd,fpath,IN_CREATE|IN_DELETE|IN_MOVE);
        if(md == -1)
            errorf("inotify not working properly...");
    }
    return 0;
}

int main(int argc, char *argv[]){
    // Place your magic here
    int f = 0;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char* p;
    int cnt;
    struct inotify_event *event;

    inotifyFd = inotify_init();
    if (inotifyFd == -1)
        errorf("%s\n","inotify_init"); 
        cnt++;

    if (argc > 2 && strchr(argv[2], 'd') != NULL)
        f |= FTW_DEPTH;
    if (argc > 2 && strchr(argv[2], 'p') != NULL)
        f |= FTW_PHYS;
    
    if (nftw((argc < 2) ? "." : argv[1], display_info, 20, f) == -1){
        errorf("%s\n","nftw");
        cnt++;
    }
    if (argc < 2 || strcmp(argv[1], "--help") == 0){
        errorf("%s\n", "Wrong usage, usage should be: %s <PATHNAME>\n", argv[0]);
        cnt++;
        return 0;
    }

    for(;;){
        numRead = read(inotifyFd, buf, BUF_LEN);
        if(numRead == 0)
            panicf("%s\n","read() from inotify fd returned 0");
            cnt++;
        if(numRead == -1)
            errorf("%s\n", "read");
            cnt++;
        for(p = buf; p < buf + numRead;){
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);
            p += sizeof(struct inotify_event) + event -> len;
        }
    }
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define REPORT_FILE argv[4]

struct pacmanData{
    char pName[100],
        insertD[100],
        removalD[100],
        uploadD[100];
    int upgradeN;
};

struct pacmanData pacP[4000];

int operation(char *rowL); //Tells me what operation is being analized
void analizeLog(char *logFile, char *report); //Analize
void wri(FILE* source,int* rFile); //Write Func
char *getN(char *rowL); //Name
char *getD(char *rowL); //Date
int rL(FILE *file, char *rowL, size_t s);

char* getN(char *rowL){
    int cnt = 0, strt = 0, s = 0;
    for(int i = 0; i < 2; i++){ //Moving until date finish
        for(strt; rowL[strt]!=']'; strt++);
        strt += 2;
    }
    strt++;
    for(strt; rowL[strt] != ' '; strt++); //Moving until operation finished
    strt++;
    for(int i = strt+1; rowL[i] != ' '; i++){
        s++;
    }
    char* name = malloc(s); //Assingning var
    for(int i = strt; rowL[i] != ' '; i++, cnt++){
        name[cnt] = rowL[i];
    }
    return name;
}

char* getD(char *rowL){
    int cnt = 0, s = 0;
    for(int i = 0; rowL[s] != ']'; s++); //Moving s to get to the first ]
    s++;
    char* date = calloc(1, s); //Date w/ Size
    for(int i = 0; i < s; i++, cnt++)
        date[cnt] = rowL[i]; //Getting date in rowL
    return date;
}

int operation(char* rowL){
    int strt = 0;
    for(int i = 0; i < 2; i++){ //i needs to be 2 so I can find ] taking the space into consideration
        for(strt; rowL[strt] != '\0'; strt++)
            if(rowL[strt] == ']')
                break;
        strt+=2;
    }
    if (rowL[strt] == 'i'){ //Installation
        return 1;
    }else if (rowL[strt] == 'u' ){ //Upgrade
        return 2;
    }else if (rowL[strt] == 'r' && rowL[strt + 2] == 'm'){ //Removal
        return 3;
    }
    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    // Implement your solution here.
    FILE*  file = fopen(logFile, "r");
    if (file == NULL){
        printf("WARNING: Error opening file. \n");
        return;
    }
    int reportFile = open(report, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (reportFile < 0) {
        perror("WARNING: Error while creating or opening file.");
        return;
    }   
    wri(file,reportFile);
    if (close(reportFile) < 0){
        perror("WARNING: Error while trying to close file.");
        exit(1);
    }
    printf("Report is generated at: [%s]\n", report);
}

int rL(FILE *file, char *rowL, size_t s){
    int n, cnt = 0, cnt2 = 0, bcPrueba = 0;
    while ((n = (char) getc(file)) != '\n' && cnt<s){
        if (n == EOF){
            break;
        }
        rowL[cnt] = (char) n;
        cnt++;
        cnt2++;
        bcPrueba++;
    }
    rowL[cnt] ='\0';
    return cnt;
}

int main(int argc, char **argv) {
    if (argc !=5 || strcmp(argv[1], "-input") != 0 || strcmp(argv[3], "-report") != 0) {
	    printf("WARNING: Usage error.\n");
	    return 0;
    }
    analizeLog(argv[2], REPORT_FILE);
    return 0;
}

void wri(FILE* source, int* rFile){    
    char rowL[255], temp[10];
    int r, installed = 0, remo = 0, upg = 0, crrnt = 0, reportFile = rFile, pMan = 0, almp = 0, almpScript=0;
    FILE*  file = source;    
    char * line = NULL;
    size_t len = 0;
    FILE * fp;
    fp = fopen("pacman.txt", "r");
    //while (getline(&line, &len, fp) != -1){    
    while(rL(file, rowL, 255) > 0){
        char *p = strstr(rowL, "[PACMAN]");
        char *a = strstr(rowL, "[ALPM]");
        char *ascp = strstr(rowL, "[ALPM-SCRIPTLET]");
        if (p != NULL) pMan++;
        if (a != NULL) almp++;
        if (ascp != NULL) almpScript++;
        if (operation(rowL) == 1){ //Installation
            strcpy(pacP[crrnt].pName, getN(rowL));
            strcpy(pacP[crrnt].insertD, getD(rowL));
            pacP[crrnt].upgradeN = 0;
            strcpy(pacP[crrnt].removalD, "-");
            installed++;
            crrnt++;
        } else if (operation(rowL) == 2){ //Updates
            for (int i = 0; i < 1500; i++){
                if (strcmp(pacP[i].pName, getN(rowL)) == 0){
                    strcpy(pacP[i].uploadD, getD(rowL));
                    upg++;
                    break;
                }
            }
        } else if (operation(rowL) == 3){ //Removals
            for (int i = 0; i < 1500; i++){
                 if (strcmp(pacP[i].pName, getN(rowL)) == 0){
                    strcpy(pacP[i].removalD, getD(rowL));
                }
                break;
            }
            remo++;
        }
        printf("%d\n",operation(rowL));
    }

    write(reportFile, "Pacman Packages Report\n", strlen("Pacman Packages Report\n"));
    write(reportFile,"----------------------\n",strlen("----------------------\n"));
    write(reportFile, "- Installed packages : ", strlen("- Installed packages : "));
    sprintf(temp, "%d\n", installed);
    write(reportFile, temp, strlen(temp));
    write(reportFile, "- Removed packages   : ", strlen("- Removed packages   : "));
    sprintf(temp, "%d\n", remo);
    write(reportFile, temp, strlen(temp));
    write(reportFile, "- Upgraded packages  : ", strlen("- Upgraded packages  : "));
    sprintf(temp, "%d\n", upg);
    write(reportFile, temp, strlen(temp));
    write(reportFile, "- Current installed  : ", strlen("- Current installed  : "));
    sprintf(temp, "%d\n", (installed-remo));
    write(reportFile, temp, strlen(temp));
    write(reportFile,"----------------\n",strlen("----------------\n"));
    write(reportFile, "\n\nGeneral Stats\n", strlen("\n\nGeneral Stats\n"));
    write(reportFile,"----------------\n",strlen("----------------\n"));
    write(reportFile, "- Oldest package   : ", strlen("- Oldest package   : "));
    write(reportFile, pacP[0].pName, strlen(pacP[0].pName));
    write(reportFile, "\n- Newest package   : ", strlen("\n- Newest package   : "));
    write(reportFile, pacP[crrnt-1].pName, strlen(pacP[crrnt-1].pName));
    write(reportFile, "\n- Package with no upgrades   : ", strlen("\n- Package with no upgrades   : "));
    char* noUpgrade[crrnt];
    int j = 0;
    for (int i = 0; i < crrnt; i++) {
        noUpgrade[i] = "";
    }
    for (int i = 0; i < crrnt; i++) {
        noUpgrade[j] = "";
        if(pacP[i].upgradeN == 0){
            noUpgrade[j] = pacP[i].pName;
            j++;
        }
    }
    for (int i = 0; i < crrnt; i++) {
        if(strcmp(noUpgrade[i],"") != 0){
            sprintf(temp, "%s, ", noUpgrade[i]);
            write(reportFile, temp, strlen(temp));
        }
    }
    write(reportFile, "\n- [ALPM-SCRIPTLET] type count   : ", strlen("\n- [ALPM-SCRIPTLET] type count   : "));
    sprintf(temp, "%d\n", almpScript);
    write(reportFile, temp, strlen(temp));
    write(reportFile, "- [ALPM] count   : ", strlen("- [ALPM] count   : "));
    sprintf(temp, "%d\n", almp);
    write(reportFile, temp, strlen(temp));
    write(reportFile, "- [PACMAN] count   : ", strlen("- [PACMAN] count   : "));
    sprintf(temp, "%d\n", pMan);
    write(reportFile, temp, strlen(temp));
    write(reportFile,"----------------\n",strlen("----------------\n"));
    write(reportFile, "\n\nList of Packages\n", strlen("\n\nList of Packages\n"));
    write(reportFile,"----------------\n",strlen("----------------\n"));
    for(int i = 0; i < 1500; i++){
        if(strcmp(pacP[i].pName, "") != 0){
            write(reportFile, "- Package Name   : ", strlen("- Package Name   : "));
            write(reportFile,pacP[i].pName, strlen(pacP[i].pName));
            write(reportFile, "\n   - Install date   : ", strlen("\n   - Install date   : "));
            write(reportFile,pacP[i].insertD, strlen(pacP[i].insertD));
            write(reportFile, "\n   - Last update date   : ", strlen("\n   - Last update date   : "));
            write(reportFile,pacP[i].uploadD, strlen(pacP[i].uploadD));
            write(reportFile, "\n   - How many updates   : ", strlen("\n   - How many updates   : "));
            sprintf(temp, "%d", pacP[i].upgradeN);
            write(reportFile,temp, strlen(temp));
            write(reportFile, "\n   - Removal date   : ", strlen("\n   - Removal date   : "));
            write(reportFile,pacP[i].removalD, strlen(pacP[i].removalD));
            write(reportFile, "\n",strlen("\n"));
        } else if (strcmp(pacP[i].pName, "") == 0){
            break;
        }
    }
}
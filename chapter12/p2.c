/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      Chapter 12 Exercise 1
******************************************************************************/

// List user's process like pstree

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <dirent.h>
#include <assert.h>
#include <pwd.h>

#define BUF_SIZE 1024

char table[3][3][10] = {
    {"┌─","┬─","─┐"},
    {"├─","┼─","─┤"},
    {"└─","┴─","─┘"}
};

char hozn[] = "──";
char vert[] = "│ ";
char space[] = "  ";

typedef struct process process;

void build_tree(process*, int);

struct process{ 
    pid_t pid;
    char* name;
    process *first_child;       /* my first children */
    process *next_sib;          /* my next sibling   */
};

long pid_max(){
    static long maxval = -1;
    if(maxval == -1){
        FILE* f = fopen("/proc/sys/kernel/pid_max", "r");
        if(f == NULL)
            errExit("fopen");
        if(fscanf(f, "%ld", &maxval) != 1)
            errExit("fscanf");
    }

    return maxval;
}

process *get_process(int id){

    static process *processes; /* list of processes */

    if(processes == NULL){
        long size = pid_max() + 5;
        processes = malloc(sizeof(process) * size);
        memset(processes, 0, sizeof(process) * size);
    }

    return &processes[id];
}

// transform username into its uid
uid_t get_uid(const char* name){
    struct passwd *pwd = getpwnam(name);
    if(pwd == NULL)
    {
        if(errno != 0)
            errExit("getpwnam");
        else
            fatal("User %s not found.", name);
    }

    return pwd->pw_uid;
}

// test if given string is numerical string
Boolean isNumericalStr(const char* str){
    for(const char *c = str; *c != '\0'; c++)
        if(!('0' <= *c && *c <= '9'))
            return false;
    return true;
}

// return a pointer to string "/proc/[dirname]/status"
const char* parsing_dirname(const char* dirname){
    static char path[_POSIX_PATH_MAX+10];

    sprintf(path, "/proc/%s/status", dirname);

    return path;
}

int main(int argc, const char *argv[]){
    // Open directory stream
    DIR *proc_dir = opendir("/proc");     
    if(proc_dir == NULL) errExit("opendir");

    // Loop each subdirectory
    for(struct dirent *subdir; (subdir = readdir(proc_dir)) != NULL ; ){

        // numberical only
        if(!isNumericalStr(subdir->d_name)) continue;
        
        // try to open /proc/[that_dir]/status. if failed, next one
        FILE* f = fopen(parsing_dirname(subdir->d_name), "r");
        if( f == NULL ) {errno = 0; continue;};

        // try to read content line by line. if EOF or failed, next one
        char buf[BUF_SIZE];
        char program_name[BUF_SIZE] = {0}; 
        pid_t pid = 0;
        pid_t ppid = 0;
        uid_t uid = 0;
        int parsed = 0;

        // parsing content
        while(fgets(buf, BUF_SIZE, f)){

            if(strncmp(buf, "Name:", 5) == 0)
                sscanf(buf+5, " %s", program_name), parsed++;

            else if(strncmp(buf, "Pid:",  4) == 0)
                sscanf(buf+4, " %d", &pid), parsed++;

            else if(strncmp(buf, "Uid:",  4) == 0)
                sscanf(buf+4, " %d", &uid), parsed++;
            
            else if(strncmp(buf, "PPid:", 5) == 0)
                sscanf(buf+5, " %d", &ppid), parsed++;
        }

        // Information collected, insert it into the tree
        if(parsed == 4){
            process *parent = get_process(ppid);
            process *procs  = get_process(pid);

            procs->name = malloc(sizeof(char) * (strlen(program_name)+20)); 
            sprintf(procs->name, "%s(%ld)", program_name, (long)pid);
            procs->next_sib = parent->first_child;
            parent->first_child = procs;
        }

        errno = 0;
    }

    build_tree(get_process(1), 0);

    if(errno != 0) errExit("readdir");

    return 0;
}

// Algorithm for print that freaking tree
void build_tree(process *pos, int sz){
    static char* content[50000];

    // insert node 
    content[sz++] = pos->name;

    // counting my child process 
    int ccount = 0;
    for(process* p = pos->first_child; p != NULL; p = p->next_sib) 
        ccount++;

    // no more node? print content
    if(ccount == 0){
        /* print it out */
        for(int i = 0;i < sz; i++)
            printf("%s", content[i]);
        printf("\n");

        /* change connector style */
        for(int i = 1;i < sz; i+=2)
            if(content[i] == hozn) content[i] = space;
            else if(content[i] == table[0][1]) content[i] = vert;
            else if(content[i] == table[2][0]) content[i] = space;
            else if(content[i] == table[1][0]) content[i] = vert;

        /* we have to modify the cotent of process name */
        for(int i = 0;i < sz; i+=2)
            for(int j = 0; content[i][j] != '\0'; j++)
                content[i][j] = ' ';    
    }

    // visit my child processes
    int t = 0;
    for(process* p = pos->first_child; p != NULL; p = p->next_sib, t++){
        // insert connector
        if(t == 0)
            content[sz++] = (p->next_sib == NULL) ? hozn : table[0][1];
        else if(t == ccount - 1)
            content[sz++] = table[2][0];
        else 
            content[sz++] = table[1][0];

        // visit next
        build_tree(p, sz);

        // remove connector
        sz--;
    }

    // remove node
    sz--;
}

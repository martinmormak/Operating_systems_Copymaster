#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//moje
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
//

#include "options.h"

void FatalError(char c, const char* msg, int exit_status);
void PrintCopymasterOptions(struct CopymasterOptions* cpm_options);

int main(int argc, char* argv[])
{
    struct CopymasterOptions cpm_options = ParseCopymasterOptions(argc, argv);

    //-------------------------------------------------------------------
    // Kontrola hodnot prepinacov
    //-------------------------------------------------------------------

    // Vypis hodnot prepinacov odstrante z finalnej verzie
    
    PrintCopymasterOptions(&cpm_options);
    
    //-------------------------------------------------------------------
    // Osetrenie prepinacov pred kopirovanim
    //-------------------------------------------------------------------
    
    if (cpm_options.fast && cpm_options.slow) {
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if (cpm_options.create && cpm_options.overwrite) {
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    /*if (cpm_options.create && cpm_options.append) {
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }*/

    if (cpm_options.overwrite && cpm_options.append) {
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.lseek && cpm_options.overwrite){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.lseek && cpm_options.append){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.lseek && cpm_options.create){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.lseek && cpm_options.link){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.create && cpm_options.link){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.append && cpm_options.link){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }

    if(cpm_options.overwrite && cpm_options.link){
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        return 42;
        exit(EXIT_FAILURE);
    }
    
    // TODO Nezabudnut dalsie kontroly kombinacii prepinacov ...
    
    //-------------------------------------------------------------------
    // Kopirovanie suborov
    //-------------------------------------------------------------------
    
    int ifd,ofd,n,flags,speed=1000000/*,permissions[4]={0,0,0,0}*/;
    struct stat statBuf;
    char buf[1000000];

    //Bez akéhokoľvek prepínača
    if(!cpm_options.fast&&!cpm_options.slow&&!cpm_options.create&&!cpm_options.overwrite&&!cpm_options.append&&!cpm_options.lseek&&!cpm_options.directory&&!cpm_options.delete_opt&&!cpm_options.chmod&&!cpm_options.inode&&!cpm_options.umask&&!cpm_options.link&&!cpm_options.truncate&&!cpm_options.sparse)
    {
        if((ifd=open(cpm_options.infile,O_RDONLY))<0)
        {
            /*fprintf(stderr, "B:%d:%s:SUBOR NEEXISTUJE\n",errno,strerror(errno));
            return 21;*/
            FatalError('B',"SUBOR NEEXISTUJE",21);
        }
        stat(cpm_options.infile,&statBuf);
        //printf("%d%d%d%d",permissions[0],permissions[1],permissions[2],permissions[3]);
        if((ofd=open(cpm_options.outfile,O_RDWR|O_CREAT|O_TRUNC,statBuf.st_mode))<0)
        {
            /*fprintf(stderr, "B:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 21;*/
            FatalError('B',"INA CHYBA",21);
        }
        while ((n = read(ifd,&buf,1)) > 0)
        {
            write(ofd,&buf,n);
        }
        close(ofd);
        close(ifd);
        return 0;
    }

    flags=O_RDWR|O_CREAT|O_TRUNC;
    stat(cpm_options.infile,&statBuf);

    //fast
    if(cpm_options.fast)
    {
        speed=1000000;
        stat(cpm_options.infile,&statBuf);
        flags=O_RDWR|O_CREAT|O_TRUNC;
    }

    //slow
    if(cpm_options.slow)
    {
        speed=1;
        stat(cpm_options.infile,&statBuf);
        flags=O_RDWR|O_CREAT|O_TRUNC;
    }
    //create
    if(cpm_options.create)
    {
        //printf("%d",cpm_options.create_mode);
        statBuf.st_mode=cpm_options.create_mode;
        flags=O_RDWR|O_CREAT|O_EXCL;
        //printf("%d",flags);
    }

    //overwrite
    if(cpm_options.overwrite)
    {
        flags=O_RDWR;
        //printf("%d",flags);
    }

    //append
    if(cpm_options.append)
    {
        flags=O_RDWR|O_APPEND;
        //printf("%d",flags);
    }

    if(cpm_options.create&&cpm_options.append)
    {
        flags=O_RDWR|O_CREAT|O_EXCL|O_APPEND;
        //printf("%d",flags);
    }

    if(cpm_options.lseek)
    {
        if((ifd=open(cpm_options.infile,O_RDONLY))<0)
        {
            /*fprintf(stderr, "l:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 33;*/
            FatalError('l',"INA CHYBA",33);
        }
        if((ofd=open(cpm_options.outfile,O_RDWR))<0)
        {
            /*fprintf(stderr, "l:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 33;*/
            FatalError('l',"INA CHYBA",33);
        }
        if(lseek(ifd,cpm_options.lseek_options.pos1,SEEK_SET)<0)
        {
            /*fprintf(stderr, "l:%d:%s:CHYBA POZICIE infile\n",errno,strerror(errno));
            return 33;*/
            FatalError('l',"CHYBA POZICIE infile",33);
        }
        if(lseek(ofd,cpm_options.lseek_options.pos2,cpm_options.lseek_options.x)<0)
        {
            /*fprintf(stderr, "l:%d:%s:CHYBA POZICIE outfile\n",errno,strerror(errno));
            return 33;*/
            FatalError('l',"CHYBA POZICIE outfile",33);
        }
        n = read(ifd,&buf,cpm_options.lseek_options.num);
        write(ofd,&buf,n);
        write(1,&buf,n);
        close(ofd);
        close(ifd);
        return 0;
    }
    
    if(cpm_options.link)
    {
        if(link(cpm_options.infile,cpm_options.outfile)<0)
        {
            if(errno==EEXIST)
            {
                FatalError('K',"VYSTUPNY SUBOR UZ EXISTUJE",30);
            }
            else
            {
                FatalError('K',"VSTUPNY SUBOR NEEXISTUJE",30);
            }
        }
        return 0;
    }
    
    if((ifd=open(cpm_options.infile,O_RDONLY))<0)
    {
        if(flags==(O_RDWR|O_CREAT|O_EXCL))
        {
            /*fprintf(stderr, "c:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 23;*/
            FatalError('c',"INA CHYBA",23);
        }
        else if(flags==(O_RDWR))
        {
            /*fprintf(stderr, "o:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 24;*/
            FatalError('o',"INA CHYBA",24);
        }
        else if(flags==(O_RDWR|O_APPEND))
        {
            /*fprintf(stderr, "a:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 22;*/
            FatalError('a',"INA CHYBA",22);
        }
        else
        {
            /*fprintf(stderr, "B:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 21;*/
            FatalError('B',"INA CHYBA",21);
        }
    }

    if(cpm_options.inode)
    {
        if(statBuf.st_ino!=cpm_options.inode_number)
        {
            errno=EINVAL;
            FatalError('i',"ZLY INODE",27);
        }
        if(S_ISDIR(statBuf.st_mode))
        {
            errno=EINVAL;
            FatalError('i',"ZLY TYP VSTUPNEHO SUBORU",27);
        }
    }

    if((ofd=open(cpm_options.outfile,flags,statBuf.st_mode))<0)
    {
        if(flags==(O_RDWR|O_CREAT|O_EXCL))
        {
            /*fprintf(stderr, "c:%d:%s:SUBOR EXISTUJE\n",errno,strerror(errno));
            return 23;*/
            FatalError('c',"SUBOR EXISTUJE",23);
        }
        else if(flags==(O_RDWR))
        {
            /*fprintf(stderr, "o:%d:%s:SUBOR NEEXISTUJE\n",errno,strerror(errno));
            return 24;*/
            FatalError('o',"SUBOR NEEXISTUJE",24);
        }
        else if(flags==(O_RDWR|O_APPEND))
        {
            /*fprintf(stderr, "a:%d:%s:SUBOR NEEXISTUJE\n",errno,strerror(errno));
            return 22;*/
            FatalError('a',"SUBOR NEEXISTUJE",22);
        }
        else
        {
            /*fprintf(stderr, "B:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 21;*/
            FatalError('B',"INA CHYBA",21);
        }
    }
    while ((n = read(ifd,&buf,speed)) > 0)
    {
        write(ofd,&buf,n);
    }
    close(ofd);
    close(ifd);

    if(cpm_options.delete_opt)
    {
        if(S_ISDIR(statBuf.st_mode))
        {
            errno=EINVAL;
            FatalError('d',"SUBOR NEBOL ZMAZANY",26);
        }
        if(unlink(cpm_options.infile)<0)
        {
            /*fprintf(stderr, "d:%d:%s:SUBOR NEBOL ZMAZANY\n",errno,strerror(errno));
            return 26;*/
            FatalError('d',"SUBOR NEBOL ZMAZANY",26);
        }
    }

    if(cpm_options.truncate)
    {
        if(cpm_options.truncate_size<0)
        {
            errno=EINVAL;
            FatalError('t',"ZAPORNA VELKOST",31);
        }
        if(S_ISDIR(statBuf.st_mode))
        {
            errno=EINVAL;
            FatalError('t',"INA CHYBA",31);
        }
        if(truncate(cpm_options.infile, cpm_options.truncate_size)<0)
        {
            /*fprintf(stderr, "d:%d:%s:SUBOR NEBOL ZMAZANY\n",errno,strerror(errno));
            return 26;*/
            FatalError('t',"INA CHYBA",31);
        }
    }
    
    if(cpm_options.chmod)
    {
        if(chmod(cpm_options.outfile,cpm_options.chmod_mode)<0)
        {
            /*fprintf(stderr, "m:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 34;*/
            FatalError('m',"INA CHYBA",34);
        }
    }

    // TODO Implementovat kopirovanie suborov
    
    // cpm_options.infile
    // cpm_options.outfile
    
    //-------------------------------------------------------------------
    // Vypis adresara
    //-------------------------------------------------------------------
    
    if(cpm_options.directory)
    {
        DIR *directory;
        struct dirent *stDirent;
        struct stat stStat;
        struct tm *stTime;
        if((directory=opendir(cpm_options.infile))==NULL)
        {
            if(errno==ENOTDIR)
            {
                /*fprintf(stderr, "D:%d:%s:VSTUPNY SUBOR NIE JE ADRESAR\n",errno,strerror(errno));
                return 28;*/
                FatalError('D',"VSTUPNY SUBOR NIE JE ADRESAR",28);
            }
            else
            {
                /*fprintf(stderr, "D:%d:%s:INA CHYBA\n",errno,strerror(errno));
                return 28;*/
                FatalError('D',"INA CHYBA",28);
            }
        }
        if((ofd=open(cpm_options.outfile,flags,statBuf.st_mode))<0)
        {
            /*fprintf(stderr, "D:%d:%s:VYSTUPNY SUBOR - CHYBA\n",errno,strerror(errno));
            return 28;*/
            FatalError('D',"VYSTUPNY SUBOR - CHYBA",28);
        }
        while ((stDirent=readdir(directory))!=NULL)
        {
            stat(stDirent->d_name,&stStat);
            if(S_ISDIR(stStat.st_mode)){dprintf(ofd,"d");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IRUSR){dprintf(ofd,"r");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IWUSR){dprintf(ofd,"w");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IXUSR){dprintf(ofd,"x");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IRGRP){dprintf(ofd,"r");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IWGRP){dprintf(ofd,"w");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IXGRP){dprintf(ofd,"x");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IROTH){dprintf(ofd,"r");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IWOTH){dprintf(ofd,"w");}else{dprintf(ofd,"-");}
            if(stStat.st_mode & S_IXOTH){dprintf(ofd,"x");}else{dprintf(ofd,"-");}
            stTime=localtime(&stStat.st_mtime);
            dprintf(ofd,"\t%ld\t%d\t%d\t%ld\t%02d-%02d-%02d\t%s\n",stStat.st_nlink,stStat.st_uid,stStat.st_gid,stStat.st_size,stTime->tm_mday,stTime->tm_mon+1,stTime->tm_year+1900,stDirent->d_name);
        }
        close(ofd);
        closedir(directory);
        return 0;
    }
        
    //-------------------------------------------------------------------
    // Osetrenie prepinacov po kopirovani
    //-------------------------------------------------------------------
    
    // TODO Implementovat osetrenie prepinacov po kopirovani
    
    return 0;
}

void FatalError(char c, const char* msg, int exit_status)
{
                /*fprintf(stderr, "D:%d:%s:INA CHYBA\n",errno,strerror(errno));
                return 28;*/
    fprintf(stderr, "%c:%d:", c, errno); 
    fprintf(stderr, "%s:", strerror(errno));
    fprintf(stderr, "%s\n", msg);
    exit(exit_status);
}

void PrintCopymasterOptions(struct CopymasterOptions* cpm_options)
{
    if (cpm_options == 0)
        return;
    
    printf("infile:        %s\n", cpm_options->infile);
    printf("outfile:       %s\n", cpm_options->outfile);
    
    printf("fast:          %d\n", cpm_options->fast);
    printf("slow:          %d\n", cpm_options->slow);
    printf("create:        %d\n", cpm_options->create);
    printf("create_mode:   %o\n", (unsigned int)cpm_options->create_mode);
    printf("overwrite:     %d\n", cpm_options->overwrite);
    printf("append:        %d\n", cpm_options->append);
    printf("lseek:         %d\n", cpm_options->lseek);
    
    printf("lseek_options.x:    %d\n", cpm_options->lseek_options.x);
    printf("lseek_options.pos1: %ld\n", cpm_options->lseek_options.pos1);
    printf("lseek_options.pos2: %ld\n", cpm_options->lseek_options.pos2);
    printf("lseek_options.num:  %lu\n", cpm_options->lseek_options.num);
    
    printf("directory:     %d\n", cpm_options->directory);
    printf("delete_opt:    %d\n", cpm_options->delete_opt);
    printf("chmod:         %d\n", cpm_options->chmod);
    printf("chmod_mode:    %o\n", (unsigned int)cpm_options->chmod_mode);
    printf("inode:         %d\n", cpm_options->inode);
    printf("inode_number:  %lu\n", cpm_options->inode_number);
    
    printf("umask:\t%d\n", cpm_options->umask);
    for(unsigned int i=0; i<kUMASK_OPTIONS_MAX_SZ; ++i) {
        if (cpm_options->umask_options[i][0] == 0) {
            // dosli sme na koniec zoznamu nastaveni umask
            break;
        }
        printf("umask_options[%u]: %s\n", i, cpm_options->umask_options[i]);
    }
    
    printf("link:          %d\n", cpm_options->link);
    printf("truncate:      %d\n", cpm_options->truncate);
    printf("truncate_size: %ld\n", cpm_options->truncate_size);
    printf("sparse:        %d\n", cpm_options->sparse);
}
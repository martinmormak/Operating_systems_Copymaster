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
            fprintf(stderr, "B:%d:%s:SUBOR NEEXISTUJE\n",errno,strerror(errno));
            return 21;
        }
        stat(cpm_options.infile,&statBuf);
        /*if(statBuf.st_mode & S_IRUSR)
        {
            permissions[1]=permissions[1]+4;
        }
        if(statBuf.st_mode & S_IWUSR)
        {
            permissions[1]=permissions[1]+2;
        }
        if(statBuf.st_mode & S_IXUSR)
        {
            permissions[1]=permissions[1]+1;
        }
        if(statBuf.st_mode & S_IRGRP)
        {
            permissions[2]=permissions[2]+4;
        }
        if(statBuf.st_mode & S_IWGRP)
        {
            permissions[2]=permissions[2]+2;
        }
        if(statBuf.st_mode & S_IXGRP)
        {
            permissions[2]=permissions[2]+1;
        }
        if(statBuf.st_mode & S_IROTH)
        {
            permissions[3]=permissions[3]+4;
        }
        if(statBuf.st_mode & S_IWOTH)
        {
            permissions[3]=permissions[3]+2;
        }
        if(statBuf.st_mode & S_IXOTH)
        {
            permissions[3]=permissions[3]+1;
        }*/
        //printf("%d%d%d%d",permissions[0],permissions[1],permissions[2],permissions[3]);
        if((ofd=open(cpm_options.outfile,O_RDWR|O_CREAT|O_TRUNC,statBuf.st_mode))<0)
        {
            fprintf(stderr, "B:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 21;
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
        if(cpm_options.create_mode!=0&&cpm_options.create_mode!=1&&cpm_options.create_mode!=2&&cpm_options.create_mode!=3&&cpm_options.create_mode!=4&&cpm_options.create_mode!=5&&cpm_options.create_mode!=6&&cpm_options.create_mode!=7&&
        cpm_options.create_mode!=8&&cpm_options.create_mode!=9&&cpm_options.create_mode!=10&&cpm_options.create_mode!=11&&cpm_options.create_mode!=12&&cpm_options.create_mode!=13&&cpm_options.create_mode!=14&&cpm_options.create_mode!=15&&
        cpm_options.create_mode!=16&&cpm_options.create_mode!=17&&cpm_options.create_mode!=18&&cpm_options.create_mode!=19&&cpm_options.create_mode!=20&&cpm_options.create_mode!=21&&cpm_options.create_mode!=22&&cpm_options.create_mode!=23&&
        cpm_options.create_mode!=24&&cpm_options.create_mode!=25&&cpm_options.create_mode!=26&&cpm_options.create_mode!=27&&cpm_options.create_mode!=28&&cpm_options.create_mode!=29&&cpm_options.create_mode!=30&&cpm_options.create_mode!=31&&
        cpm_options.create_mode!=32&&cpm_options.create_mode!=33&&cpm_options.create_mode!=34&&cpm_options.create_mode!=35&&cpm_options.create_mode!=36&&cpm_options.create_mode!=37&&cpm_options.create_mode!=38&&cpm_options.create_mode!=39&&
        cpm_options.create_mode!=40&&cpm_options.create_mode!=41&&cpm_options.create_mode!=42&&cpm_options.create_mode!=43&&cpm_options.create_mode!=44&&cpm_options.create_mode!=45&&cpm_options.create_mode!=46&&cpm_options.create_mode!=47&&
        cpm_options.create_mode!=48&&cpm_options.create_mode!=49&&cpm_options.create_mode!=50&&cpm_options.create_mode!=51&&cpm_options.create_mode!=52&&cpm_options.create_mode!=53&&cpm_options.create_mode!=54&&cpm_options.create_mode!=55&&
        cpm_options.create_mode!=56&&cpm_options.create_mode!=57&&cpm_options.create_mode!=58&&cpm_options.create_mode!=59&&cpm_options.create_mode!=60&&cpm_options.create_mode!=61&&cpm_options.create_mode!=62&&cpm_options.create_mode!=63&&
        cpm_options.create_mode!=64&&cpm_options.create_mode!=65&&cpm_options.create_mode!=66&&cpm_options.create_mode!=67&&cpm_options.create_mode!=68&&cpm_options.create_mode!=69&&cpm_options.create_mode!=70&&cpm_options.create_mode!=71&&
        cpm_options.create_mode!=72&&cpm_options.create_mode!=73&&cpm_options.create_mode!=74&&cpm_options.create_mode!=75&&cpm_options.create_mode!=76&&cpm_options.create_mode!=77&&cpm_options.create_mode!=78&&cpm_options.create_mode!=79&&
        cpm_options.create_mode!=80&&cpm_options.create_mode!=81&&cpm_options.create_mode!=82&&cpm_options.create_mode!=83&&cpm_options.create_mode!=84&&cpm_options.create_mode!=85&&cpm_options.create_mode!=86&&cpm_options.create_mode!=87&&
        cpm_options.create_mode!=88&&cpm_options.create_mode!=89&&cpm_options.create_mode!=90&&cpm_options.create_mode!=91&&cpm_options.create_mode!=92&&cpm_options.create_mode!=93&&cpm_options.create_mode!=94&&cpm_options.create_mode!=95&&
        cpm_options.create_mode!=96&&cpm_options.create_mode!=97&&cpm_options.create_mode!=98&&cpm_options.create_mode!=99&&cpm_options.create_mode!=100&&cpm_options.create_mode!=101&&cpm_options.create_mode!=102&&cpm_options.create_mode!=103&&
        cpm_options.create_mode!=104&&cpm_options.create_mode!=105&&cpm_options.create_mode!=106&&cpm_options.create_mode!=107&&cpm_options.create_mode!=108&&cpm_options.create_mode!=109&&cpm_options.create_mode!=110&&cpm_options.create_mode!=111&&
        cpm_options.create_mode!=112&&cpm_options.create_mode!=113&&cpm_options.create_mode!=114&&cpm_options.create_mode!=115&&cpm_options.create_mode!=116&&cpm_options.create_mode!=117&&cpm_options.create_mode!=118&&cpm_options.create_mode!=119&&
        cpm_options.create_mode!=120&&cpm_options.create_mode!=121&&cpm_options.create_mode!=122&&cpm_options.create_mode!=123&&cpm_options.create_mode!=124&&cpm_options.create_mode!=125&&cpm_options.create_mode!=126&&cpm_options.create_mode!=127&&
        cpm_options.create_mode!=128&&cpm_options.create_mode!=129&&cpm_options.create_mode!=130&&cpm_options.create_mode!=131&&cpm_options.create_mode!=132&&cpm_options.create_mode!=133&&cpm_options.create_mode!=134&&cpm_options.create_mode!=135&&
        cpm_options.create_mode!=136&&cpm_options.create_mode!=137&&cpm_options.create_mode!=138&&cpm_options.create_mode!=139&&cpm_options.create_mode!=140&&cpm_options.create_mode!=141&&cpm_options.create_mode!=142&&cpm_options.create_mode!=143&&
        cpm_options.create_mode!=144&&cpm_options.create_mode!=145&&cpm_options.create_mode!=146&&cpm_options.create_mode!=147&&cpm_options.create_mode!=148&&cpm_options.create_mode!=149&&cpm_options.create_mode!=150&&cpm_options.create_mode!=151&&
        cpm_options.create_mode!=152&&cpm_options.create_mode!=153&&cpm_options.create_mode!=154&&cpm_options.create_mode!=155&&cpm_options.create_mode!=156&&cpm_options.create_mode!=157&&cpm_options.create_mode!=158&&cpm_options.create_mode!=159&&
        cpm_options.create_mode!=160&&cpm_options.create_mode!=161&&cpm_options.create_mode!=162&&cpm_options.create_mode!=163&&cpm_options.create_mode!=164&&cpm_options.create_mode!=165&&cpm_options.create_mode!=166&&cpm_options.create_mode!=167&&
        cpm_options.create_mode!=168&&cpm_options.create_mode!=169&&cpm_options.create_mode!=170&&cpm_options.create_mode!=171&&cpm_options.create_mode!=172&&cpm_options.create_mode!=173&&cpm_options.create_mode!=174&&cpm_options.create_mode!=175&&
        cpm_options.create_mode!=176&&cpm_options.create_mode!=177&&cpm_options.create_mode!=178&&cpm_options.create_mode!=179&&cpm_options.create_mode!=180&&cpm_options.create_mode!=181&&cpm_options.create_mode!=182&&cpm_options.create_mode!=183&&
        cpm_options.create_mode!=184&&cpm_options.create_mode!=185&&cpm_options.create_mode!=186&&cpm_options.create_mode!=187&&cpm_options.create_mode!=188&&cpm_options.create_mode!=189&&cpm_options.create_mode!=190&&cpm_options.create_mode!=191&&
        cpm_options.create_mode!=192&&cpm_options.create_mode!=193&&cpm_options.create_mode!=194&&cpm_options.create_mode!=195&&cpm_options.create_mode!=196&&cpm_options.create_mode!=197&&cpm_options.create_mode!=198&&cpm_options.create_mode!=199&&
        cpm_options.create_mode!=200&&cpm_options.create_mode!=201&&cpm_options.create_mode!=202&&cpm_options.create_mode!=203&&cpm_options.create_mode!=204&&cpm_options.create_mode!=205&&cpm_options.create_mode!=206&&cpm_options.create_mode!=207&&
        cpm_options.create_mode!=208&&cpm_options.create_mode!=209&&cpm_options.create_mode!=210&&cpm_options.create_mode!=211&&cpm_options.create_mode!=212&&cpm_options.create_mode!=213&&cpm_options.create_mode!=214&&cpm_options.create_mode!=215&&
        cpm_options.create_mode!=216&&cpm_options.create_mode!=217&&cpm_options.create_mode!=218&&cpm_options.create_mode!=219&&cpm_options.create_mode!=220&&cpm_options.create_mode!=221&&cpm_options.create_mode!=222&&cpm_options.create_mode!=223&&
        cpm_options.create_mode!=224&&cpm_options.create_mode!=225&&cpm_options.create_mode!=226&&cpm_options.create_mode!=227&&cpm_options.create_mode!=228&&cpm_options.create_mode!=229&&cpm_options.create_mode!=230&&cpm_options.create_mode!=231&&
        cpm_options.create_mode!=232&&cpm_options.create_mode!=233&&cpm_options.create_mode!=234&&cpm_options.create_mode!=235&&cpm_options.create_mode!=236&&cpm_options.create_mode!=237&&cpm_options.create_mode!=238&&cpm_options.create_mode!=239&&
        cpm_options.create_mode!=240&&cpm_options.create_mode!=241&&cpm_options.create_mode!=242&&cpm_options.create_mode!=243&&cpm_options.create_mode!=244&&cpm_options.create_mode!=245&&cpm_options.create_mode!=246&&cpm_options.create_mode!=247&&
        cpm_options.create_mode!=248&&cpm_options.create_mode!=249&&cpm_options.create_mode!=250&&cpm_options.create_mode!=251&&cpm_options.create_mode!=252&&cpm_options.create_mode!=253&&cpm_options.create_mode!=254&&cpm_options.create_mode!=255&&
        cpm_options.create_mode!=256&&cpm_options.create_mode!=257&&cpm_options.create_mode!=258&&cpm_options.create_mode!=259&&cpm_options.create_mode!=260&&cpm_options.create_mode!=261&&cpm_options.create_mode!=262&&cpm_options.create_mode!=263&&
        cpm_options.create_mode!=264&&cpm_options.create_mode!=265&&cpm_options.create_mode!=266&&cpm_options.create_mode!=267&&cpm_options.create_mode!=268&&cpm_options.create_mode!=269&&cpm_options.create_mode!=270&&cpm_options.create_mode!=271&&
        cpm_options.create_mode!=272&&cpm_options.create_mode!=273&&cpm_options.create_mode!=274&&cpm_options.create_mode!=275&&cpm_options.create_mode!=276&&cpm_options.create_mode!=277&&cpm_options.create_mode!=278&&cpm_options.create_mode!=279&&
        cpm_options.create_mode!=280&&cpm_options.create_mode!=281&&cpm_options.create_mode!=282&&cpm_options.create_mode!=283&&cpm_options.create_mode!=284&&cpm_options.create_mode!=285&&cpm_options.create_mode!=286&&cpm_options.create_mode!=287&&
        cpm_options.create_mode!=288&&cpm_options.create_mode!=289&&cpm_options.create_mode!=290&&cpm_options.create_mode!=291&&cpm_options.create_mode!=292&&cpm_options.create_mode!=293&&cpm_options.create_mode!=294&&cpm_options.create_mode!=295&&
        cpm_options.create_mode!=296&&cpm_options.create_mode!=297&&cpm_options.create_mode!=298&&cpm_options.create_mode!=299&&cpm_options.create_mode!=300&&cpm_options.create_mode!=301&&cpm_options.create_mode!=302&&cpm_options.create_mode!=303&&
        cpm_options.create_mode!=304&&cpm_options.create_mode!=305&&cpm_options.create_mode!=306&&cpm_options.create_mode!=307&&cpm_options.create_mode!=308&&cpm_options.create_mode!=309&&cpm_options.create_mode!=310&&cpm_options.create_mode!=311&&
        cpm_options.create_mode!=312&&cpm_options.create_mode!=313&&cpm_options.create_mode!=314&&cpm_options.create_mode!=315&&cpm_options.create_mode!=316&&cpm_options.create_mode!=317&&cpm_options.create_mode!=318&&cpm_options.create_mode!=319&&
        cpm_options.create_mode!=320&&cpm_options.create_mode!=321&&cpm_options.create_mode!=322&&cpm_options.create_mode!=323&&cpm_options.create_mode!=324&&cpm_options.create_mode!=325&&cpm_options.create_mode!=326&&cpm_options.create_mode!=327&&
        cpm_options.create_mode!=328&&cpm_options.create_mode!=329&&cpm_options.create_mode!=330&&cpm_options.create_mode!=331&&cpm_options.create_mode!=332&&cpm_options.create_mode!=333&&cpm_options.create_mode!=334&&cpm_options.create_mode!=335&&
        cpm_options.create_mode!=336&&cpm_options.create_mode!=337&&cpm_options.create_mode!=338&&cpm_options.create_mode!=339&&cpm_options.create_mode!=340&&cpm_options.create_mode!=341&&cpm_options.create_mode!=342&&cpm_options.create_mode!=343&&
        cpm_options.create_mode!=344&&cpm_options.create_mode!=345&&cpm_options.create_mode!=346&&cpm_options.create_mode!=347&&cpm_options.create_mode!=348&&cpm_options.create_mode!=349&&cpm_options.create_mode!=350&&cpm_options.create_mode!=351&&
        cpm_options.create_mode!=352&&cpm_options.create_mode!=353&&cpm_options.create_mode!=354&&cpm_options.create_mode!=355&&cpm_options.create_mode!=356&&cpm_options.create_mode!=357&&cpm_options.create_mode!=358&&cpm_options.create_mode!=359&&
        cpm_options.create_mode!=360&&cpm_options.create_mode!=361&&cpm_options.create_mode!=362&&cpm_options.create_mode!=363&&cpm_options.create_mode!=364&&cpm_options.create_mode!=365&&cpm_options.create_mode!=366&&cpm_options.create_mode!=367&&
        cpm_options.create_mode!=368&&cpm_options.create_mode!=369&&cpm_options.create_mode!=370&&cpm_options.create_mode!=371&&cpm_options.create_mode!=372&&cpm_options.create_mode!=373&&cpm_options.create_mode!=374&&cpm_options.create_mode!=375&&
        cpm_options.create_mode!=376&&cpm_options.create_mode!=377&&cpm_options.create_mode!=378&&cpm_options.create_mode!=379&&cpm_options.create_mode!=380&&cpm_options.create_mode!=381&&cpm_options.create_mode!=382&&cpm_options.create_mode!=383&&
        cpm_options.create_mode!=384&&cpm_options.create_mode!=385&&cpm_options.create_mode!=386&&cpm_options.create_mode!=387&&cpm_options.create_mode!=388&&cpm_options.create_mode!=389&&cpm_options.create_mode!=390&&cpm_options.create_mode!=391&&
        cpm_options.create_mode!=392&&cpm_options.create_mode!=393&&cpm_options.create_mode!=394&&cpm_options.create_mode!=395&&cpm_options.create_mode!=396&&cpm_options.create_mode!=397&&cpm_options.create_mode!=398&&cpm_options.create_mode!=399&&
        cpm_options.create_mode!=400&&cpm_options.create_mode!=401&&cpm_options.create_mode!=402&&cpm_options.create_mode!=403&&cpm_options.create_mode!=404&&cpm_options.create_mode!=405&&cpm_options.create_mode!=406&&cpm_options.create_mode!=407&&
        cpm_options.create_mode!=408&&cpm_options.create_mode!=409&&cpm_options.create_mode!=410&&cpm_options.create_mode!=411&&cpm_options.create_mode!=412&&cpm_options.create_mode!=413&&cpm_options.create_mode!=414&&cpm_options.create_mode!=415&&
        cpm_options.create_mode!=416&&cpm_options.create_mode!=417&&cpm_options.create_mode!=418&&cpm_options.create_mode!=419&&cpm_options.create_mode!=420&&cpm_options.create_mode!=421&&cpm_options.create_mode!=422&&cpm_options.create_mode!=423&&
        cpm_options.create_mode!=424&&cpm_options.create_mode!=425&&cpm_options.create_mode!=426&&cpm_options.create_mode!=427&&cpm_options.create_mode!=428&&cpm_options.create_mode!=429&&cpm_options.create_mode!=430&&cpm_options.create_mode!=431&&
        cpm_options.create_mode!=432&&cpm_options.create_mode!=433&&cpm_options.create_mode!=434&&cpm_options.create_mode!=435&&cpm_options.create_mode!=436&&cpm_options.create_mode!=437&&cpm_options.create_mode!=438&&cpm_options.create_mode!=439&&
        cpm_options.create_mode!=440&&cpm_options.create_mode!=441&&cpm_options.create_mode!=442&&cpm_options.create_mode!=443&&cpm_options.create_mode!=444&&cpm_options.create_mode!=445&&cpm_options.create_mode!=446&&cpm_options.create_mode!=447&&
        cpm_options.create_mode!=448&&cpm_options.create_mode!=449&&cpm_options.create_mode!=450&&cpm_options.create_mode!=451&&cpm_options.create_mode!=452&&cpm_options.create_mode!=453&&cpm_options.create_mode!=454&&cpm_options.create_mode!=455&&
        cpm_options.create_mode!=456&&cpm_options.create_mode!=457&&cpm_options.create_mode!=458&&cpm_options.create_mode!=459&&cpm_options.create_mode!=460&&cpm_options.create_mode!=461&&cpm_options.create_mode!=462&&cpm_options.create_mode!=463&&
        cpm_options.create_mode!=464&&cpm_options.create_mode!=465&&cpm_options.create_mode!=466&&cpm_options.create_mode!=467&&cpm_options.create_mode!=468&&cpm_options.create_mode!=469&&cpm_options.create_mode!=470&&cpm_options.create_mode!=471&&
        cpm_options.create_mode!=472&&cpm_options.create_mode!=473&&cpm_options.create_mode!=474&&cpm_options.create_mode!=475&&cpm_options.create_mode!=476&&cpm_options.create_mode!=477&&cpm_options.create_mode!=478&&cpm_options.create_mode!=479&&
        cpm_options.create_mode!=480&&cpm_options.create_mode!=481&&cpm_options.create_mode!=482&&cpm_options.create_mode!=483&&cpm_options.create_mode!=484&&cpm_options.create_mode!=485&&cpm_options.create_mode!=486&&cpm_options.create_mode!=487&&
        cpm_options.create_mode!=488&&cpm_options.create_mode!=489&&cpm_options.create_mode!=490&&cpm_options.create_mode!=491&&cpm_options.create_mode!=492&&cpm_options.create_mode!=493&&cpm_options.create_mode!=494&&cpm_options.create_mode!=495&&
        cpm_options.create_mode!=496&&cpm_options.create_mode!=497&&cpm_options.create_mode!=498&&cpm_options.create_mode!=499&&cpm_options.create_mode!=500&&cpm_options.create_mode!=501&&cpm_options.create_mode!=502&&cpm_options.create_mode!=503&&
        cpm_options.create_mode!=504&&cpm_options.create_mode!=505&&cpm_options.create_mode!=506&&cpm_options.create_mode!=507&&cpm_options.create_mode!=508&&cpm_options.create_mode!=509&&cpm_options.create_mode!=510&&cpm_options.create_mode!=511)
        {
            fprintf(stderr, "c:%d:%s:ZLE PRAVA\n",errno,strerror(errno));
            return 23;
        }
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
            fprintf(stderr, "l:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 33;
        }
        if((ofd=open(cpm_options.outfile,O_RDWR))<0)
        {
            fprintf(stderr, "l:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 33;
        }
        if(lseek(ifd,cpm_options.lseek_options.pos1,SEEK_SET)<0)
        {
            fprintf(stderr, "l:%d:%s:CHYBA POZICIE infile\n",errno,strerror(errno));
            return 33;
        }
        if(lseek(ofd,cpm_options.lseek_options.pos2,cpm_options.lseek_options.x)<0)
        {
            fprintf(stderr, "l:%d:%s:CHYBA POZICIE outfile\n",errno,strerror(errno));
            return 33;
        }
        n = read(ifd,&buf,cpm_options.lseek_options.num);
        write(ofd,&buf,n);
        write(1,&buf,n);
        close(ofd);
        close(ifd);
        return 0;
    }

    // if(cpm_options.directory)
    // {
    //     DIR *directory;
    //     struct dirent *stDirent;
    //     struct stat stStat;
    //     struct tm *stTime;
    //     if((directory=opendir(cpm_options.infile))==NULL)
    //     {
    //         if(errno==ENOTDIR)
    //         {
    //             fprintf(stderr, "D:%d:%s:VSTUPNY SUBOR NIE JE ADRESAR\n",errno,strerror(errno));
    //             return 28;
    //         }
    //         else
    //         {
    //             fprintf(stderr, "D:%d:%s:INA CHYBA\n",errno,strerror(errno));
    //             return 28;
    //         }
    //     }
    //     if((ofd=open(cpm_options.outfile,flags,statBuf.st_mode))<0)
    //     {
    //         fprintf(stderr, "D:%d:%s:VYSTUPNY SUBOR - CHYBA\n",errno,strerror(errno));
    //         return 28;
    //     }
    //     while ((stDirent=readdir(directory))!=NULL)
    //     {
    //         stat(stDirent->d_name,&stStat);
    //         if(S_ISDIR(stStat.st_mode)){dprintf(ofd,"d");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IRUSR){dprintf(ofd,"r");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IWUSR){dprintf(ofd,"w");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IXUSR){dprintf(ofd,"x");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IRGRP){dprintf(ofd,"r");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IWGRP){dprintf(ofd,"w");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IXGRP){dprintf(ofd,"x");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IROTH){dprintf(ofd,"r");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IWOTH){dprintf(ofd,"w");}else{dprintf(ofd,"-");}
    //         if(stStat.st_mode & S_IXOTH){dprintf(ofd,"x");}else{dprintf(ofd,"-");}
    //         stTime=localtime(&stStat.st_mtime);
    //         dprintf(ofd,"\t%ld\t%d\t%d\t%ld\t%02d-%02d-%02d\t%s\n",stStat.st_nlink,stStat.st_uid,stStat.st_gid,stStat.st_size,stTime->tm_mday,stTime->tm_mon+1,stTime->tm_year+1900,stDirent->d_name);
    //     }
    //     close(ofd);
    //     closedir(directory);
    //     return 0;
    // }
    
    if((ifd=open(cpm_options.infile,O_RDONLY))<0)
    {
        if(flags==(O_RDWR|O_CREAT|O_EXCL))
        {
            fprintf(stderr, "c:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 23;
        }
        else if(flags==(O_RDWR))
        {
            fprintf(stderr, "o:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 24;
        }
        else if(flags==(O_RDWR|O_APPEND))
        {
            fprintf(stderr, "a:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 22;
        }
        else
        {
            fprintf(stderr, "B:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 21;
        }
    }
    if((ofd=open(cpm_options.outfile,flags,statBuf.st_mode))<0)
    {
        if(flags==(O_RDWR|O_CREAT|O_EXCL))
        {
            fprintf(stderr, "c:%d:%s:SUBOR EXISTUJE\n",errno,strerror(errno));
            return 23;
        }
        else if(flags==(O_RDWR))
        {
            fprintf(stderr, "o:%d:%s:SUBOR NEEXISTUJE\n",errno,strerror(errno));
            return 24;
        }
        else if(flags==(O_RDWR|O_APPEND))
        {
            fprintf(stderr, "a:%d:%s:SUBOR NEEXISTUJE\n",errno,strerror(errno));
            return 22;
        }
        else
        {
            fprintf(stderr, "B:%d:%s:INA CHYBA\n",errno,strerror(errno));
            return 21;
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
        if(unlink(cpm_options.infile)<0)
        {
            fprintf(stderr, "d:%d:%s:SUBOR NEBOL ZMAZANY\n",errno,strerror(errno));
            return 26;
        }
    }
    
    if(cpm_options.chmod)
    {
        if(cpm_options.chmod_mode!=0&&cpm_options.chmod_mode!=1&&cpm_options.chmod_mode!=2&&cpm_options.chmod_mode!=3&&cpm_options.chmod_mode!=4&&cpm_options.chmod_mode!=5&&cpm_options.chmod_mode!=6&&cpm_options.chmod_mode!=7&&
        cpm_options.chmod_mode!=8&&cpm_options.chmod_mode!=9&&cpm_options.chmod_mode!=10&&cpm_options.chmod_mode!=11&&cpm_options.chmod_mode!=12&&cpm_options.chmod_mode!=13&&cpm_options.chmod_mode!=14&&cpm_options.chmod_mode!=15&&
        cpm_options.chmod_mode!=16&&cpm_options.chmod_mode!=17&&cpm_options.chmod_mode!=18&&cpm_options.chmod_mode!=19&&cpm_options.chmod_mode!=20&&cpm_options.chmod_mode!=21&&cpm_options.chmod_mode!=22&&cpm_options.chmod_mode!=23&&
        cpm_options.chmod_mode!=24&&cpm_options.chmod_mode!=25&&cpm_options.chmod_mode!=26&&cpm_options.chmod_mode!=27&&cpm_options.chmod_mode!=28&&cpm_options.chmod_mode!=29&&cpm_options.chmod_mode!=30&&cpm_options.chmod_mode!=31&&
        cpm_options.chmod_mode!=32&&cpm_options.chmod_mode!=33&&cpm_options.chmod_mode!=34&&cpm_options.chmod_mode!=35&&cpm_options.chmod_mode!=36&&cpm_options.chmod_mode!=37&&cpm_options.chmod_mode!=38&&cpm_options.chmod_mode!=39&&
        cpm_options.chmod_mode!=40&&cpm_options.chmod_mode!=41&&cpm_options.chmod_mode!=42&&cpm_options.chmod_mode!=43&&cpm_options.chmod_mode!=44&&cpm_options.chmod_mode!=45&&cpm_options.chmod_mode!=46&&cpm_options.chmod_mode!=47&&
        cpm_options.chmod_mode!=48&&cpm_options.chmod_mode!=49&&cpm_options.chmod_mode!=50&&cpm_options.chmod_mode!=51&&cpm_options.chmod_mode!=52&&cpm_options.chmod_mode!=53&&cpm_options.chmod_mode!=54&&cpm_options.chmod_mode!=55&&
        cpm_options.chmod_mode!=56&&cpm_options.chmod_mode!=57&&cpm_options.chmod_mode!=58&&cpm_options.chmod_mode!=59&&cpm_options.chmod_mode!=60&&cpm_options.chmod_mode!=61&&cpm_options.chmod_mode!=62&&cpm_options.chmod_mode!=63&&
        cpm_options.chmod_mode!=64&&cpm_options.chmod_mode!=65&&cpm_options.chmod_mode!=66&&cpm_options.chmod_mode!=67&&cpm_options.chmod_mode!=68&&cpm_options.chmod_mode!=69&&cpm_options.chmod_mode!=70&&cpm_options.chmod_mode!=71&&
        cpm_options.chmod_mode!=72&&cpm_options.chmod_mode!=73&&cpm_options.chmod_mode!=74&&cpm_options.chmod_mode!=75&&cpm_options.chmod_mode!=76&&cpm_options.chmod_mode!=77&&cpm_options.chmod_mode!=78&&cpm_options.chmod_mode!=79&&
        cpm_options.chmod_mode!=80&&cpm_options.chmod_mode!=81&&cpm_options.chmod_mode!=82&&cpm_options.chmod_mode!=83&&cpm_options.chmod_mode!=84&&cpm_options.chmod_mode!=85&&cpm_options.chmod_mode!=86&&cpm_options.chmod_mode!=87&&
        cpm_options.chmod_mode!=88&&cpm_options.chmod_mode!=89&&cpm_options.chmod_mode!=90&&cpm_options.chmod_mode!=91&&cpm_options.chmod_mode!=92&&cpm_options.chmod_mode!=93&&cpm_options.chmod_mode!=94&&cpm_options.chmod_mode!=95&&
        cpm_options.chmod_mode!=96&&cpm_options.chmod_mode!=97&&cpm_options.chmod_mode!=98&&cpm_options.chmod_mode!=99&&cpm_options.chmod_mode!=100&&cpm_options.chmod_mode!=101&&cpm_options.chmod_mode!=102&&cpm_options.chmod_mode!=103&&
        cpm_options.chmod_mode!=104&&cpm_options.chmod_mode!=105&&cpm_options.chmod_mode!=106&&cpm_options.chmod_mode!=107&&cpm_options.chmod_mode!=108&&cpm_options.chmod_mode!=109&&cpm_options.chmod_mode!=110&&cpm_options.chmod_mode!=111&&
        cpm_options.chmod_mode!=112&&cpm_options.chmod_mode!=113&&cpm_options.chmod_mode!=114&&cpm_options.chmod_mode!=115&&cpm_options.chmod_mode!=116&&cpm_options.chmod_mode!=117&&cpm_options.chmod_mode!=118&&cpm_options.chmod_mode!=119&&
        cpm_options.chmod_mode!=120&&cpm_options.chmod_mode!=121&&cpm_options.chmod_mode!=122&&cpm_options.chmod_mode!=123&&cpm_options.chmod_mode!=124&&cpm_options.chmod_mode!=125&&cpm_options.chmod_mode!=126&&cpm_options.chmod_mode!=127&&
        cpm_options.chmod_mode!=128&&cpm_options.chmod_mode!=129&&cpm_options.chmod_mode!=130&&cpm_options.chmod_mode!=131&&cpm_options.chmod_mode!=132&&cpm_options.chmod_mode!=133&&cpm_options.chmod_mode!=134&&cpm_options.chmod_mode!=135&&
        cpm_options.chmod_mode!=136&&cpm_options.chmod_mode!=137&&cpm_options.chmod_mode!=138&&cpm_options.chmod_mode!=139&&cpm_options.chmod_mode!=140&&cpm_options.chmod_mode!=141&&cpm_options.chmod_mode!=142&&cpm_options.chmod_mode!=143&&
        cpm_options.chmod_mode!=144&&cpm_options.chmod_mode!=145&&cpm_options.chmod_mode!=146&&cpm_options.chmod_mode!=147&&cpm_options.chmod_mode!=148&&cpm_options.chmod_mode!=149&&cpm_options.chmod_mode!=150&&cpm_options.chmod_mode!=151&&
        cpm_options.chmod_mode!=152&&cpm_options.chmod_mode!=153&&cpm_options.chmod_mode!=154&&cpm_options.chmod_mode!=155&&cpm_options.chmod_mode!=156&&cpm_options.chmod_mode!=157&&cpm_options.chmod_mode!=158&&cpm_options.chmod_mode!=159&&
        cpm_options.chmod_mode!=160&&cpm_options.chmod_mode!=161&&cpm_options.chmod_mode!=162&&cpm_options.chmod_mode!=163&&cpm_options.chmod_mode!=164&&cpm_options.chmod_mode!=165&&cpm_options.chmod_mode!=166&&cpm_options.chmod_mode!=167&&
        cpm_options.chmod_mode!=168&&cpm_options.chmod_mode!=169&&cpm_options.chmod_mode!=170&&cpm_options.chmod_mode!=171&&cpm_options.chmod_mode!=172&&cpm_options.chmod_mode!=173&&cpm_options.chmod_mode!=174&&cpm_options.chmod_mode!=175&&
        cpm_options.chmod_mode!=176&&cpm_options.chmod_mode!=177&&cpm_options.chmod_mode!=178&&cpm_options.chmod_mode!=179&&cpm_options.chmod_mode!=180&&cpm_options.chmod_mode!=181&&cpm_options.chmod_mode!=182&&cpm_options.chmod_mode!=183&&
        cpm_options.chmod_mode!=184&&cpm_options.chmod_mode!=185&&cpm_options.chmod_mode!=186&&cpm_options.chmod_mode!=187&&cpm_options.chmod_mode!=188&&cpm_options.chmod_mode!=189&&cpm_options.chmod_mode!=190&&cpm_options.chmod_mode!=191&&
        cpm_options.chmod_mode!=192&&cpm_options.chmod_mode!=193&&cpm_options.chmod_mode!=194&&cpm_options.chmod_mode!=195&&cpm_options.chmod_mode!=196&&cpm_options.chmod_mode!=197&&cpm_options.chmod_mode!=198&&cpm_options.chmod_mode!=199&&
        cpm_options.chmod_mode!=200&&cpm_options.chmod_mode!=201&&cpm_options.chmod_mode!=202&&cpm_options.chmod_mode!=203&&cpm_options.chmod_mode!=204&&cpm_options.chmod_mode!=205&&cpm_options.chmod_mode!=206&&cpm_options.chmod_mode!=207&&
        cpm_options.chmod_mode!=208&&cpm_options.chmod_mode!=209&&cpm_options.chmod_mode!=210&&cpm_options.chmod_mode!=211&&cpm_options.chmod_mode!=212&&cpm_options.chmod_mode!=213&&cpm_options.chmod_mode!=214&&cpm_options.chmod_mode!=215&&
        cpm_options.chmod_mode!=216&&cpm_options.chmod_mode!=217&&cpm_options.chmod_mode!=218&&cpm_options.chmod_mode!=219&&cpm_options.chmod_mode!=220&&cpm_options.chmod_mode!=221&&cpm_options.chmod_mode!=222&&cpm_options.chmod_mode!=223&&
        cpm_options.chmod_mode!=224&&cpm_options.chmod_mode!=225&&cpm_options.chmod_mode!=226&&cpm_options.chmod_mode!=227&&cpm_options.chmod_mode!=228&&cpm_options.chmod_mode!=229&&cpm_options.chmod_mode!=230&&cpm_options.chmod_mode!=231&&
        cpm_options.chmod_mode!=232&&cpm_options.chmod_mode!=233&&cpm_options.chmod_mode!=234&&cpm_options.chmod_mode!=235&&cpm_options.chmod_mode!=236&&cpm_options.chmod_mode!=237&&cpm_options.chmod_mode!=238&&cpm_options.chmod_mode!=239&&
        cpm_options.chmod_mode!=240&&cpm_options.chmod_mode!=241&&cpm_options.chmod_mode!=242&&cpm_options.chmod_mode!=243&&cpm_options.chmod_mode!=244&&cpm_options.chmod_mode!=245&&cpm_options.chmod_mode!=246&&cpm_options.chmod_mode!=247&&
        cpm_options.chmod_mode!=248&&cpm_options.chmod_mode!=249&&cpm_options.chmod_mode!=250&&cpm_options.chmod_mode!=251&&cpm_options.chmod_mode!=252&&cpm_options.chmod_mode!=253&&cpm_options.chmod_mode!=254&&cpm_options.chmod_mode!=255&&
        cpm_options.chmod_mode!=256&&cpm_options.chmod_mode!=257&&cpm_options.chmod_mode!=258&&cpm_options.chmod_mode!=259&&cpm_options.chmod_mode!=260&&cpm_options.chmod_mode!=261&&cpm_options.chmod_mode!=262&&cpm_options.chmod_mode!=263&&
        cpm_options.chmod_mode!=264&&cpm_options.chmod_mode!=265&&cpm_options.chmod_mode!=266&&cpm_options.chmod_mode!=267&&cpm_options.chmod_mode!=268&&cpm_options.chmod_mode!=269&&cpm_options.chmod_mode!=270&&cpm_options.chmod_mode!=271&&
        cpm_options.chmod_mode!=272&&cpm_options.chmod_mode!=273&&cpm_options.chmod_mode!=274&&cpm_options.chmod_mode!=275&&cpm_options.chmod_mode!=276&&cpm_options.chmod_mode!=277&&cpm_options.chmod_mode!=278&&cpm_options.chmod_mode!=279&&
        cpm_options.chmod_mode!=280&&cpm_options.chmod_mode!=281&&cpm_options.chmod_mode!=282&&cpm_options.chmod_mode!=283&&cpm_options.chmod_mode!=284&&cpm_options.chmod_mode!=285&&cpm_options.chmod_mode!=286&&cpm_options.chmod_mode!=287&&
        cpm_options.chmod_mode!=288&&cpm_options.chmod_mode!=289&&cpm_options.chmod_mode!=290&&cpm_options.chmod_mode!=291&&cpm_options.chmod_mode!=292&&cpm_options.chmod_mode!=293&&cpm_options.chmod_mode!=294&&cpm_options.chmod_mode!=295&&
        cpm_options.chmod_mode!=296&&cpm_options.chmod_mode!=297&&cpm_options.chmod_mode!=298&&cpm_options.chmod_mode!=299&&cpm_options.chmod_mode!=300&&cpm_options.chmod_mode!=301&&cpm_options.chmod_mode!=302&&cpm_options.chmod_mode!=303&&
        cpm_options.chmod_mode!=304&&cpm_options.chmod_mode!=305&&cpm_options.chmod_mode!=306&&cpm_options.chmod_mode!=307&&cpm_options.chmod_mode!=308&&cpm_options.chmod_mode!=309&&cpm_options.chmod_mode!=310&&cpm_options.chmod_mode!=311&&
        cpm_options.chmod_mode!=312&&cpm_options.chmod_mode!=313&&cpm_options.chmod_mode!=314&&cpm_options.chmod_mode!=315&&cpm_options.chmod_mode!=316&&cpm_options.chmod_mode!=317&&cpm_options.chmod_mode!=318&&cpm_options.chmod_mode!=319&&
        cpm_options.chmod_mode!=320&&cpm_options.chmod_mode!=321&&cpm_options.chmod_mode!=322&&cpm_options.chmod_mode!=323&&cpm_options.chmod_mode!=324&&cpm_options.chmod_mode!=325&&cpm_options.chmod_mode!=326&&cpm_options.chmod_mode!=327&&
        cpm_options.chmod_mode!=328&&cpm_options.chmod_mode!=329&&cpm_options.chmod_mode!=330&&cpm_options.chmod_mode!=331&&cpm_options.chmod_mode!=332&&cpm_options.chmod_mode!=333&&cpm_options.chmod_mode!=334&&cpm_options.chmod_mode!=335&&
        cpm_options.chmod_mode!=336&&cpm_options.chmod_mode!=337&&cpm_options.chmod_mode!=338&&cpm_options.chmod_mode!=339&&cpm_options.chmod_mode!=340&&cpm_options.chmod_mode!=341&&cpm_options.chmod_mode!=342&&cpm_options.chmod_mode!=343&&
        cpm_options.chmod_mode!=344&&cpm_options.chmod_mode!=345&&cpm_options.chmod_mode!=346&&cpm_options.chmod_mode!=347&&cpm_options.chmod_mode!=348&&cpm_options.chmod_mode!=349&&cpm_options.chmod_mode!=350&&cpm_options.chmod_mode!=351&&
        cpm_options.chmod_mode!=352&&cpm_options.chmod_mode!=353&&cpm_options.chmod_mode!=354&&cpm_options.chmod_mode!=355&&cpm_options.chmod_mode!=356&&cpm_options.chmod_mode!=357&&cpm_options.chmod_mode!=358&&cpm_options.chmod_mode!=359&&
        cpm_options.chmod_mode!=360&&cpm_options.chmod_mode!=361&&cpm_options.chmod_mode!=362&&cpm_options.chmod_mode!=363&&cpm_options.chmod_mode!=364&&cpm_options.chmod_mode!=365&&cpm_options.chmod_mode!=366&&cpm_options.chmod_mode!=367&&
        cpm_options.chmod_mode!=368&&cpm_options.chmod_mode!=369&&cpm_options.chmod_mode!=370&&cpm_options.chmod_mode!=371&&cpm_options.chmod_mode!=372&&cpm_options.chmod_mode!=373&&cpm_options.chmod_mode!=374&&cpm_options.chmod_mode!=375&&
        cpm_options.chmod_mode!=376&&cpm_options.chmod_mode!=377&&cpm_options.chmod_mode!=378&&cpm_options.chmod_mode!=379&&cpm_options.chmod_mode!=380&&cpm_options.chmod_mode!=381&&cpm_options.chmod_mode!=382&&cpm_options.chmod_mode!=383&&
        cpm_options.chmod_mode!=384&&cpm_options.chmod_mode!=385&&cpm_options.chmod_mode!=386&&cpm_options.chmod_mode!=387&&cpm_options.chmod_mode!=388&&cpm_options.chmod_mode!=389&&cpm_options.chmod_mode!=390&&cpm_options.chmod_mode!=391&&
        cpm_options.chmod_mode!=392&&cpm_options.chmod_mode!=393&&cpm_options.chmod_mode!=394&&cpm_options.chmod_mode!=395&&cpm_options.chmod_mode!=396&&cpm_options.chmod_mode!=397&&cpm_options.chmod_mode!=398&&cpm_options.chmod_mode!=399&&
        cpm_options.chmod_mode!=400&&cpm_options.chmod_mode!=401&&cpm_options.chmod_mode!=402&&cpm_options.chmod_mode!=403&&cpm_options.chmod_mode!=404&&cpm_options.chmod_mode!=405&&cpm_options.chmod_mode!=406&&cpm_options.chmod_mode!=407&&
        cpm_options.chmod_mode!=408&&cpm_options.chmod_mode!=409&&cpm_options.chmod_mode!=410&&cpm_options.chmod_mode!=411&&cpm_options.chmod_mode!=412&&cpm_options.chmod_mode!=413&&cpm_options.chmod_mode!=414&&cpm_options.chmod_mode!=415&&
        cpm_options.chmod_mode!=416&&cpm_options.chmod_mode!=417&&cpm_options.chmod_mode!=418&&cpm_options.chmod_mode!=419&&cpm_options.chmod_mode!=420&&cpm_options.chmod_mode!=421&&cpm_options.chmod_mode!=422&&cpm_options.chmod_mode!=423&&
        cpm_options.chmod_mode!=424&&cpm_options.chmod_mode!=425&&cpm_options.chmod_mode!=426&&cpm_options.chmod_mode!=427&&cpm_options.chmod_mode!=428&&cpm_options.chmod_mode!=429&&cpm_options.chmod_mode!=430&&cpm_options.chmod_mode!=431&&
        cpm_options.chmod_mode!=432&&cpm_options.chmod_mode!=433&&cpm_options.chmod_mode!=434&&cpm_options.chmod_mode!=435&&cpm_options.chmod_mode!=436&&cpm_options.chmod_mode!=437&&cpm_options.chmod_mode!=438&&cpm_options.chmod_mode!=439&&
        cpm_options.chmod_mode!=440&&cpm_options.chmod_mode!=441&&cpm_options.chmod_mode!=442&&cpm_options.chmod_mode!=443&&cpm_options.chmod_mode!=444&&cpm_options.chmod_mode!=445&&cpm_options.chmod_mode!=446&&cpm_options.chmod_mode!=447&&
        cpm_options.chmod_mode!=448&&cpm_options.chmod_mode!=449&&cpm_options.chmod_mode!=450&&cpm_options.chmod_mode!=451&&cpm_options.chmod_mode!=452&&cpm_options.chmod_mode!=453&&cpm_options.chmod_mode!=454&&cpm_options.chmod_mode!=455&&
        cpm_options.chmod_mode!=456&&cpm_options.chmod_mode!=457&&cpm_options.chmod_mode!=458&&cpm_options.chmod_mode!=459&&cpm_options.chmod_mode!=460&&cpm_options.chmod_mode!=461&&cpm_options.chmod_mode!=462&&cpm_options.chmod_mode!=463&&
        cpm_options.chmod_mode!=464&&cpm_options.chmod_mode!=465&&cpm_options.chmod_mode!=466&&cpm_options.chmod_mode!=467&&cpm_options.chmod_mode!=468&&cpm_options.chmod_mode!=469&&cpm_options.chmod_mode!=470&&cpm_options.chmod_mode!=471&&
        cpm_options.chmod_mode!=472&&cpm_options.chmod_mode!=473&&cpm_options.chmod_mode!=474&&cpm_options.chmod_mode!=475&&cpm_options.chmod_mode!=476&&cpm_options.chmod_mode!=477&&cpm_options.chmod_mode!=478&&cpm_options.chmod_mode!=479&&
        cpm_options.chmod_mode!=480&&cpm_options.chmod_mode!=481&&cpm_options.chmod_mode!=482&&cpm_options.chmod_mode!=483&&cpm_options.chmod_mode!=484&&cpm_options.chmod_mode!=485&&cpm_options.chmod_mode!=486&&cpm_options.chmod_mode!=487&&
        cpm_options.chmod_mode!=488&&cpm_options.chmod_mode!=489&&cpm_options.chmod_mode!=490&&cpm_options.chmod_mode!=491&&cpm_options.chmod_mode!=492&&cpm_options.chmod_mode!=493&&cpm_options.chmod_mode!=494&&cpm_options.chmod_mode!=495&&
        cpm_options.chmod_mode!=496&&cpm_options.chmod_mode!=497&&cpm_options.chmod_mode!=498&&cpm_options.chmod_mode!=499&&cpm_options.chmod_mode!=500&&cpm_options.chmod_mode!=501&&cpm_options.chmod_mode!=502&&cpm_options.chmod_mode!=503&&
        cpm_options.chmod_mode!=504&&cpm_options.chmod_mode!=505&&cpm_options.chmod_mode!=506&&cpm_options.chmod_mode!=507&&cpm_options.chmod_mode!=508&&cpm_options.chmod_mode!=509&&cpm_options.chmod_mode!=510&&cpm_options.chmod_mode!=511)
        {
            fprintf(stderr, "m:%d:%s:ZLE PRAVA\n",errno,strerror(errno));
            return 34;
        }
        if(chmod(cpm_options.outfile,cpm_options.chmod_mode)<0)
        {
            fprintf(stderr, "m:%d:%s:INA CHYBA\n",errno,strerror(errno));
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
                fprintf(stderr, "D:%d:%s:VSTUPNY SUBOR NIE JE ADRESAR\n",errno,strerror(errno));
                return 28;
            }
            else
            {
                fprintf(stderr, "D:%d:%s:INA CHYBA\n",errno,strerror(errno));
                return 28;
            }
        }
        if((ofd=open(cpm_options.outfile,flags,statBuf.st_mode))<0)
        {
            fprintf(stderr, "D:%d:%s:VYSTUPNY SUBOR - CHYBA\n",errno,strerror(errno));
            return 28;
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
    fprintf(stderr, "%c:%d\n", c, errno); 
    fprintf(stderr, "%c:%s\n", c, strerror(errno));
    fprintf(stderr, "%c:%s\n", c, msg);
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


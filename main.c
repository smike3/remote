/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int command_mvp(char cc[20480],int br)
{
 DIR *dir;
 int i,fd;
 char s[20480];
 struct dirent *fname;
 for(i=0;i<br-4;i++) s[i]=cc[i+4];
 if(!strncmp(cc,"1dir",4)){
 printf("\n%s\n",s);
 cc[4]='\0';
     dir=opendir(s);
     if (!dir) {
             perror("diropen");
             return 0;
         };
  //   printf("<%s>",cc);
     //sprintf(cc,"directy");
     while((fname=readdir(dir))!=NULL)

             if((fname->d_name[0]!='.')&&(fname->d_type==DT_REG)) {//printf("|%s|",fname->d_name);
             sprintf(cc,"%s!%s",cc,fname->d_name);
             };


         closedir(dir);
//for(i=0;i<br;i++) cc[i]=cc[i+1];

 }
 else if(!strncmp(cc,"1mvp",4))
    {
	
        switch(fork())
	{
	case -1:
	    perror("Fork faild");
	    exit(1);
	case 0:
	    execl("/usr/bin/mpv", "/usr/bin/mpv","--input-ipc-server=/tmp/clmvp","--vo","opengl",s,NULL);
	    
	    exit(0);
	}
	fd=open("/tmp/clmpv",O_RDWR);
//    write(fd,"{ \"command\": [ \"seek\", \"60\"] }",31);
	write(fd,"{ \"command\": [\"set_property_string\", \"pause\",\"yes\"] }",53);
    printf("\n{ \"command\": [ \"seek\", \"60\"] }\n");
    close(fd);
    }

return strlen(cc);
}

int command_aud(char cc[20480],int br)
{
    pid_t p;
    int cpipe[2],rbb,i,rb;
    pipe(cpipe);
//    int rb[2];
    char bb[256]="",c1[4],c2[2048],*c;
    for(i=0;i<br;i++) cc[i]=cc[i+1];
    switch(fork())
	{
	case -1:
	    perror("Fork faild");
	    exit(1);
	case 0:
	    close(cpipe[0]);
	    dup2(cpipe[1],1);
	    br--;
	    if(cc[br]=='\n') cc[br]='\0';
	    if(!strncmp(cc,"current-song",12)) execl("/usr/bin/audtool","/usr/bin/audtool","get-volume",cc,NULL);
	    if(!strncmp(cc,"set-volume",10)){
		c=strchr(cc,' ');
		c++;
	     execl("/usr/bin/audtool","/usr/bin/audtool","set-volume",c,"get-volume","current-song",NULL);
	     }
	    else execl("/usr/bin/audtool","/usr/bin/audtool",cc,"get-volume","current-song",NULL);
	    printf("\n[%s]\n",cc);
	    close(cpipe[1]);
	    exit(0);
	}
/*    close(cpipe[1]);
    rb[0]=read(cpipe[0],cc,256);
    cc[--rb[0]]='\0';
    rb[1]=atoi(cc);
    rb[0]=read(cpipe[0],cc,256);
    close(cpipe[0]);
    cc[--rb[0]]='\0';
    printf("{%s}(%d)[%d]",cc,rb[0],rb[1]);*/
    close(cpipe[1]);
    rb=read(cpipe[0],c1,5);
    c1[--rb]='\0';
    rb=read(cpipe[0],c2,2048);
    close(cpipe[0]);
    c2[--rb]='\0';
    if(rb<0)
    {
    sprintf(cc,"0[%03d]none",atoi(c1));
    rb=4;
    }
    else sprintf(cc,"0[%03d]%s",atoi(c1),c2);
    printf("{%s}(%d}",cc,rb);
    return rb+6;
}

int main ()
{
    int sc, ls, c,i;       // дескрипторы сокетов
    struct sockaddr_in addr; // структура с адресом
    char buf[20480];       // буфур для приема
    int bread;           // кол-во принятых байт
    ls=socket(AF_INET,SOCK_STREAM,0); // создаем сокет для входных подключений
    if(ls<0)
	{
    	 perror("socket");
    	 exit(1);
	}
    // Указываем параметры сервера
    addr.sin_family=AF_INET;
    addr.sin_port=htons(3379);
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr.s_addr=inet_addr("10.6.133.66");
    if(bind(ls,(struct sockaddr *)&addr,sizeof(addr))<0) // связываемся с сетевым устройство. Сейчас это устройство lo - "петля", которое используется для отладки сетевых приложений
	{
	 perror("bind");
	 exit(2);
	}
    listen(ls,1); // очередь входных подключений
    while(1)
	{
         sc=accept(ls,NULL,NULL); // принимаем входные подключение и создаем отделный сокет для каждого нового подключившегося клиента
         if(sc<0)
    	    {
             perror("Прием входящих подключений");
             exit(3);
    	    }
	 switch(fork())
	    {
	     case -1:
	        perror("fork");
	        break;
	     case 0:
		close(ls);
		while(1)
		    {
         	     printf("Ожидаем сообщение...\n");
         	     buf[0]='\0';
                     bread=recv(sc,buf,2048,0); // принимаем сообщение от клиента
	             if(bread<=0) break;
    	    	     printf("Получено %d bytes\tСообщение: %s\n",bread,buf);
        	     printf("Отправляю принятое сообщение клиенту\n\n\n");
        	     printf("!%c!\n",buf[0]);
        	     switch(buf[0])
        	     {
//        	     case '0': c=command_aud(buf,bread,c);
        	     case '0': c=command_aud(buf,bread);
        	        break;
//        	     case '1': c=command_mvp(buf,bread,c);
        	     case '1': c=command_mvp(buf,bread);
        	        break;
        	     default: buf[0]='\0';

        	     }


        //	     read(fileno(stdout),buf,2048);
			//if(rb==-1) send(sc,buf,c,0);
    		     printf("C: %d<%s>\n",c,buf);
	    	     if(c>4) send(sc,buf,c,0); // отправляем принятое сообщение клиенту
	    	     else send(sc,"0[000]none",10,0);
    		    }
    	        close(sc); // закрываем сокет
    	        exit(0);
	     default:
		close(sc);
	    }   
	}
    return 0;
}

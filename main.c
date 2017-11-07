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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int command(char cc[2048],int br,int rb)
{
    pid_t p;
    int cpipe[2],rbb;
    pipe(cpipe);
//    int rb[2];
    char bb[256]="",c1[4],c2[2048],*c;
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
    sprintf(cc,"[%03d]%s",atoi(c1),c2);
    printf("{%s}(%d)",cc,rb);
     return rb+5;
}

int main ()
{
    int sc, ls, c;       // дескрипторы сокетов
    struct sockaddr_in addr; // структура с адресом
    char buf[2048];       // буфур для приема
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
        	     c=command(buf,bread,c);
        //	     read(fileno(stdout),buf,2048);
			//if(rb==-1) send(sc,buf,c,0);
    		     printf("C: %d[%s]\n",c,buf);
	    	     if(c>4) send(sc,buf,c,0); // отправляем принятое сообщение клиенту
	    	     else send(sc,"[000]none",9,0);
    		    }
    	        close(sc); // закрываем сокет
    	        exit(0);
	     default:
		close(sc);
	    }   
	}
    return 0;
}

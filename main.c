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
int command(char cc[2048],int br)
{
    pid_t p;
    int cpipe[2];
    pipe(cpipe);
    int rb;
    char bb[256]="";
//    printf("(%c)",cc[strlen(cc)-1]);
//    int sl=strlen(cc)-1;
//p=fork();
//if(strncmp(cc,"current-song",12))
switch(fork())
{case -1:
    perror("Fork faild");
    exit(1);
case 0:
    close(cpipe[0]);
    dup2(cpipe[1],1);
    br--;
    if(cc[br]=='\n') cc[br]='\0';
//    execl("/usr/bin/audtool","/usr/bin/audtool",cc,NULL);
    if(strncmp(cc,"current-song",12)) execl("/usr/bin/audtool","/usr/bin/audtool",cc,"current-song",NULL);
    else execl("/usr/bin/audtool","/usr/bin/audtool",cc,NULL);
    printf("\n[%s]\n",cc);
    close(cpipe[1]);
    exit(0);
}
close(cpipe[1]);
rb=read(cpipe[0],cc,256);
close(cpipe[0]);
cc[--rb]='\0';
printf("{%s}(%d)",cc,rb);
/*br--;
if(cc[br]=='\n') cc[br]='\0';
    if(p==0)
	{
	    execl("/usr/bin/audtool","/usr/bin/audtool",cc,NULL);
	    
	}
    printf("\n[%s]\n",cc);*/
/*  int c, cc;
  read_gp4 ();
  printf
    ("Version: %s\nTitle: %s\nSubtitle: %s\nInterpret: %s\nAlbum: %s\nAuthor: %s\nCopyright: %s\nAuthor of the tab: %s\nInstructional: %s\nNotice: ",
     ff.version, ff.title, ff.subtitle, ff.interpret, ff.album, ff.author,
     ff.copyright, ff.authort, ff.instructional);
  for (c = 0; c < nnotice; c++)
    printf ("%s\n", ff.notice[c]);
  printf ("TripletFeel: %d\nLyrics track: %d\nLyrics\n", (int) ff.tripletfeel,
	  ff.lyricstrack);
  for (c = 0; c < 5; c++)
    printf ("%d| %d %d '%s'\n", c, m, sll, ff.lyrics[c]);
  printf ("Tempo: %d\nKey: ", ff.tempo);
  switch (ff.key)
    {
    case 0:
      printf ("C\n");
      break;
    case 1:
      printf ("G\n");
      break;
    case 2:
      printf ("D\n");
      break;
    case -1:
      printf ("F\n");
      break;
    }
  printf ("Octava: %d\nMidi table\n", ff.octava);
  for (c = 0; c < 4; c++)
    for (cc = 0; cc < 16; cc++)
      printf
	("[%d,%d] i: %d v: %d b: %d c: %d r: %d p: %d t: %d 1: %d 2: %d\n", c,
	 cc, ff.mchannels[c][cc].instrument, (int) ff.mchannels[c][cc].volume,
	 (int) ff.mchannels[c][cc].balance, (int) ff.mchannels[c][cc].chorus,
	 (int) ff.mchannels[c][cc].reverb, (int) ff.mchannels[c][cc].phaser,
	 (int) ff.mchannels[c][cc].tremolo, (int) ff.mchannels[c][cc].blank1,
	 (int) ff.mchannels[c][cc].blank2);
  //free(ff.notice);
  printf ("Number of Measures: %d\nNumber of Tracks: %d\n", ff.nmeasures,
	  ff.ntracks);*/

 return rb;
}


int
main ()
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
        	     c=command(buf,bread);
        //	     read(fileno(stdout),buf,2048);
			//if(rb==-1) send(sc,buf,c,0);
    		printf("C: %d[%s]\n",c,buf);
        	     send(sc,buf,c,0); // отправляем принятое сообщение клиенту
        	    
    		    }
    	        close(sc); // закрываем сокет
    	        exit(0);
	     default:
		close(sc);
	    }   
	}
    return 0;
}

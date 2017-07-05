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


int
main ()
{
    
    pid_t p=fork();
    if(p==0)
	{
	    execl("/usr/bin/audtool","/usr/bin/audtool","playback-stop",NULL);
	    
	}
    printf("\nplayback-stop\n");
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
  return 0;
}

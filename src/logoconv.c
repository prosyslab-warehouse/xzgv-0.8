/* xzgv v0.2 - picture viewer for X, with file selector.
 * Copyright (C) 1999 Russell Marks. See main.c for license details.
 *
 * logoconv.c - convert logo PPM to C containing unsigned char array
 * 		with the RGB data. This is a bit unpleasant, but
 * 		#include-ing an XPM this size slows down startup
 * 		noticeably.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/* we take an extremely generous outlook - anything other than a decimal
 * digit is considered whitespace.
 * and as per p[bgp]m(5), comments can start anywhere.
 */
int read_next_number(FILE *in)
{
int c,num,in_num,gotnum;

num=0;
in_num=gotnum=0;

do
  {
  if((c=fgetc(in))=='#')
    while((c=fgetc(in))!='\n' && c!=EOF) ;	/* ditch line */
  else
    {
    if(c==EOF)
      return(0);
    if(isdigit(c))
      num=10*num+c-49+(in_num=1);
    else
      gotnum=in_num;
    }
  }
while(!gotnum);  

return(num);
}


int main(void)
{
unsigned char buf[128];
int w,h,x,y,c;

fgets(buf,sizeof(buf),stdin);
if(strcmp(buf,"P6\n")!=0)
  fprintf(stderr,"logoconv: stdin not a raw PPM file.\n"),exit(1);

w=read_next_number(stdin);
h=read_next_number(stdin);
if(w==0 || h==0)
  fprintf(stderr,"logoconv: bad width/height.\n"),exit(1);

if(read_next_number(stdin)!=255)
  fprintf(stderr,"logoconv: PPM's maxval must be 255.\n"),exit(1);

printf("/* auto-generated by logoconv, do not edit! */\n\n");
printf("int logo_w=%d,logo_h=%d;\n\n",w,h);
printf("unsigned char logo_data[]=\n{\n");

for(y=0;y<h;y++)
  {
  for(x=0;x<w*3;x++)
    {
    c=getchar();
    printf("%d,",c);
    }
  putchar('\n');
  }

printf("};\n");
exit(0);
}

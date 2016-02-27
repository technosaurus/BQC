//gcc $CFLAGS -nostdlib -nostartfiles -o get get.c $LDFLAGS
//Copyright Brad Conroy - 2015
//This file is licensed separately from bqc.h, please contact the author for licensing information
#include "bqc.h"

#define strcpyALL(buf, offset, ...) do{ \
   char *bp=(char*)(buf+offset); \
   const char *s, *a[] = { __VA_ARGS__,NULL}, **ss=a; \
   while((s=*ss++)) \
      while((*s)&&(++offset<(int)sizeof(buf))) \
         *bp++=*s++; \
   if (offset!=sizeof(buf))*bp=0; \
}while(0)

static char buf[4096];

static inline uint32_t query_ip(const char *host){
   const char hdr[]={0,0,1,0,0,1,0,0,0,0,0,0,/*host*/ 0,1,0,1},*hp=hdr;
   struct sockaddr_in dest = {
      .sin_family=AF_INET, .sin_port=htons(53), .sin_addr.s_addr=0x04020204
   }, *dp = &dest;
   size_t offset=0, i, j=0, sz=sizeof(dest);
   uint32_t ans, ip=0;
   int sfd=socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
   for(i=12;i;i--) buf[offset++]=*hp++; //copy header
   do{ /* convert www.example.com to 3www7example3com */
      if(host[i]=='.' || !host[i]){ //could use strchrnul() here instead
         buf[offset++] = i-j;
         for(;j<i;j++) buf[offset++]=host[j];
         ++j;
      }
   }while(host[i++]);
   buf[offset++]='\0';
   if (!(offset)&1) buf[offset++]='\0';
   for(i=4;i;i--) buf[offset++]=*hp++; //copy header
   if(( connect(sfd, dp, sz)) < 0 ) goto IPV4END;
   if((write(sfd, buf, offset))<0) goto IPV4END;
   if((read(sfd,buf,sizeof(buf)))<0) goto IPV4END;
   for(i=0;i<buf[7];i++){ //[7] holds num of answers([6] does too but >256???)
      while(buf[offset]) offset++; //skip names
      ans=buf[offset+1]; //[1] holds the answer type ([0] does too, but >256???)
      offset += 10;
      if(ans == 1){ uint32_t j=4; // ipv4 address
         char *ipp=(char *)&ip;
         while(j--) *ipp++=buf[offset++];
         goto IPV4END;
      }else while(buf[offset]) offset++; //skip (alias) names
   }
IPV4END:
   close(sfd);
   return ip;
}

static inline void get(const char *host, const char *path){
   struct sockaddr_in dest = {
      .sin_family=AF_INET,.sin_port=htons(80),.sin_addr.s_addr=query_ip(host)
   };
   int len=0, sz=sizeof(dest), s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if(( connect(s, &dest,sz)) != 0 ) goto GETEND;
   strcpyALL(buf,len,"GET ",path," HTTP/1.0\nHost: ",host,"\n\n");
   if((write(s, buf, len))<0) goto GETEND;
   len=read(s,buf,sizeof(buf));
   if (len<0) goto GETEND;
   else{
      char *bp=strstr(buf,"\r\n\r\n");
      if (bp==NULL) goto GETEND;
      bp+=4;
      len-=(bp-buf);
      write(1,bp,len);
      while ((len=read(s,buf,sizeof(buf)))>0)
         write(1,buf,len);
   }
GETEND:
   close(s);
}

int main(int argc, char **argv){
  if (argc<3){
    write(2,"usage:\n\tget host /path/file\n",28);
    return 1;
  }
   get(argv[1],argv[2]);
   return 0;
}

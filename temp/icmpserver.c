
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

#define IN_BUF_SIZE   1024
    struct iphdr *ip,*iph;
#define OUT_BUF_SIZE  64

struct sockaddr_in source,dest;
unsigned short checksum(unsigned short *ptr, int nbytes)
{
    unsigned long sum;
    unsigned short oddbyte, rs;

    sum = 0;
    while(nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if(nbytes == 1) {
        oddbyte = 0;
        *((unsigned char *) &oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum  = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    rs = ~sum;
    return rs;
}

void PrintData (const u_char * data , int Size)
{
	FILE *logfile=stdout;
    int i , j;
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(logfile , "         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(logfile , "%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else fprintf(logfile , "."); //otherwise print a dot
            }
            fprintf(logfile , "\n");
        } 
         
        if(i%16==0) fprintf(logfile , "   ");
            fprintf(logfile , " %02X",(unsigned int)data[i]);
                 
        if( i==Size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) 
            {
              fprintf(logfile , "   "); //extra spaces
            }
             
            fprintf(logfile , "         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) 
                {
                  fprintf(logfile , "%c",(unsigned char)data[j]);
                }
                else
                {
                  fprintf(logfile , ".");
                }
            }
             
            fprintf(logfile ,  "\n" );
        }
    }
}
void print_ip_header(const u_char * Buffer, int Size)
{
	iph=ip;
	///FILE *logfile=fopen("log.txt","w");
    FILE *logfile=stdout;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    fprintf(logfile , "   |-IP Version        : %d\n",(unsigned int)ip->version);
    fprintf(logfile , "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(logfile , "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    fprintf(logfile , "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    fprintf(logfile , "   |-Identification    : %d\n",ntohs(iph->id));
    //fprintf(logfile , "   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
    //fprintf(logfile , "   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
    //fprintf(logfile , "   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    fprintf(logfile , "   |-TTL      : %d\n",(unsigned int)iph->ttl);
    fprintf(logfile , "   |-Protocol : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(iph->check));
    fprintf(logfile , "   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    fprintf(logfile , "   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
}
void print_icmp_packet(const u_char * Buffer , int Size)
{
	iph=ip;
	FILE *logfile=stdout;
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
    fprintf(logfile , "\n\n***********************ICMP Packet*************************\n"); 
     
    print_ip_header(Buffer , Size);
             
    fprintf(logfile , "\n");
         
    fprintf(logfile , "ICMP Header\n");
    fprintf(logfile , "   |-Type : %d",(unsigned int)(icmph->type));
             
    if((unsigned int)(icmph->type) == 11)
    {
        fprintf(logfile , "  (TTL Expired)\n");
    }
    else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
    {
        fprintf(logfile , "  (ICMP Echo Reply)\n");
    }
     
    fprintf(logfile , "   |-Code : %d\n",(unsigned int)(icmph->code));
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(icmph->checksum));
    //fprintf(logfile , "   |-ID       : %d\n",ntohs(icmph->id));
    //fprintf(logfile , "   |-Sequence : %d\n",ntohs(icmph->sequence));
    fprintf(logfile , "\n");
 
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer + iphdrlen , sizeof icmph);
         
    fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , (Size - header_size) );
     
    fprintf(logfile , "\n###########################################################");
}

int main(int argc, char **argv)
{
    int sockfd;
    int flags;
    char in_buf[IN_BUF_SIZE];
    char out_buf[OUT_BUF_SIZE]="fuck";
    unsigned int out_size;
    int nbytes;
    struct icmphdr *icmp;
    char *data;
    struct sockaddr_in addr;
    printf("DEV - master\n");
    flags = fcntl(0, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
    sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    printf("running...\n");

    while(1) {
        // read data from socket
        memset(in_buf, 0x00, IN_BUF_SIZE);
        nbytes = read(sockfd, in_buf, IN_BUF_SIZE - 1);
        printf("HERE %d %s",nbytes,in_buf);
        int i=0;

        if (nbytes > 0) {
        	for(i=0;i<nbytes;i++)
        		printf("%d ",in_buf[i] );
        	printf("------------------------------------------------------------\n");
            // get ip and icmp header and data part
            ip = (struct iphdr *) in_buf;

            print_ip_header(in_buf,nbytes);
            print_icmp_packet(in_buf,nbytes);
            if (nbytes > sizeof(struct iphdr)) {
                nbytes -= sizeof(struct iphdr);
                icmp = (struct icmphdr *) (ip + 1);
                printf("TYPE = %x",icmp->checksum);
                if (nbytes > sizeof(struct icmphdr)) {
                    nbytes -= sizeof(struct icmphdr);
                    data = (char *) (icmp + 1);
                    data[nbytes] = '\0';
                    printf("%s", data);
                    fflush(stdout);
                }
                
                // reuse headers
                icmp->type = 0;//icmp replay
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = ip->saddr;
                //print("%s",ip->saddr);
        		
                // read data from stdin
                /*nbytes = read(0, out_buf, OUT_BUF_SIZE);
                if (nbytes > -1) {
                    memcpy((char *) (icmp + 1), out_buf, nbytes);
                    out_size = nbytes;
                } else {
                    out_size = 0;
                }*/
                nbytes=4;
                memcpy((char *) (icmp + 1), out_buf, nbytes);
                out_size = nbytes;
                
                icmp->checksum = 0x00;
                icmp->checksum = checksum((unsigned short *) icmp, sizeof(struct icmphdr) + out_size);

                // send reply
                nbytes = sendto(sockfd, icmp, sizeof(struct icmphdr) + out_size, 0, (struct sockaddr *) &addr, sizeof(addr));
                if (nbytes == -1) {
                    perror("sendto");
                    return -1;
                }   
                exit(0);
            }
        }
    }

    return 0;
}

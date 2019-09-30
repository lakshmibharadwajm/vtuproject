#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Attack Services
#define ATTACK 1
#define DEFENCE 2
#define CRAFT 3
#define LISTEN 4

//Modules
#define DROP_SYN 1
#define BAN_IP 2

#ifndef INSTALL_DIR
#define INSTALL_DIR "/home/Awcator/FinalyearProj/native/"
#endif
#define RELEASE_VERSION "Thor 1.0\n"

unsigned short NUMBER_OF_THREADS=5;
unsigned short SERVICE_TYPE=0;
unsigned short MODULE_TYPE=0;
unsigned short VERBROSE_LEVEL=1;

void printUsageAndExit(unsigned short EXIT_STATUS);
void showInitialSettings(void);
void performJob(void);

void printUsageAndExit(unsigned short EXIT_STATUS) {
	char dest[40];
	strcpy(dest,INSTALL_DIR);
	strcat(dest,"./usage.sh");
	system(dest);exit(EXIT_STATUS);
}

void showInitialSettings() {
	printf("Number of threads Accepted \t[ %d ]\n",NUMBER_OF_THREADS);
	printf("VERBROSE_LEVEL \t\t\t[ %d ]\n",VERBROSE_LEVEL);
	if(SERVICE_TYPE==ATTACK) printf("Service Type \t\t\t[ ATTACK ]\n");
	else if(SERVICE_TYPE==DEFENCE) {
		printf("Service Type \t\t\t[ DEFENCE ]\n");
		if(MODULE_TYPE==DROP_SYN)printf("MODULE_TYPE  \t\t\t[ DROP_SYN ]\n");
		else if(MODULE_TYPE==BAN_IP)printf("MODULE_TYPE  \t\t\t[ BAN_IP ]\n");
		else printf("MODULE_TYPE \t\t\t[ UNKOWN ]\n");
	} else if(SERVICE_TYPE==CRAFT) printf("Service Type \t\t\t[ CRAFT ]\n");
	else if(SERVICE_TYPE==LISTEN) printf("Service Type \t\t\t[ LISTEN ]\n");
	else printf("Service Type \t\t\t[ UNKOWN ]\n");

}

void performJob(){
	char v[2];
	if(VERBROSE_LEVEL==1){
		strcpy(v,"1");
	} else if(VERBROSE_LEVEL==2){
		strcpy(v,"2");
	}
	if(SERVICE_TYPE==DEFENCE && MODULE_TYPE==DROP_SYN){
		char command[100]="nice -n -20 ";
		//strcat(command," nice -n -20 ./syndropper --verbrose_level ");
		strcat(command,INSTALL_DIR);
		strcat(command,"./syndropper --verbrose_level ");
		strcat(command,v);
		system(command);
	}
}

int main(int argc,char** argv) {
	int i=0;
	if(argc>=2){
		for(i=1;i<argc;i=i+2){
			if(argv[i][0]=='-' && argv[i][1]=='-') {
				if(strcmp(argv[i],"--version")==0) {
					printf(RELEASE_VERSION);
					exit(0);
				} else if(strcmp(argv[i],"--verbrose_level")==0) {
					VERBROSE_LEVEL=atoi(argv[i+1]);
				} else if(strcmp(argv[i],"--service")==0) {
					if(strcmp(argv[i+1],"defence")==0) {
						SERVICE_TYPE = DEFENCE;
					} else if(strcmp(argv[i+1],"attack")==0) {
						SERVICE_TYPE = ATTACK;
					} else if(strcmp(argv[i+1],"craft")==0) {
						SERVICE_TYPE = CRAFT;
					} else if(strcmp(argv[i+1],"listen")==0) {
						SERVICE_TYPE = LISTEN;
					} else { 
						printf("Unkown sub Arugment passed with --service [ %s ] at postion [ %d ]. Try --help for more info.\n",argv[i+1],i+1);
						exit(1);
					}
				} else if(strcmp(argv[i],"--module")==0) {
					if(strcmp(argv[i+1],"dropsyns")==0) {
						MODULE_TYPE = DROP_SYN;
					} else if(strcmp(argv[i+1],"banip")==0) {
						MODULE_TYPE = BAN_IP;
					} else {
						printf("Unkown sub Arugment passed with --module [ %s ] at postion [ %d ]. Try --help for more info.\n",argv[i+1],i+1);
						exit(1);
					}
				} else if(strcmp(argv[i],"--help")==0) {
					printUsageAndExit(0);
				} else if(strcmp(argv[i],"--threads")==0) {
					NUMBER_OF_THREADS=atoi(argv[i+1]);
				} else {
					printf("Unkown Arugment passed [ %s ] at postion [ %d ]. Try --help for more info.\n",argv[i],i);
					exit(1);
				}
			}
			else {
				printf("Unkown Arugment passed [ %s ] at postion [ %d ]. Try --help for more info.\n",argv[i],i);
				exit(1);
			}
		}
	}
	else {
		printf("Thor 1.0. Try --help for more info.\n");
	}
	showInitialSettings();
	performJob();
}

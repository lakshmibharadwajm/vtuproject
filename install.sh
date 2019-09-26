#!/bin/bash
compile(){
	echo -e "\nComipling and Generating Binaries to /tmp/thor/....\nCleaning up\t\t[PENDING]";
	sleep 2s
	rm -vrf /tmp/thor/ /opt/thor/
	mkdir -p /tmp/thor/ /opt/thor/
	echo -e "Cleaned\t\t\t[OK]\n\nInstalling to /opt/Thor/....\n\n";
}
about(){
	echo -e "\t\t\t\tAutomated Web Penetration Testing tool\nAttacking tools provided: SqlInjection, ICMP/TCP/UDP Shell, WebFuzzing, Bruteforcing, Directory busting, XSS payloads, Network Scanner, Port Scanner, Service Enumeration, OS finger print Scanner, Low Level TCP/ICMP packet Crafter";
	echo "Defence tools: Anti-DDos, Anti-BruteForcer, SynDropper, IP Banner";
	sleep 2s
}

usage(){
	echo "General Usage: "
	echo "thor --service attackType --module moduleName --threads number [--http-request file | --http-headers headers --http-cookies cookies --http-post params] --payload file --paylod file2 --payload file3  --target-host 127.0.0.1 --target-port 4444 --proxy-host 127.0.0.2 --proxy-port 3333  --gui [yes|no]"
	echo -e "\n\nCommandline parameters summary:"
	echo -e "\n--Attacktype:\n\t\t--attacktype defence : to make defence attacks\n\t\t--attacktype attack  : to perform web attack\n\t\t--attacktype craft: Used to craft low level IP packets\n\t\t--attacktype listen: Used to setup TCP/UDP/ICMP packets listener"
	echo -e "--module:\n\t\t--attacktype attack --module sqlinjection : Performs SQLInjection Testing"
	echo -e "\t\t--attacktype attack --module XSS : Performs XSS payload Attack"
	echo -e "\t\t--attacktype attack --module webfuzz : Performs webfuzzing Attack"
	echo -e "\t\t--attacktype attack --module wordlistgenerator : Generates wordslist"
	echo -e "\t\t--attacktype attack --module ns : Performs Network scan"
	echo -e "\t\t--attacktype defence --module banip : Performs IP ban"
	echo -e "\t\t--attacktype defence --module dropsyns : Performs overloaded syn dropper,antiddos,antibruteforcer"
	echo -e "\t\t--attacktype craft --module  tcp: Creates new low level tcp packet"
	echo -e "\t\t--attacktype craft --module icmp : creates new lew level icmp packet"
	echo -e "\t\t--attacktype listen --module  tcp: Creates tcp server"
	echo -e "\t\t--attacktype listen --module  http: Creates simple http web server"
	echo -e "\t\t--attacktype listen --module  udp: listens to udp packets"
	echo -e "\t\t--attacktype listen --module  icmp: listens to icmp packets"
	echo -e "--threads: \n\t\t--threads 5: Number of threads to spit the job to perform the job, default 5 threads"
}
#-----------MAIN-------------
echo "Thor 1.0" 
about
echo -e "User : `whoami` shell: $SHELL \nKernelInfo: `uname -a -p -i`"
echo -e "\n\nChecking if necessary packages were installed..., \n";
sleep 2s
if [ `id -u` -eq 0 ]; then echo -e "Running as Root \t[OK]"; else val=1; echo -e "Running as Root \t[FAILED]\nrun as root, sudo ./install.sh";exit;  fi
export val=0
nmap -V >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "NMAP \t\t\t[OK]"; else val=1; echo -e "NMAP \t\t\t[FAILED]\ntry installing nmap\nArchLinux: pacman -S nmap\nUbuntu: apt install nmap";  fi
tcpdump --version >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "TCPDUMP \t\t[OK]"; else val=1;echo -e "tcpdump \t\t[FAILED]\ntry installing TCPDUMP\nArchLinux: pacman -S tcpdump\nUbuntu: apt install tcpdump";  fi
java -version >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "JAVA-JRE8 \t\t[OK]"; else val=1;echo -e "java-jre8 \t\t[FAILED]\ntry installing JAVA-8-JRE\nArchLinux: pacman -S extra/jre8-openjdk\nUbuntu: apt install jre8";  fi
javac -version >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "JAVA-JDK8 \t\t[OK]"; else val=1;echo -e "java-jdk8 \t\t[FAILED]\ntry installing JAVA-8-JDK\nArchLinux: pacman -S extra/jdk8-openjdk\nUbuntu: apt install jdk8";  fi
gcc -v >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "GCC \t\t\t[OK]"; else val=1;echo -e "GCC \t\t\t[FAILED]\ntry installing Gcc\nArchLinux: pacman -S gcc\nUbuntu: apt install gcc";  fi
wireshark -v >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "WireShark \t\t[OK]"; else val=1;echo -e "WireShark \t\t[FAILED]\ntry installing WireShark\nArchLinux: pacman -S wireshark-qt\nUbuntu: apt install wireshark";  fi
python3 --version >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "Python3 \t\t[OK]"; else val=1;echo -e "Python3 \t\t[FAILED]\ntry installing Python3\nArchLinux: pacman -S python3\nUbuntu: apt install python3";  fi
burpsuite --version >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "Burpsuite \t\t[OK]"; else val=1;echo -e "Burpsuite \t\t[FAILED]\ntry installing Burpsuite\nArchLinux: pacman -S burpsuite\nUbuntu: apt install burpsuite";  fi
sqlmap --version >/dev/null 2>/dev/null;if [ $? -eq 0 ]; then echo -e "SqlMap \t\t\t[OK]"; else val=1;echo -e "SqlMap \t\t\t[FAILED]\ntry installing SqlMap\nArchLinux: pacman -S sqlmap\nUbuntu: apt install sqlmap";  fi
nemesis >/dev/null 2>/dev/null;if [ $? -eq 1 ]; then echo -e "nemesis \t\t[OK]"; else val=1;echo -e "nemesis \t\t\t[FAILED]\ntry installing nemesis\nArchLinux: pacman -S nemesis\nUbuntu: apt install nemesis";  fi

if [ $val -eq 0 ]; then compile;else echo -e "\n\n\nSolve Depnedcies first and rerun installtion script\n\n-Thor 1.0\n";fi


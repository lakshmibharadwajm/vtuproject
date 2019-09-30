#!/bin/bash
compile(){
	export destini=/opt/thor
	export temp_dir=/tmp/thor
	echo -e "\nCleaning up\t\t[PENDING]";
	sleep 2s
	rm -vrf $temp_dir $destini
	mkdir -p $temp_dir $destini
	echo -e "Cleaned\t\t\t[OK]\n\nComipling and Generating Binaries to $temp_dir....";
	echo -en "gcc -Wall -D INSTALL_DIR=\\\"$destini/\\\" native/dropper.c -o $temp_dir/syndropper\t\t"|xargs -t -0 -i% sh -c '%';if [ $? -eq 0 ]; then echo -e "\t\t\t[OK]\n"; else echo -e "\t\t\t[FAILED]\n";return;fi
	echo -en "gcc -Wall -D INSTALL_DIR=\\\"$destini/\\\" native/thor.c -o $temp_dir/thor\t\t"|xargs -t -0 -i% sh -c '%';if [ $? -eq 0 ]; then echo -e "\t\t\t[OK]\n"; else echo -e "\t\t\t[FAILED]\n";return;fi
	echo -e "\n\nInstalling to $destini/....\n\n";
	mv  -vt "$destini/" $temp_dir/*
	cp -fvr ./thor_completion.sh /usr/share/bash-completion/completions/thor
	source /usr/share/bash-completion/completions/thor
	cp -fvr ./native/*.sh $destini
	if [ $? -eq 0 ]; then echo -e "****************************Thor Installed Sucessfully**********************";else echo "Failed to install ";fi
}
about(){
	echo -e "\t\t\t\tAutomated Web Penetration Testing tool\nAttacking tools provided: SqlInjection, ICMP/TCP/UDP Shell, WebFuzzing, Bruteforcing, Directory busting, XSS payloads, Network Scanner, Port Scanner, Service Enumeration, OS finger print Scanner, Low Level TCP/ICMP packet Crafter";
	echo "Defence tools: Anti-DDos, Anti-BruteForcer, SynDropper, IP Banner";
	sleep 2s
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


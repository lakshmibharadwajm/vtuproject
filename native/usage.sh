#!/bin/bash
usage(){
        echo -e "\n by, CSE, GECH\n\n General Usage: "
        echo "thor --service attackType --module moduleName --threads number [--http-request file | --http-headers headers --http-cookies cookies --http-post params] --payload file --paylod file2 --payload file3  --target-host 127.0.0.1 --target-port 4444 --proxy-host 127.0.0.2 --proxy-port 3333  --gui [yes|no] --verbrose_level [1|2]"
        echo -e "\n\nCommandline parameters summary:"
        echo -e "\n--service:\n\t\t--service defence : to make defence attacks\n\t\t--service attack  : to perform web attack\n\t\t--service craft: Used to craft low level IP packets\n\t\t--service listen: Used to setup TCP/UDP/ICMP packets listener"
        echo -e "--module:\n\t\t--service attack --module sqlinjection : Performs SQLInjection Testing"
        echo -e "\t\t--service attack --module XSS : Performs XSS payload Attack"
        echo -e "\t\t--service attack --module webfuzz : Performs webfuzzing Attack"
        echo -e "\t\t--service attack --module wordlistgenerator : Generates wordslist"
        echo -e "\t\t--service attack --module ns : Performs Network scan"
        echo -e "\t\t--service defence --module banip : Performs IP ban"
        echo -e "\t\t--service defence --module dropsyns : Performs overloaded syn dropper,antiddos,antibruteforcer"
        echo -e "\t\t--service craft --module  tcp: Creates new low level tcp packet"
        echo -e "\t\t--service craft --module icmp : creates new lew level icmp packet"
        echo -e "\t\t--service listen --module  tcp: Creates tcp server"
        echo -e "\t\t--service listen --module  http: Creates simple http web server"
        echo -e "\t\t--service listen --module  udp: listens to udp packets"
        echo -e "\t\t--service listen --module  icmp: listens to icmp packets"
        echo -e "--threads: \n\t\t--threads 5: Number of threads to spit the job to perform the job, default 5 threads"
}
usage;

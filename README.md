# About

This is a penetration testing tool compromises basic blaballalalsldladslkjsdajl

## Installation

Remember: Installation script is writtend for Apt & Pacman package manager. FOr more:
[pip](https://pip.pypa.io/en/stable/) to install bllavdfjlkdjas.

```bash
cd vtuproject
sudo ./install.sh

#Dependecy: java, Jdk, Gcc, Python
```

## Usage

```python
General Usage: 
thor --service attackType --module moduleName --threads number [--http-request file | --http-headers headers --http-cookies cookies --http-post params] --payload file --paylod file2 --payload file3  --target-host 127.0.0.1 --target-port 4444 --proxy-host 127.0.0.2 --proxy-port 3333  --gui [yes|no] --verbrose_level [1|2]


Commandline parameters summary:

--service:
		--service defence : to make defence attacks
		--service attack  : to perform web attack
		--service craft: Used to craft low level IP packets
		--service listen: Used to setup TCP/UDP/ICMP packets listener
--module:
		--service attack --module sqlinjection : Performs SQLInjection Testing
		--service attack --module XSS : Performs XSS payload Attack
		--service attack --module webfuzz : Performs webfuzzing Attack
		--service attack --module wordlistgenerator : Generates wordslist
		--service attack --module ns : Performs Network scan
		--service defence --module banip : Performs IP ban
		--service defence --module dropsyns : Performs overloaded syn dropper,antiddos,antibruteforcer
		--service craft --module  tcp: Creates new low level tcp packet
		--service craft --module icmp : creates new lew level icmp packet
		--service listen --module  tcp: Creates tcp server
		--service listen --module  http: Creates simple http web server
		--service listen --module  udp: listens to udp packets
		--service listen --module  icmp: listens to icmp packets
--threads: 
		--threads 5: Number of threads to spit the job to perform the job, default 5 threads
";
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.
By GEC, CSE studnets, 2016 Batch

## License
[GNU](https://www.gnu.org/licenses/gpl-3.0.en.html)
Suggestions

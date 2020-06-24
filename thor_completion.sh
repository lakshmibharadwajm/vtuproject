#/usr/bin/env bash
attacktype=0
defaultopts="--service --help --version";
_foo() { 
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    #dont expand for these
    case $prev in --verbrose_level|--help|--version)return;esac
    #if Empty args, then set to defaults
    if [ ${#COMP_WORDS[@]} -lt 3 ]; then defaultopts="--service --help --version";fi
    # Identfiy subset options for args
    local special i
	for (( i=0; i < ${#COMP_WORDS[@]}-1; i++ )); do
	    if [[ ${COMP_WORDS[i]} == @(--service) ]]; then
	    	if [[ ${#COMP_WORDS[@]} == 3 ]]; then
	        	special=${COMP_WORDS[i]}
	        fi
	    elif [[ ${COMP_WORDS[i]} == @(--module) ]]; then
	    	if [[ ${#COMP_WORDS[@]} == 5 ]]; then
	        	special=${COMP_WORDS[i]}
	        fi
	    fi
	done
	# Produce subset options
    if [[ -n $special ]]; then
    	case $special in --service)
    		opts="attack defence listen craft";
    		COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
    		x=$(compgen -W "${opts}" -- ${cur})
    		if [ "$x" == "attack" ];then attacktype=1;
    		elif [ "$x" == "defence" ];then attacktype=2;
    		elif [ "$x" == "listen" ];then attacktype=4;
    		elif [ "$x" == "craft" ];then attacktype=3;
    		else attacktype=2;
    		fi;
    		defaultopts="--module"
    		return
    		;;
    	esac
    	case $special in --module)
    		if [ $attacktype == 1 ];then opts="sqlinjection xss webfuzz wordlistgenerator ns";
    		elif [ $attacktype == 2 ];then opts="banip dropsyns";
    		elif [ $attacktype == 3 ];then opts="tcp icmp";
    		elif [ $attacktype == 4 ];then opts="tcp http udp icmp";
    		else opts="SOMETHING";
    		fi;
    		COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
    		defaultopts="--verbrose_level --port"
    		return
    		;;
    	esac
    fi
    #Execute default string if no special args found
    #if [[ ${prev} == "--service" ]]; then opts="attack defence listen craft"; else opts="--help --verbose --version --service --module";fi
    if [[ ${cur} == * ]] ; then
        COMPREPLY=( $(compgen -W "${defaultopts}" -- ${cur}) )
        return 0
    fi
}
complete -F _foo thor

#!/usr/bin/env bash
	
	MOD_LEVEL_ITEM_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"
	
	source $MOD_LEVEL_ITEM_ROOT"/conf/conf.sh.dist"
	
	if [ -f $MOD_LEVEL_ITEM_ROOT"/conf/conf.sh" ]; then
	    source $MOD_LEVEL_ITEM_ROOT"/conf/conf.sh"
	fi

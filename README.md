
AT-command interpreter
=========================

A simple AT command library implement based on *FSM*(parse AT-script) , *queue*(AT-notation buffer) and *hash looup*(AT-notation store).

![Build Status](https://travis-ci.org/Shylock-Hg/AT-interpreter.svg?branch=master)

## overview

standard record string : "AT+HGABC?;+HGDDD=?;+HGSBD;+HGHIO=1,2,\"hello world!\n\"\n"

1. at command register sequence :
at command struct table --> hash lookup table.


2. at command handle sequence :
origin input string -*filter*-> standard record string -*fsm*-> list of at_cmd_t 
--> perform list --> release list.


## terminology

1. record : "AT+HGABC?;+HGDDD=?;+HGSBD;+HGHIO=1,2,\"hello world!\n\"\n"
2. prefix : "AT"
3. command : "+HGABC?" , "+HGDDD=?" , "+HGSBD" and "+HGHIO=1,2,\"hello world!\n\"\n"
4. command name : "+HGABC" , "+HGDDD" , "+HGSBD" and "+HGHIO"
5. delimiter command : ';'
6. delimiter record : '\n'


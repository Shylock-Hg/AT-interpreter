
AT-command interpreter
=========================

[![Build Status](https://travis-ci.org/Shylock-Hg/AT-interpreter.svg?branch=master)](https://travis-ci.org/Shylock-Hg/AT-interpreter)
[![codecov](https://codecov.io/gh/Shylock-Hg/AT-interpreter/branch/master/graph/badge.svg)](https://codecov.io/gh/Shylock-Hg/AT-interpreter)
[![CodeFactor](https://www.codefactor.io/repository/github/shylock-hg/at-interpreter/badge)](https://www.codefactor.io/repository/github/shylock-hg/at-interpreter)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Shylock-Hg/AT-interpreter.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Shylock-Hg/AT-interpreter/context:cpp)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)

A simple AT command library implement based on *FSM*(parse AT-script) , *queue*(AT-notation buffer) and *hash looup*(AT-notation store).

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

## AT front-end FSM

![AT-FSM](https://drive.google.com/uc?id=1gcYMMP4pJyG8tfi-xOeEGJjwGsGMwHuJ)


ibm:	ex_walkup_new.cc
	bgxlC_r -o ex_walkup_ibm -g -O4 ex_walkup_new.cc

ibme:	ex_walkup_new.cc
	bgxlC_r -c -g -O4 -E ex_walkup_new.cc >& ex_walkup_new.cc.E

gnu:	ex_walkup.c
	bggcc -o ex_walkup_gnu -O3 ex_walkup.c

clean:
	@-rm -f ex_walkup_ibm *~

cleanrun:
	@-rm -f *.log *.out *.core


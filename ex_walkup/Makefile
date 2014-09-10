ibm:	ex_walkup_new.cc
	bgxlC_r -o ex_walkup_ibm -g -O4 ex_walkup_new.cc
gnu:	ex_walkup.c
	bggcc -o ex_walkup_gnu -O3 ex_walkup.c

clean:
	@-rm -f ex_walkup_ibm *~

cleanrun:
	@-rm -f *.log *.out *.core


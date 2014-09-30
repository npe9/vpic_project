default: ex_bits_sp_gnu_ib ex_bits_dp_gnu_ib ex_bits_sp_int_ib ex_bits_dp_int_ib

ex_bits_sp_gnu_ib: ex_bits_sp.cc v4_port_sp.hh
	g++ -o ex_bits_sp_gnu_ib -g -O2 ex_bits_sp.cc

ex_bits_dp_gnu_ib: ex_bits_dp.cc v4_port_dp.hh
	g++ -o ex_bits_dp_gnu_ib -g -O2 ex_bits_dp.cc

ex_bits_sp_int_ib: ex_bits_sp.cc v4_port_sp.hh
	icpc -o ex_bits_sp_int_ib -g -O2 ex_bits_sp.cc

ex_bits_dp_int_ib: ex_bits_dp.cc v4_port_dp.hh
	icpc -o ex_bits_dp_int_ib -g -O2 ex_bits_dp.cc

ibm:	ex_walkup.cc
	bgxlC_r -o ex_walkup_ibm -g -O4 ex_walkup.cc

ibme:	ex_walkup.cc
	bgxlC_r -c -g -O4 -E ex_walkup.cc >& ex_walkup.cc.E

gnu:	ex_walkup.c
	bggcc -o ex_walkup_gnu -O3 ex_walkup.c

clean:
	@-rm -f ex_walkup_ibm *~ ex_bits_sp_gnu_ib ex_bits_dp_gnu_ib ex_bits_sp_int_ib ex_bits_dp_int_ib

cleanrun:
	@-rm -f *.log *.out *.core


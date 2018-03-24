# run Makefile

all:  settings fort pcapt 

settings:
	g++ ./settings.cc -o ../run/settings.exe

fort:
	gfortran P1.FOR P2.FOR P3.FOR P4.FOR -o ../run/fort.exe

pcapt:
	g++ pcapt.cc -lgmpxx -lgmp -lmpfr -lgsl -lgslcblas -std=c++11 -o ../run/pcapt.exe 

help: 
	@printf "Example (if you are in ./source/ folder):\nmake all;\ncd ../run/;\n./settings.exe -A1 40 -Z1 20 -A2 40 -Z2 18 < "yes";\n";
	@printf "./fort.exe;\n./pcapt.exe;\nR CMD BATCH plot.R;\nR CMD BATCH potentail.R;\n";
	@printf "R CMD BATCH prob.R;\nR CMD BATCH probd.R;\n"

clean:
	-rm -f ../run/CaAr.DAT 
	-rm -f ../run/conf.potential.txt 
	-rm -f ../run/pcapt.exe 
	-rm -f ../run/potential2.png 
	-rm -f ../run/potential.Rout 
	-rm -f ../run/probd.R 
	-rm -f ../run/prob.R
	-rm -f ../run/conf.inp.txt 
	-rm -f ../run/fort.exe 
	-rm -f ../run/plot.R 
	-rm -f ../run/potential.png 
	-rm -f ../run/probability.deformed.png 
	-rm -f ../run/probd.Rout 
	-rm -f ../run/prob.Rout
	-rm -f ../run/conf.out.txt 
	-rm -f ../run/INP.DAT 
	-rm -f ../run/plot.Rout 
	-rm -f ../run/potential.R 
	-rm -f ../run/probability.png 
	-rm -f ../run/prob.energies.txt 
	-rm -f ../run/settings.exe


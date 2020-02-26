CHARMC=/Users/epmikida/Programming/online_tutorial/charm/bin/charmc

all: main

main: main.decl.h main.C
	$(CHARMC) -language charm++ main.C -o main

main.decl.h: main.ci
	$(CHARMC) main.ci

clean:
	rm -f *.decl.h *.def.h *.o main charmrun

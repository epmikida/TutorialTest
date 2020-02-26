CHARMC=/Users/epmikida/Programming/online_tutorial/charm/bin/charmc

all: ring collective

ring: ring.decl.h ring.C
	$(CHARMC) -language charm++ ring.C -o ring

ring.decl.h: ring.ci
	$(CHARMC) ring.ci

collective: collective.decl.h collective.C
	$(CHARMC) -language charm++ collective.C -o collective

collective.decl.h: collective.ci
	$(CHARMC) collective.ci

clean:
	rm -f *.decl.h *.def.h *.o ring collective charmrun

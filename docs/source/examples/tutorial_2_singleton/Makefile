CHARMC=/Users/epmikida/Programming/online_tutorial/charm/bin/charmc

all: singleton multiple

singleton: singleton.decl.h singleton.C
	$(CHARMC) -language charm++ singleton.C -o singleton

singleton.decl.h: singleton.ci
	$(CHARMC) singleton.ci

multiple: multiple.decl.h multiple.C
	$(CHARMC) -language charm++ multiple.C -o multiple

multiple.decl.h: multiple.ci
	$(CHARMC) multiple.ci

clean:
	rm -f *.decl.h *.def.h *.o charmrun singleton multiple

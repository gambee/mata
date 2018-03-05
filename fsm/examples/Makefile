%.out : %.h
	cp $< FSM.h
	gcc test.c -o $@
%.svg : %.dot
	dot -Tsvg $< > $@
%.dot %.h : %.mata
	./parser $< 
clear: 
	rm *.out *.svg *.dot  FSM.h

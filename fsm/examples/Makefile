%.out : %.h
	cp $< FSM.h
	gcc test.c -o $@
	rm FSM.h
%.svg : %.dot
	dot -Tsvg $< > $@
%.dot %.h : %.mata
	cat $< | ./parser $< 

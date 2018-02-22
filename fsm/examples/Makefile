%.svg : %.dot
	dot -Tsvg $< > $@
%.dot : %.mata
	cat $< | ./parser $@ 

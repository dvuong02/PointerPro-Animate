# You may modify this file as you wish, but `make animate.o` should always
# compile all necessary code into a single object file.

# builds animate.o and test_simple when makefile is run without arguments
default: animate.o test_simple

# file variables to simplify code
source = animate.c canvas.c frame.c sprite.c sprite_placement.c
header = animate.h animate_struct.h

# builds test program: dependencies | headers
test_simple: main_simple.c animate.o | $(header)
    # $^ short form for all dependencies, $@ short form for target name
    # gcc main_simple.c animate.o -o test_simple
	gcc $^ -o $@

# builds animate object file. "|" means 'Do not recompile dependencies if headers
# change', but we remove it because we must recompile source files if header changes
animate.o: $(source) $(header)
    # 'if any file in $(source) or $(header) changes, animate.o is out of date
    # to rebuild animate.o, run the command below:
    # gcc -fPIC -c animate.c -o animate.o
	gcc -fPIC -c animate.c -o $@

API_DOC=PointerProAnimateRefman.pdf
doc: $(API_DOC)

# Generates a doxygen configuration file
Doxyfile:
	doxygen -g
    # Adjust params
	sed -i 's/\(GENERATE_HTML *= *\).*/\1NO/g' $@
	sed -i 's/\(EXTRACT_ALL *= *\).*/\1YES/g' $@
	sed -i 's/\(INPUT *= *\).*/\1"animate.h"/g' $@
	sed -i 's/\(PROJECT_NAME *= *\).*/\1"PointerPro Animate"/g' $@
	sed -i 's/\(OPTIMIZE_OUTPUT_FOR_C *= *\).*/\1YES/g' $@

$(API_DOC): DOC_MAKEFILE=latex/Makefile
$(API_DOC): DOC_TOP=latex/refman.tex
$(API_DOC): Doxyfile | animate.h
	doxygen $^
    # Don't index
	sed -i 's/\t$$(MKIDX/\t#/g'             $(DOC_MAKEFILE)
    # Remove some dead chapters and ToC
	sed -i 's/^ *\\clearemptydoublepage//g' $(DOC_TOP)
	sed -i 's/^ *\\tableofcontents//g'      $(DOC_TOP)
	sed -i 's/^ *\\chapter{File Index}//g'  $(DOC_TOP)
	sed -i 's/^ *\\input{files}//g'         $(DOC_TOP)
	cd latex && make
	cp latex/refman.pdf $@

clean:
	rm -f animate.o
	rm -f Doxyfile
	rm -rf latex

clobber: clean
	rm -f $(API_DOC)

.PHONY: doc clean clobber default
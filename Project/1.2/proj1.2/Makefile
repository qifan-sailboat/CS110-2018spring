UNAME_S := $(shell uname -s)
TAR_FLAGS = --exclude=".DS_Store" --exclude="*/.DS_Store"
ifeq ($(UNAME_S),Darwin)
	TAR_FLAGS += --disable-copyfile
endif

test:
	@java -jar Mars.jar nc sm linker-tests/test_string.s; \
	java -jar Mars.jar nc sm linker-tests/test_symbol_list.s; \
	java -jar Mars.jar nc sm linker-tests/test_parsetools.s; \
	java -jar Mars.jar nc sm linker-tests/test_linker_utils.s; \
	./linker link-in/linker1.out link-out/output1; \
	diff link-out/output1 link-out/ref/output1_ref && echo "Good Job!"; \
	./linker link-in/linker2.out link-out/output2; \
	diff link-out/output2 link-out/ref/output2_ref && echo "Good Job!"; \
	./linker link-in/linker3A.out link-in/linker3B.out link-out/output3; \
	diff link-out/output3 link-out/ref/output3_ref && echo "Good Job!";

clean:
	@printf "Cleanning up tar file... " && rm -f proj1.2.tar && echo "Done."

tar: clean
	@printf "Generating new tar file... " && \
	mkdir -p /tmp/proj1.2/linker-src && \
	cp linker-src/file_utils.s linker-src/linker_utils.s linker-src/linker.s \
		linker-src/parsetools.s linker-src/string.s linker-src/symbol_list.s /tmp/proj1.2/linker-src/ && \
	tar cf proj1.2.tar $(TAR_FLAGS) -C /tmp/ proj1.2 && \
	rm -rf /tmp/proj1.2/ && \
	echo "Done."

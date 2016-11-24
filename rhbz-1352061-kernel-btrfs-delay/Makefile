all: reproduce
	
.PHONY: reproduce checkfs

checkfs:
	test `stat --file-system --format '%T' main.c` = btrfs


reproduce: binary checkfs
	while : ; do ./reproducer; done

binary: main.c
	gcc $< -O0 -g3 -o $@

clean:
	rm -rf binary sparsefile

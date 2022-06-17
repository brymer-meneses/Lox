
.PHONY: treewalker bytecode


treewalker:
	$(MAKE) -C "treewalker" all

bytecode:
	$(MAKE) -C "bytecode" all

compile:
	$(MAKE) -C "treewalker" compile
	# $(MAKE) -C "CLoxVM" compile

clean:
	$(MAKE) -C "treewalker" clean



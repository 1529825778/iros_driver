.PHONY: clean install

INSTALL_DIR		= build

all: install 

install: 
	-mkdir -p $(INSTALL_DIR)/include
	cp -f include/*.hpp $(INSTALL_DIR)/include

clean:
	-rm -rf $(INSTALL_DIR)


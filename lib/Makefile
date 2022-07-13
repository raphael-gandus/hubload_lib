CC= /home/raph/Development-tools/test1/output/host/bin/arm-buildroot-linux-gnueabihf-gcc
AR= ar -rv
NAME= Hubload
SRC= $(NAME).c
HDR= $(SRC:.c=.h)
OBJ= $(SRC:.c=.o)
LIB= lib$(SRC:.c=).a
DIR= /home/raph/Development-tools/test1/output/target

all:	expander_i2c.o	MCP3202.o	rtc_eeprom.o	pn532.o	PN532_Rpi_I2C.o	dma.o	mailbox.o	pcm.o	pwm.o	rpihw.o	ws2811.o
#	echo "On change de repertoire -> /lib";
#	cd ./lib;
	@echo "On genere les bibliothèques statiques";
#	sudo gcc -o lib$(NAME).so -shared $^;
	sudo $(AR) lib$(NAME).a $^;

	

expander_i2c.o:	expander_i2c.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;

MCP3202.o:	MCP3202.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;

rtc_eeprom.o:	rtc_eeprom.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;

pn532.o:	pn532.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;

PN532_Rpi_I2C.o:	PN532_Rpi_I2C.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;


dma.o:	dma.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;


mailbox.o:	mailbox.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;

pcm.o:	pcm.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;


pwm.o:	pwm.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;

rpihw.o:	rpihw.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;


ws2811.o:	ws2811.c
	@echo "generation de l'objet...";
	sudo $(CC) -o $@ -c $<;


.PHONY: install
install:
	@echo "installation des bibliothèques";
	sudo cp *.a $(DIR)/usr/lib;
	sudo cp *.h $(DIR)/usr/include;
	@echo "l'installation fini";
	@echo "checking des installations";
# cd ../test; 
	sudo gcc ../test/test_lib.c -o ../test/test_lib.exe -Wall -lHubload;
	sudo ../test/test_lib.exe;
# cd ../;

.PHONY: uninstall
uninstall:
	sudo rm -f $(DIR)/usr/lib/libHubload.*;
	sudo rm -f $(DIR)/usr/include/expander_i2c.h $(DIR)/usr/include/MCP3202.h $(DIR)/usr/include/rtc_eeprom.h $(DIR)/usr/include/pn532.h $(DIR)/usr/include/PN532_Rpi_I2C.h;

.PHONY: clean
clean:
	sudo rm -f *.o *.a;

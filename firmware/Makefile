build: build.file
	idf.py -DCMAKE_EXPORT_COMPILE_COMMANDS=1 build
	touch build.file

menuconfig:
	idf.py menuconfig

clean:
	idf.py clean

fullclean:
	idf.py fullclean
	idf.py python-clean

flash:
	idf.py -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -p /dev/ttyUSB0 flash

monitor:
	idf.py -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -p /dev/ttyUSB0 monitor

fm:
	idf.py -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -p /dev/ttyUSB0 flash monitor

docs:
	idf.py docs

size:
	idf.py size

filesizes:
	idf.py -DOUTPUT_JSON=1 size-files

build.file:


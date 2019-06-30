TARGETS= fraction_demo
INC_FLAGS= -I./include
CPP_FLAGS= -O2 -Wall -std=c++11 -pthread  -Wno-deprecated-declarations 

PLAT_FLAGS=-L/System/Library/Frameworks -framework OpenGL -framework GLUT -framework Foundation 

CC=gcc
CXX=g++

.SUFFIXES: .cpp .c

PHYSIC_O= build/fraction.o build/acos_table.o build/atan2_table.o build/sincos_table.o
APP_O= build/main.o build/application.o 

DEMO_FRACTION_O= build/fraction_demo.o

none:
	@echo "Please do"
	@echo "   make TARGET"
	@echo "where TARGET is one of these:"
	@echo "   $(TARGETS)"

${PHYSIC_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/fraction.cpp -o build/fraction.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/acos_table.cpp -o build/acos_table.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/atan2_table.cpp -o build/atan2_table.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/sincos_table.cpp -o build/sincos_table.o	

${APP_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} application.cpp -o build/application.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} main.cpp -o build/main.o

${DEMO_FRACTION_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} -I./samples/fraction  samples/fraction/fraction_demo.cpp -o build/fraction_demo.o
	
DEMO_FRACTION= ${APP_O} ${PHYSIC_O} ${DEMO_FRACTION_O}

clean_fraction_demo:
	rm -f ${DEMO_FRACTION} ./bin/fraction_demo
fraction_demo: clean_fraction_demo ${DEMO_FRACTION}
	${CXX} ${PLAT_FLAGS} -o ./bin/fraction_demo ${DEMO_FRACTION}

clean: clean_fraction_demo

	
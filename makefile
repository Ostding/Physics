TARGETS= fraction_demo
INC_FLAGS= -I./include -I./
CPP_FLAGS= -g -Wall -std=c++11 -pthread -Wno-shift-count-overflow  -Wno-deprecated-declarations 

PLAT_FLAGS=-L/System/Library/Frameworks -framework OpenGL -framework GLUT -framework Foundation 

CC=gcc
CXX=g++

.SUFFIXES: .cpp .c

dir:
	mkdir -p ./build
	mkdir -p ./bin

PHYSIC_O= build/timer.o build/fraction.o build/acos_table.o build/atan2_table.o build/sincos_table.o
APP_O= build/main.o build/application.o 

DEMO_FIXPOINT_O= build/fix_point_demo.o

none:
	@echo "Please do"
	@echo "   make TARGET"
	@echo "where TARGET is one of these:"
	@echo "   $(TARGETS)"

${PHYSIC_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/timer.cpp -o build/timer.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/fraction.cpp -o build/fraction.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/acos_table.cpp -o build/acos_table.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/atan2_table.cpp -o build/atan2_table.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/sincos_table.cpp -o build/sincos_table.o	

${APP_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} application.cpp -o build/application.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} main.cpp -o build/main.o

${DEMO_FIXPOINT_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} -I./samples/fixpoint  samples/fixpoint/fix_point_demo.cpp -o build/fix_point_demo.o
	
DEMO_FIXPOINT= ${APP_O} ${PHYSIC_O} ${DEMO_FIXPOINT_O}

clean_fix_point_demo: 
	rm -f ${DEMO_FIXPOINT} ./bin/fix_point_demo
fix_point_demo: clean_fix_point_demo dir ${DEMO_FIXPOINT}
	${CXX} ${PLAT_FLAGS} -o ./bin/fix_point_demo ${DEMO_FIXPOINT}

clean: clean_fix_point_demo

	
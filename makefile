TARGETS= fraction_demo
INC_FLAGS= -I./include -I./
CPP_FLAGS= -g -Wall -std=c++11 -pthread -Wno-shift-count-overflow  -Wno-deprecated-declarations -Wno-unused-function

PLAT_FLAGS=-L/System/Library/Frameworks -framework OpenGL -framework GLUT -framework Foundation 

CC=gcc
CXX=g++

.SUFFIXES: .cpp .c

dir:
	mkdir -p ./build
	mkdir -p ./bin

PHYSIC_O= build/timer.o build/fixed_float.o build/vector3.o build/quaternion.o \
					build/body.o build/primitive.o build/plane.o build/sphere.o\
					build/renderer.o 

APP_O= build/main.o build/application.o 

none:
	@echo "Please do"
	@echo "   make TARGET"
	@echo "where TARGET is one of these:"
	@echo "   $(TARGETS)"

${PHYSIC_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/timer.cpp -o build/timer.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/fixed_float.cpp -o build/fixed_float.o	
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/vector3.cpp -o build/vector3.o	
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/quaternion.cpp -o build/quaternion.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/body.cpp -o build/body.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/primitive.cpp -o build/primitive.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/plane.cpp -o build/plane.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/sphere.cpp -o build/sphere.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact.cpp -o build/contact.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_generator.cpp -o build/contact_generator.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_processor.cpp -o build/contact_processor.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/force_generator.cpp -o build/force_generator.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/force_manager.cpp -o build/force_manager.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/world.cpp -o build/world.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/renderer.cpp -o build/renderer.o

${APP_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} application.cpp -o build/application.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} main.cpp -o build/main.o

########################
DEMO_FIXPOINT_O= build/fix_point_demo.o \
								 build/test_fixed_float.o

${DEMO_FIXPOINT_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} -I./samples/fixpoint  samples/fixpoint/test_fixed_float.cpp -o build/test_fixed_float.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} -I./samples/fixpoint  samples/fixpoint/fix_point_demo.cpp -o build/fix_point_demo.o
	
DEMO_FIXPOINT= ${APP_O} ${PHYSIC_O} ${DEMO_FIXPOINT_O}

clean_fix_point_demo: 
	rm -f ${DEMO_FIXPOINT} ./bin/fix_point_demo
fix_point_demo: clean_fix_point_demo dir ${DEMO_FIXPOINT}
	${CXX} ${PLAT_FLAGS} -o ./bin/fix_point_demo ${DEMO_FIXPOINT}

#########################
DEMO_PRIMITIVES_O= build/primitives_demo.o 

${DEMO_PRIMITIVES_O}:
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} -I./samples/primitives  samples/primitives/primitives_demo.cpp -o build/primitives_demo.o
	
DEMO_PRIMITIVES= ${APP_O} ${PHYSIC_O} ${DEMO_PRIMITIVES_O}

clean_primitives_demo:
	rm -f ${DEMO_PRIMITIVES} ./bin/primitives_demo
primitives_demo: clean_primitives_demo dir ${DEMO_PRIMITIVES}
	${CXX} ${PLAT_FLAGS} -o ./bin/primitives_demo ${DEMO_PRIMITIVES}

clean: clean_fix_point_demo

	
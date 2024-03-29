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

PHYSIC_O= build/timer.o build/fixed_float.o build/vector3.o build/quaternion.o build/body.o \
					build/primitive.o build/plane.o build/sphere.o build/box.o build/polyhedron.o build/capsule.o \
					build/contact.o build/collision_data.o build/contact_generator.o build/contact_processor.o \
					build/contact_sphere.o build/contact_box.o build/contact_polyhedron.o build/contact_capsule.o\
					build/force_manager.o build/gravity_force.o build/move_force.o build/spring_force.o \
					build/octree.o build/world.o build/renderer.o \
					build/gjk_epa.o build/utils.o

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
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/box.cpp -o build/box.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/polyhedron.cpp -o build/polyhedron.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/capsule.cpp -o build/capsule.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/octree.cpp -o build/octree.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact.cpp -o build/contact.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/collision_data.cpp -o build/collision_data.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_generator.cpp -o build/contact_generator.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_sphere.cpp -o build/contact_sphere.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_box.cpp -o build/contact_box.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_polyhedron.cpp -o build/contact_polyhedron.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_capsule.cpp -o build/contact_capsule.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/contact_processor.cpp -o build/contact_processor.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/force_manager.cpp -o build/force_manager.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/gravity_force.cpp -o build/gravity_force.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/move_force.cpp -o build/move_force.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/spring_force.cpp -o build/spring_force.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/world.cpp -o build/world.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/renderer.cpp -o build/renderer.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/utils.cpp -o build/utils.o
	${CXX} -c ${INC_FLAGS} ${CPP_FLAGS} src/gjk_epa.cpp -o build/gjk_epa.o

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

	
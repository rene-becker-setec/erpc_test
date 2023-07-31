CXX      := -c++
#CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror
CXXFLAGS := -pedantic-errors -Wall
LDFLAGS  := -L/usr/lib -lstdc++ -lm -pthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
GEN_DIR  := $(BUILD)/generated
TARGET   := program
INCLUDE  := -Iinclude/ \
            -I$(GEN_DIR) \
			-I/home/rene/Projects/erpc/erpc_c/infra \
            -I/home/rene/Projects/erpc/erpc_c/port \
			-I/home/rene/Projects/erpc/erpc_c/config \
			-I/home/rene/Projects/erpc/erpc_c/transports \
			-I/home/rene/Projects/erpc/erpc_c/setup
SRC      :=                      \
   $(wildcard src/*.cpp)         \
   $(GEN_DIR)/example_pc2emu_server.cpp \
   $(GEN_DIR)/example_emu2pc_client.cpp \
   /home/rene/Projects/erpc/erpc_c/setup/erpc_setup_tcp.cpp \
   /home/rene/Projects/erpc/erpc_c/setup/erpc_setup_mbf_dynamic.cpp \
   /home/rene/Projects/erpc/erpc_c/setup/erpc_server_setup.cpp \
   /home/rene/Projects/erpc/erpc_c/setup/erpc_arbitrated_client_setup.cpp \
   /home/rene/Projects/erpc/erpc_c/transports/erpc_tcp_transport.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_server.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_framed_transport.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_crc16.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_message_buffer.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_simple_server.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_basic_codec.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_arbitrated_client_manager.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_client_manager.cpp \
   /home/rene/Projects/erpc/erpc_c/infra/erpc_transport_arbitrator.cpp \
   /home/rene/Projects/erpc/erpc_c/port/erpc_port_stdlib.cpp \
   /home/rene/Projects/erpc/erpc_c/port/erpc_threading_pthreads.cpp

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(GEN_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"

# root directory's path
ROOT_DIR:=$(strip $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))))

# compiler
ifndef CXX
CXX =g++
endif

# compiler flags
CXXFLAGS =-Wall -Wextra -std=c++17 -Wno-write-strings

ifeq ($(strip $(CXX)),clang++)
CXXFLAGS +=-Wno-unused-command-line-argument -fsanitize=address
endif

# release flags
RFLAGS =-O2

# debug flags
DFLAGS =-ggdb -Og

# Include flags
IFLAGS =-I$(ROOT_DIR)/include/

# Extract libgvc (Graphviz toolkit) from system
LIBGVC_LDFLAGS :=$(shell pkg-config libgvc --libs)


#################
ifndef LIBGVC_LDFLAGS
$(error libgvc linker flags not found. Please check if library is present.)
endif

LIBGVC_IFLAGS :=$(shell pkg-config libgvc --cflags)

ifndef LIBGVC_IFLAGS
$(error libgvc include flags not found. Please check if library is present.)
endif
#################

#################
GTEST_LDFLAGS :=$(shell pkg-config gtest --libs)

ifndef GTEST_LDFLAGS
$(error gtest linker flags not found. Please check if library is present.)
endif

GTEST_IFLAGS :=$(shell pkg-config gtest --cflags)

ifndef GTEST_IFLAGS
$(error gtest include flags not found. Please check if library is present.)
endif
#################

#################
GTEST_MAIN_LDFLAGS :=$(shell pkg-config gtest_main --libs)

ifndef GTEST_MAIN_LDFLAGS
$(error gtest_main linker flags not found. Please check if library is present.)
endif

GTEST_MAIN_IFLAGS :=$(shell pkg-config gtest_main --cflags)

ifndef GTEST_MAIN_IFLAGS
$(error gtest include flags not found. Please check if library is present.)
endif

#################

GVIZ_DIR =$(ROOT_DIR)/src/graphviz
GVIZ_SRC :=$(shell find $(GVIZ_DIR)/ -follow -type f -name "*.cpp")
GVIZ_OBJ =$(GVIZ_SRC:.cpp=.o)

TESTDIR =$(ROOT_DIR)/tests
EXAMPLES_DIR =$(ROOT_DIR)/examples

define print_cxx_target
@echo '        CXX $(notdir $(1))'
endef

TESTS_LFLAGS = $(GTEST_LDFLAGS) $(LIBGVC_LDFLAGS)
TESTS_IFLAGS = $(IFLAGS) $(GTEST_IFLAGS) $(LIBGVC_IFLAGS)

.PHONY = help
help:
	@echo 'Usage:'
	@echo '    make all - build whole projects: lib + tests + examples.'
	@echo '    make clean - Cleans from *.o, *.png and executables.'
	@echo '        This does not delete any files located in /img/ directory.'
	@echo '    make install - install all visualization programs to the system.'
	@echo '    make uninstall - uninstall all visualization programs from the system.'

.PHONY = install
install:
	@sudo cp examples/bfs_vizu /usr/bin/
	@sudo cp examples/detect_cycles /usr/bin/
	@sudo cp examples/dfs_vizu /usr/bin/
	@sudo cp examples/sccs /usr/bin/
	@sudo cp examples/tpsort /usr/bin/

uninstall:
	@sudo rm /usr/bin/bfs_vizu
	@sudo rm /usr/bin/detect_cycles
	@sudo rm /usr/bin/dfs_vizu
	@sudo rm /usr/bin/sccs
	@sudo rm /usr/bin/tpsort

#EXAMPLES =$(EXAMPLES_DIR)/detect_cycles $(EXAMPLES_DIR)/sccs_visual $(EXAMPLES_DIR)/dfs_vizu $(EXAMPLES_DIR)/bfs_vizu
#TESTS =$(TESTDIR)/dfs_test $(TESTDIR)/adj_list_test $(TESTDIR)/adj_matrix_test $(TESTDIR)/tpsort_test $(TESTDIR)/sccs_test $(TESTDIR)/gtest

all_info: info all

all: gviz_wrapper $(EXAMPLES_DIR)/detect_cycles $(EXAMPLES_DIR)/tpsort $(EXAMPLES_DIR)/sccs $(EXAMPLES_DIR)/dfs_vizu $(EXAMPLES_DIR)/bfs_vizu $(TESTDIR)/dfs_test $(TESTDIR)/adj_list_test $(TESTDIR)/adj_matrix_test $(TESTDIR)/tpsort_test $(TESTDIR)/sccs_test $(TESTDIR)/gtest

.PHONY=test
test:
	@./tests/gtest --gtest_color=yes

.PHONY = info
info:
	@echo '----------------------------------------'
	@echo 'CXX: $(CXX)'
	@echo 'CXXFLAGS: $(CXXFLAGS)'
	@echo 'RFLAGS: $(RFLAGS)'
	@echo 'DFLAGS: $(DFLAGS)'
	@echo 'GTEST: $(GTEST_LDFLAGS) $(GTEST_IFLAGS)'
	@echo 'GTEST_MAIN: $(GTEST_MAIN_LDFLAGS) $(GTEST_MAIN_IFLAGS)'
	@echo '----------------------------------------'

# build all object files of wrapper
gviz_wrapper: $(GVIZ_OBJ)

# recipe for compiling graphviz wrapper file
$(GVIZ_DIR)/%.o: $(GVIZ_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -c $^ -o $@ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)


$(EXAMPLES_DIR)/tpsort: $(EXAMPLES_DIR)/tpsort.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(EXAMPLES_DIR)/detect_cycles: $(EXAMPLES_DIR)/detect_cycles.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(EXAMPLES_DIR)/dfs_vizu: $(EXAMPLES_DIR)/dfs_vizu.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(EXAMPLES_DIR)/sccs: $(EXAMPLES_DIR)/sccs.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS) src/utility.cpp
	$(call print_cxx_target, $@)

$(EXAMPLES_DIR)/bfs_vizu: $(EXAMPLES_DIR)/bfs_vizu.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(TESTDIR)/dfs_test: $(TESTDIR)/dfs_test.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(TESTDIR)/gtest: $(TESTDIR)/gtest.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(TESTS_LFLAGS) $(TESTS_IFLAGS)
	$(call print_cxx_target, $@)

$(TESTDIR)/tpsort_test: $(TESTDIR)/tpsort_test.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(TESTDIR)/sccs_test: $(TESTDIR)/sccs_test.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(TESTDIR)/adj_list_test: $(TESTDIR)/adj_list_test.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

$(TESTDIR)/adj_matrix_test: $(TESTDIR)/adj_matrix_test.cpp $(GVIZ_OBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) -o $@ $^ $(IFLAGS) $(LIBGVC_IFLAGS) $(LIBGVC_LDFLAGS)
	$(call print_cxx_target, $@)

.PHONY = clean
clean:
	@find ./src/ -type f -name "*.o" -delete
	@find ./tests/ -type f -executable -delete
	@find ./examples/ -type f -executable -delete


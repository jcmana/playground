CXX = /usr/bin/clang++-7
CXX_FLAGS = -Wpedantic

OUT_BIN = main
OUT_DIR = ./build

SOURCES=main.cpp
HEADERS=callback_store.hpp callback.hpp callback_guard.hpp

$(OUT_BIN) : $(SOURCES) $(HEADERS)
	mkdir -p $(OUT_DIR)
	$(CXX) $(CXX_FLAGS) $(SOURCES) -o $(OUT_DIR)/$(OUT_BIN)

run : $(OUT_BIN)
	$(OUT_DIR)/$(OUT_BIN)

clean :
	rm $(OUT_DIR)/$(OUT_BIN) 2> /dev/null
	rmdir $(OUT_DIR) 2> /dev/null
	exit 0



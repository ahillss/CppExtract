EXE = $(OUT)/CppExtract
OUT = bin
CC = g++

CPPFLAGS := `llvm-config --cxxflags` -fexceptions -std=c++11

LDFLAGS := \
	-lclangFrontend \
	-lclangDriver \
	-lclangSerialization \
	-lclangParse \
	-lclangSema \
	-lclangAnalysis \
	-lclangEdit \
	-lclangAST \
	-lclangLex \
	-lclangBasic \
	`llvm-config --libs` \
	`llvm-config --ldflags` \
	`llvm-config --system-libs`


objs := $(patsubst %.cpp,$(OUT)/%.o,$(wildcard *.cpp))
deps := $(objs:.o=.dep)

.PHONY: all test
all: $(EXE)

-include $(deps)

$(OUT)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) -c $< -o $@
	@$(CC) $(CPPFLAGS) -MM $< | sed -e '1,1 s|[^:]*:|$@:|' > $(@D)/$*.dep

$(EXE) : $(objs)
	$(CC) $^ $(LDFLAGS) -o $@

test:
	@$(EXE) -lang=c -I/usr/include -Itests stb_image.h

clean:
	@rm -f $(objs) $(deps) $(EXE)
	@rmdir --ignore-fail-on-non-empty $(OUT)

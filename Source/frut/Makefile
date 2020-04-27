UNITTEST_OBJECTS := $(patsubst %.h,%.cpp,$(wildcard unittest/*.h))
UNITTEST := unittest/unittest.cpp

.PHONY: default clean unittest


default : unittest


unittest : $(UNITTEST_OBJECTS) \
           $(UNITTEST)
	@echo "Done."
	@echo ""


$(UNITTEST_OBJECTS) : %.cpp : %.h
	@cxxtestgen --part --runner=ErrorPrinter --have-eh -o "$@" "$<"
	@echo "Created frut/$@"


$(UNITTEST) : $(UNITTEST_OBJECTS)
	@cxxtestgen --root --runner=ErrorPrinter --have-eh -o "$@"
	@echo "Created frut/$@"


clean :
	@rm -f unittest/*.cpp

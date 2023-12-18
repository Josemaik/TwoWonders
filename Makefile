APP		  := ZeldaWonders
CC        :=   g++
CCFLAGS   := -std=c++23 -Wall -Wpedantic -Wextra -Wconversion -Isrc/
LIBS      := -lraylib
SANITIZE  := -fsanitize=address,undefined

MKDIR      := mkdir -p
SRC  	   := src
OBJ  	   := obj
RELEASE    := release
ASSETS     := assets
LIBS_DIR   := libs
RAYLIB_LIB := /usr/lib/libraylib.so

ALLCPP       := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ    := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS      := $(shell find $(SRC) -type d)
OBJSUBDIRS   := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

DATE     := $(shell date +'%d-%m-%y')
ZIP_NAME := $(APP)_$(RELEASE)_$(DATE).zip

$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE)

%.o : %.cpp
	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) -g $(SANITIZE)

$(RELEASE) : $(APP) $(ASSETS)
	$(MKDIR) $(RELEASE)
	cp $(APP) $(RELEASE)/
	cp -r $(ASSETS) $(RELEASE)/
	$(MKDIR) $(RELEASE)/$(LIBS_DIR)
	cp $(RAYLIB_LIB) $(RELEASE)/$(LIBS_DIR)
	zip -r $(ZIP_NAME) $(RELEASE)/
	make clean

$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)

$(ASSETS): 
	$(MKDIR) $(ASSETS)
	
.PHONY : dir clean game

game: $(APP)
	./$(APP)

dir:
	$(info $(ASSETS))
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLCPPOBJ))

clean:
	rm -r ./$(OBJ)/ ./$(APP) ./$(RELEASE)/
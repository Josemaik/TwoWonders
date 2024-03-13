APP		   := TwoWonders
CCACHE 	   := 
CC         := g++
C		   := gcc
CCFLAGS    := -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -Isrc/ -static-libstdc++ -static-libgcc

LIBS       := -L./libs libs/libraylib.a -L./ fmod.dll fmodstudio.dll -lwinmm -lgdi32

SANITIZE   :=

# agregar g++ | clang++

MKDIR      := mkdir -p
SRC  	   := src
OBJ  	   := obj
RELEASE    := release
ASSETS     := assets
LIBS_DIR   := libs
LIBS_COPY  := /usr/lib/libraylib.so.420 libs/raygui.so fmodlibs/libfmod.so.13 fmodlibs/libfmodstudio.so.13

ALLCPP     := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ  := $(patsubst %.cpp,%.o,$(ALLCPP))
ALLC 	   := $(shell find $(SRC) -type f -iname *.c)
ALLCOBJ    := $(patsubst %.c,%.o,$(ALLC))
SUBDIRS    := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

DATE       := $(shell date +'%d-%m-%y')
ZIP_NAME   := $(APP)_$(RELEASE)_$(DATE).zip

# Variables si es make release
ifeq ($(filter release,$(MAKECMDGOALS)),release)
    SANITIZE :=
    CCFLAGS  += -O3 -DNDEBUG
else
    CCFLAGS  += -g
endif

# Regla principal (enlazado de los .o)
$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ) $(ALLCOBJ)
	$(CCACHE) $(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCOBJ)) $(LIBS) $(SANITIZE) -Wl,-rpath=libs -Wl,-rpath,./fmodlibs

# Regla que compila los .cpp
%.o : %.cpp
	$(CCACHE) $(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) $(SANITIZE)

%.o : %.c
	$(CCACHE) $(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $(CCFLAGS) $^

# Regla que crea una release
$(RELEASE) : $(APP) $(ASSETS)
	$(MKDIR) $(RELEASE)
	cp $(APP) $(RELEASE)/
	cp -r $(ASSETS) $(RELEASE)/
	$(MKDIR) $(RELEASE)/$(LIBS_DIR)
	cp $(LIBS_COPY) $(RELEASE)/$(LIBS_DIR)
	zip -r $(ZIP_NAME) $(RELEASE)/

# Reglas auxiliares que crean carpetas

$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)

$(ASSETS):
	$(MKDIR) $(ASSETS)

# Otras reglas

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
	rm -r ./$(OBJ)/ ./$(APP) ./$(RELEASE)/ ./$(ZIP_NAME)

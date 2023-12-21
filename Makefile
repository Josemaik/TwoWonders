APP		   := ZeldaWonders
CC         := ccache g++
CCFLAGS    := -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -Isrc/
LIBS       := -lraylib
SANITIZE   := -fsanitize=address,undefined

# agregar g++ | clang++

# libs/libraylib.a
# LD_LIBRARY_PATH=libs ./ZeldaWonders

MKDIR      := mkdir -p
SRC  	   := src
OBJ  	   := obj
RELEASE    := release
ASSETS     := assets
LIBS_DIR   := libs
LIBS_COPY  := /usr/lib/libraylib.so.450 /usr/lib/libstdc++.so.6 /usr/lib/libc.so.6 /usr/lib/libm.so.6 /usr/lib/libgcc_s.so.1

ALLCPP     := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ  := $(patsubst %.cpp,%.o,$(ALLCPP))
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
$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE) -Wl,-rpath=$(LIBS_DIR) 

# Regla que compila los .cpp
%.o : %.cpp
	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) $(SANITIZE)

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
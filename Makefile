APP		   	:= TwoWonders
CCFLAGS    	:= -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -Isrc/

ifeq ($(OS),Windows_NT)
	CC 	   		:= g++
	CCACHE 	   	:=
    LIBS 	   	:= -L./ libs/raylib.dll libs/raygui.dll libs/fmod.dll libs/fmodstudio.dll -lwinmm -lgdi32
	SANITIZE   	:=
	LIBS_COPY  	:= libs/raylib.dll libs/raygui.dll libs/fmod.dll libs/fmodstudio.dll libs/libstdc++-6.dll libs/libgcc_s_seh-1.dll libs/libwinpthread-1.dll
else
	CC 		   	:= g++-12
	CCACHE 	   	:= ccache
    LIBS 		:= -lraylib -L./fmodlibs -lfmod -lfmodstudio libs/raygui.so
	SANITIZE   	:= -fsanitize=address,undefined
	LIBS_COPY  	:= /usr/lib/libraylib.so.420 libs/raygui.so fmodlibs/libfmod.so.13 fmodlibs/libfmodstudio.so.13
endif

# agregar g++ | clang++

MKDIR      	:= mkdir -p
SRC  	   	:= src
OBJ  	   	:= obj
RELEASE    	:= release
ASSETS     	:= assets
LIBS_DIR   	:= libs

ALLCPP     	:= $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ  	:= $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS    	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

DATE       	:= $(shell date +'%d-%m-%y')
ZIP_NAME   	:= $(APP)_$(RELEASE)_$(DATE).zip


# Variables si es make release
ifeq ($(filter release,$(MAKECMDGOALS)),release)
    SANITIZE :=
    CCFLAGS  += -O3 -DNDEBUG
else
    CCFLAGS  += -g
endif

# Regla principal (enlazado de los .o)
$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CCACHE) $(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE) -Wl,-rpath=libs -Wl,-rpath,./fmodlibs

# Regla que compila los .cpp
%.o : %.cpp
	$(CCACHE) $(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) $(SANITIZE)

# Regla que crea una release
$(RELEASE) : $(APP) $(ASSETS)
	$(MKDIR) $(RELEASE)
	cp $(APP) $(RELEASE)/
	cp -r $(ASSETS) $(RELEASE)/
ifeq ($(OS),Windows_NT)
	cp $(LIBS_COPY) $(RELEASE)/
else
	$(MKDIR) $(RELEASE)/$(LIBS_DIR)
	cp $(LIBS_COPY) $(RELEASE)/$(LIBS_DIR)
endif
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

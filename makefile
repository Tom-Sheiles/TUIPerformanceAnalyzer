
CC: cl
INTERM: interm

SRCS:${wildcard ./*.c}

INCLUDE: $(USERPROFILE)/Files/Programming/Projects/cConsoleLib
CFLAGS: /Zi /EHsc /nologo /Fo$(INTERM)/
INC: /I $(INCLUDE)
BIN: /Builds/PerfAn.exe
BINF: /Fe: $(BIN)
LIBS: User32.lib


all: $(BIN)


$(BIN):
	$(CC) $(CFLAGS) $(INC) $(BINF) $(SRCS) $(LIBS)
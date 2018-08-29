MT = mt

CFLAGS  = /nologo /Zi /W3 /MP /D_CRT_SECURE_NO_WARNINGS
LDFLAGS = /nologo /Zi
LDLIBS  = user32.lib comctl32.lib advapi32.lib
MTFLAGS = /nologo

TARGET   = bootto-gui.exe
SOURCES  = *.c ..\common\*.c
HEADERS  = ..\common\*.h
MANIFEST = bootto-gui.exe.manifest

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) $(MANIFEST)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) $(LDLIBS)
	$(MT) $(MTFLAGS) -manifest $(MANIFEST) -outputresource:$(TARGET);1

clean: .phony
	if exist $(TARGET) del $(TARGET)
	if exist *.obj del *.obj
	if exist *.pdb del *.pdb
	if exist *.ilk del *.ilk

.phony:

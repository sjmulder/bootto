MT = mt

CFLAGS  = /nologo /Zi /W3 /MP /D_CRT_SECURE_NO_WARNINGS
LDFLAGS = /nologo /Zi
LDLIBS  = advapi32.lib
MTFLAGS = /nologo

TARGET   = bootto.exe
OBJECTS  = bootto.obj os.obj efi.obj util.obj
MANIFEST = bootto.exe.manifest

all: $(TARGET)

$(TARGET): $(OBJECTS) $(HEADERS) $(MANIFEST)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS)
	$(MT) $(MTFLAGS) -manifest $(MANIFEST) -outputresource:$(TARGET);1

clean: .phony
	if exist $(TARGET) del $(TARGET)
	if exist *.obj del *.obj
	if exist *.pdb del *.pdb
	if exist *.ilk del *.ilk

bootto.obj: bootto.h
os.obj:     bootto.h
efi.obj:    bootto.h
util.obj:   bootto.h

.phony:

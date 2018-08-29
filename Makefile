MKFLAGS = /nologo

all: .phony
	cd bootto && $(MAKE) $(MKFLAGS)

clean: .phony
	cd bootto && $(MAKE) $(MKFLAGS) clean

.phony:

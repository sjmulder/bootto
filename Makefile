MKFLAGS = /nologo

all: .phony
	cd bootto     && $(MAKE) $(MKFLAGS)
	cd bootto-gui && $(MAKE) $(MKFLAGS)

clean: .phony
	cd bootto     && $(MAKE) $(MKFLAGS) clean
	cd bootto-gui && $(MAKE) $(MKFLAGS) clean

.phony:

Targets := x235 x249 x2232 x2252 x2410 x2416 g2553

Lib_dirs := $(wildcard $(addsuffix /lib,$(Targets)))

#$(Lib_dirs):
#	$(MAKE) -j 2 -C $@ Lib


Clean: $(Lib_dirs:=.clean)

%.clean:
	if [ -d $* ]; then \
	$(MAKE) -C $* Clean ; \
	fi;

Lib: $(Lib_dirs:=.build)

%.build:
	$(MAKE) -C $* Lib


.PHONY: Lib Clean

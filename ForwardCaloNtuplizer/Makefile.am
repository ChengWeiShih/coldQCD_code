AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64

pkginclude_HEADERS = \
  ForwardCaloNtuplizer.h

lib_LTLIBRARIES = \
  libForwardCaloNtuplizer.la

libForwardCaloNtuplizer_la_SOURCES = \
  ForwardCaloNtuplizer.cc

libForwardCaloNtuplizer_la_LIBADD = \
  -lphool \
  -lffarawobjects \
  -lfun4all \
  -lcalo_io \
  -lcalo_reco \
  -lSubsysReco


BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libForwardCaloNtuplizer.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)

/* Configuration for an i386 running GNU with ELF as the target machine.  */

/* This does it mostly for us.  */
#include <i386/linux.h>

/* Get machine-independent configuration parameters for the GNU system.  */
#include <gnu.h>

#undef TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (i386 GNU)");

#undef CPP_PREDEFINES
#define CPP_PREDEFINES "-D__ELF__ -D__i386__ -DMACH -Asystem(mach) \
  -Dunix -Asystem(unix) -Asystem(posix) -D__GNU__ -Asystem(gnu)"

#undef CPP_SPEC
#define CPP_SPEC "%(cpp_cpu) %{posix:-D_POSIX_SOURCE} %{bsd:-D_BSD_SOURCE}"

#undef CC1_SPEC
#define CC1_SPEC "%(cc1_cpu)"

#undef	LINK_SPEC
#define LINK_SPEC "-m elf_i386 %{shared:-shared} \
  %{!shared: \
    %{!static: \
      %{rdynamic:-export-dynamic} \
      %{!dynamic-linker:-dynamic-linker /lib/ld.so}} \
    %{static:-static}}"

#undef	STARTFILE_SPEC
#define STARTFILE_SPEC \
  "%{!shared: \
     %{!static: \
       %{pg:gcrt1.o%s} %{!pg:%{p:gcrt1.o%s} %{!p:crt1.o%s}}} \
     %{static:crt0.o%s}} \
   crti.o%s %{!shared:crtbegin.o%s} %{shared:crtbeginS.o%s}"

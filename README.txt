Analyzing with diffMu:

0. Unpack from diffMu.tgz in a G4 working directory, e.g., g4work. This
creates the subdirectory g4work/diffMu where this readme sits. (You must
have done this already if you're reading this.)

1. Build executable in diffMu-build according to G4 instructions
in cmake_diffMu.txt, using cmake and make commands. Then make
sure you are still in g4work/diffMu-build subdirectory.

2. Test with:
   ./diffMu -h, to see command line input options
   ./diffMu, for visualization and interactive mode
   ./diffMu -n 1000 -r 101 2>&1 |tee r101.log, to run 1000
   events, save histograms to a root file, and save a log file

3. In diffMu directory, modify input macro files beam.mac and
target.mac as desired, such as by copying other beam-*.mac and
target-*.mac to those files. Run "make" again from diffMu-build
to copy them to the build directory.

3. Run g4run_many.sh from build directory. As of this writing, it
takes parameters of the initial run number in the sequence and
number of events for each run, e.g.,
"./g4run_many.sh 150 100000".
It copies its output .root and puts its .log files to
diffMu-output, which must exist.

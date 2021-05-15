#!/bin/sh

# g4run_many.sh

. ~/bin/g4v10_setup.sh

g4command="./diffMu"
runmin=$1
nevents=$2

outbasename="density_scan"
#densarray=( 26.5  50. )
densarray=( 20. 50. 100. 200. 500. 1000. 2000. )

#phiarray=( 0.03 0.10 )
#pitchtophi=2.0
#pitcharray=( 0.20 )
#deptharray=( 1.0 3.0 )
#pitcharray=( 0.2 0.3 0.4 )
#deptharray=( 0.100 0.300 1.0 3.0 9.6 )




basedir=`pwd`
echo "running from directory" ${basedir}
#logdir=${basedir/"-build"/"-output/"}
logdir="../diffMu-output"

if [ -d ${logdir} ]; then
    echo "Writing G4 log files to " ${logdir}
else
    echo " ERROR: Could not find log file directory " ${dir}
    echo " "
    exit 1
fi

irun=$((runmin))

for idens in ${densarray[@]}; do
    echo "Run: " ${irun} "at density" ${idens}
    commandpars="-n ${nevents} -den ${idens} -r ${irun} -dir ${logdir} -log ${outbasename}"
    sdens=`echo "10*${idens}/1" |bc`
    filenamestring="${logdir}/${outbasename}_n${nevents}_den%05d_r${irun}.log"
    outputlog=`printf ${filenamestring} ${sdens}`
    echo "Command line: ${g4command} ${commandpars} 2>&1 |tee ${outputlog}"
    ${g4command} ${commandpars} 2>&1 |tee ${outputlog}
    irun=$((irun+1))
done


##### the script segment below is obsolete, but provides a guide for more complicated multi-run analysis     
##for iphi in ${phiarray[@]}; do
##    sphi=`echo "1000*${iphi}/1" |bc`
##    ipit=`echo "${pitchtophi}*${iphi}" |bc`   # uses ratio of pitch to diameter
##    spit=`echo "1000*${ipit}/1" |bc`
##    for idep in ${deptharray[@]}; do
##	sdep=`echo "1000*${idep}/1" |bc`
##	filenamestring="R%04d_"${g4command}"_%d_PHI%04d_PIT%04d_DEP%04d"
##	filename=`printf ${filenamestring} ${irun} ${nevents} ${sphi} ${spit} ${sdep}`
##	rootfile=${filename}".root"
##	logfile=${logdir}${filename}".log"
##	echo "Run: " ${irun}
##	echo "Rootfile:" ${rootfile}
##	echo "Logfile:" ${logfile}
##	if [ -e ${logfile} ]; then
##	    echo "==> Logfile already exists: exiting"
##	    echo " "
##	    exit 1
##  	fi
##	(run G4 here)
##    done
##done

echo " "
exit 0


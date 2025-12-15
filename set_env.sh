module purge
module use /group/halla/modulefiles
module use /group/nps/modulefiles
module load nps_replay/5.28.24

setenv ROOTSYS /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/root/6.30.04-gcc11.4.0
setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${LD_LIBRARY_PATH}
setenv PATH ${ROOTSYS}/bin:${PATH}
setenv PYTHONPATH ${ROOTSYS}/lib:${PYTHONPATH}

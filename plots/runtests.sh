#!/bin/bash

# $3: environments (i.e. cuda, omp )
# $2: platform (i.e. gpu model, a100 or l4 )
# $3: device index (0,1,...)
 

script="$(dirname "$(readlink -f -- "$0")")/plots.py"
path="$(dirname "$(readlink -f -- "$0")")/"
nrun=3
environments=$1
platform=$2
device_nb=$3
output="$(date "+%y%m%d-%H%M%S")"
declare -a sycl_mms=("BUF" "PTR")
declare -a tests=("InvariantMasses" "Boost")


for e in "$environments"
do      
    for s in ${sycl_mms[@]}
    do
        for t in ${tests[@]}
        do
            echo "ACPP_VISIBILITY_MASK=${e} ONEAPI_DEVICE_SELECTOR=${e}:${device_nb}  python3 $script ${path} ${t} ${platform} ${e} ${device_nb} ${s} $nrun $output"
            # ACPP_VISIBILITY_MASK=${e} ONEAPI_DEVICE_SELECTOR=${e}:${device_nb} 
            python3 $script ${path} ${t} ${platform} ${e} ${device_nb} ${s} $nrun $output
        done
    done
done


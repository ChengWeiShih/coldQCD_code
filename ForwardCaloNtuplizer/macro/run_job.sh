#! /bin/bash
# export USER="$(id -u -n)"
# export LOGNAME=${USER}
# export HOME=/sphenix/u/${LOGNAME}

if [ ! -f "$3" ]; then
  echo "input file $3 does not exist"
  exit 1
fi

source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.427

export MYINSTALL=/sphenix/user/ChengWei/build_sPH_coresoftware/spin_build/ForwardCaloNtuplizer_build/install
export LD_LIBRARY_PATH=$MYINSTALL/lib:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=$MYINSTALL/include:$ROOT_INCLUDE_PATH

source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

echo $ROOT_INCLUDE_PATH | sed -e "s/:/\n/g"

# print the environment - needed for debugging
# printenv
# this is how you run your Fun4All_G4_sPHENIX.C macro in batch:
root.exe -q -b Fun4All_ForwardCalo.C\($1,$2,\"$3\",\"$4\",\"$5\"\)

echo all done

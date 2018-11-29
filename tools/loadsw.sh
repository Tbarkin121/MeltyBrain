#!/bin/bash
# ******************************************************************************
# @file    ctswload.sh
# @author  Bob Gatewood
# @date    Spring 2016
# @brief   Loads Clinical Tiffin software onto device
# ******************************************************************************
# @attention
#
# COPYRIGHT © Portal Instruments
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#   1. Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#   3. Neither the name of Portal/Continuum nor the names of its contributors
#      may be used to endorse or promote products derived from this software
#      without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ******************************************************************************
#
# Loads binary onto target, either clinical-tiffin or ct-dev
# Default location of binary is ../build/clinicaltiffin

usage() { echo "Usage: $0 [-b /path/to/binary] [-h]
Where
   -b /path/to/binary will load a binary at any path
        (default is $project_root/build/clinicaltiffin)
   -h prints this

Examples:
  $0 -b /other/ff/binary
  $0 -h
"; exit 1; }

script_dir=$(dirname $0)
project_root=$( cd $script_dir; cd ..; pwd -P )
cd $script_dir
binary_path="$project_root/build/FieldOrientedControl"
openocd_config="openocd.config"
target_name="production"
post_program_commands=" -c 'resume' -c 'shutdown'"

while getopts db: o; do
  case "${o}" in
    b) # non-default binary path
      binary_path=${OPTARG}
      if [[ $binary_path == *"_test_hw"* ]]; then
        openocd_config="openocd.hwtest.config"
        post_program_commands=" -c 'arm semihosting enable'"
        post_program_commands+=" -c 'reset run' -c 'wait_halt 750000'"
        post_program_commands+=" -c 'shutdown'"
      fi
      ;;

    *)
      usage
      ;;
  esac
done
shift $((OPTIND-1))

openocd_command="sudo openocd"
openocd_command+=" -s \"$script_dir\" -f \"/vagrant/tools/openocd.cfg\""
openocd_command+=" -c \"program $binary_path verify\""
openocd_command+="$post_program_commands"
eval $openocd_command

retstat=$?
if [ $retstat == 0 ]; then
  echo "*** Loaded $binary_path onto $target_name board"
else
  echo "*** FAILED  Load of $binary_path onto $target_name did not succeed. Error return $retstat."
fi

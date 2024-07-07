#!/bin/bash

if [ ! -f "/usr/bin/cppcheck" ];
then
    echo "cppcheck is not installed";
    exit 0;
else
    echo "cppcheck has been installed";
fi

date > cppcheck.log
c_path=build_dir/target*
#d_path=build_dir/target*/linux*

#cppcheck --enable=all --suppress=missingIncludeSystem $c_path/dalcmd* $c_path/fw_upgrade* $c_path/libzcfg_dbg* $c_path/libzcfg_fe_rdm_access* $c_path/libzcfg_fe_rdm_string* $c_path/libzcfg_fe_rdm_struct* $c_path/libzcfg_fe_schema* $c_path/libzcmd_tool* $c_path/libzlog* $c_path/ztr69cmd* $c_path/zyMAPLog* $c_path/zyMAPSteer* $c_path/zyschedule* $c_path/zycfgfilter* 2> cppcheck.log 1> /dev/null 

filename='scripts/cppcheck_pk_name.list'
exec < $filename

while read line
do
    echo $line # 一行一行印出內容
    #cppcheck --enable=all --suppress=missingIncludeSystem $c_path/$line 2>> cppcheck.log 1> /dev/null
    cppcheck --enable=all --suppress=missingIncludeSystem --inline-suppr $c_path/$line --suppress=*:*ZyIMS*/port/ext_phone* --suppress=*:*ZyIMS*/port/platform/*/dsp_phone_api* 2>> cppcheck.log 1> /dev/null
done

grep error: cppcheck.log > error_cppcheck.log

cat error_cppcheck.log

date >> cppcheck.log

if grep -q error: cppcheck.log;
then
    if [ "$1" == "cppenable" ]; then
      echo "cppcheck fail";
      exit -1;
    elif [ "$1" == "cppdisable" ]; then
      echo "cppcheck disable";
      exit 0;
    fi
else
    echo "cppcheck pass";
    exit 0;
fi


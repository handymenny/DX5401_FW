#!/bin/bash

src=$1
dst=$2

if [ -d $dst ] ; then
	echo "$dst exist. Please try another one"
	exit 1
fi
cur_pwd=`pwd`
mkdir $dst

cd $src
dirs=`find . -maxdepth 1 -type d | grep -v .git`

for dir in $dirs 
do
	if [ $dir == '.' ] || [ $dir == './build_dir' ] || [ $dir == './bin' ] \
		|| [ $dir == './tmp' ] || [ $dir == './staging_dir' ] || [ $dir == './dl' ]; then
		continue
	fi
	
	echo " ===cp -rf $dir $cur_pwd/$dst"
	cp -rf $dir $dst
done

../.zyrepo/gpl.sh $dst

cp *.* $dst
cp * $dst
cp .config $dst

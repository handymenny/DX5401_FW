ReadMe for V5.17(ABYO.4)C0


0. Introduction

  This file will show you how to build the DX5401-B0 series linux system,
  please note, the download image will overwrite the original image existed in the flash memory of EV board.

1. Prepare the build OS.

   We suggest using the Linux distribution to setup your environment for reduce compatible issue.

    Install ubuntu-16.04 LTS https://releases.ubuntu.com/16.04.7/ubuntu-16.04.7-desktop-amd64.iso


2. Prepare the compiler environment

   Install the following necessary tools 

    $ sudo apt-get update

    $ sudo apt-get upgrade

    $ sudo apt-get install bash

    $ sudo apt-get install build-essential subversion git-core libncurses5-dev

    $ sudo apt-get install zlib1g-dev gawk flex quilt libssl-dev xsltproc libxml-parser-perl mercurial bzr ecj cvs libglib2.0-dev

    $ sudo apt-get install autoconf automake

    $ sudo update-alternatives --install /bin/sh sh /bin/bash 100

    $ sudo add-apt-repository ppa:git-core/ppa 

    $ sudo apt-get update 

    $ sudo apt-get install git -y

    $ sudo add-apt-repository ppa:deadsnakes/ppa

    $ sudo apt-get update

    $ sudo apt-get install python3.5 python3.5-dev python3.5-gdbm uuid-dev

    $ sudo apt-get update

    $ cd /usr/bin

    $ sudo rm python3

    $ sudo ln -s python3.5 python3

    $ sudo apt-get install bc libc6-dev-i386 curl -y

    $ sudo curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -

    $ sudo apt-get update

    $ sudo apt-get install -y  --force-yes nodejs --fix-missing


4. Build the firmware for Web-GUI upgrade using

   Clean and build the firmware

	$ make dirclean

	$ make


6. The image is in the images directory.

    The firmware image will locate at bin/brcm963xx-glibc/zyxel_pureUbi/ras.bin (for ABXA use bin/brcm963xx-glibc/zyxel/ras.bin)
    
    You can use it update DX5401-B0 series by using the firmware update procedure.

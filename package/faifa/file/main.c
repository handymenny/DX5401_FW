/*
 *  Program entry and command line parsing
 *
 *  Copyright (C) 2007-2008 Xavier Carcelle <xavier.carcelle@gmail.com>
 *                  Florian Fainelli <florian@openwrt.org>
 *              Nicolas Thill <nico@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 *  In addition, as a special exception, the copyright holders give
 *  permission to link the code of portions of this program with the
 *  OpenSSL library under certain conditions as described in each
 *  individual source file, and distribute linked combinations
 *  including the two.
 *  You must obey the GNU General Public License in all respects
 *  for all of the code used other than OpenSSL.  If you modify
 *  file(s) with this exception, you may extend this exception to your
 *  version of the file(s), but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source
 *  files in the program, then also delete it here.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
#include <sys/select.h>
#include <sys/un.h>
#endif
#include <pthread.h>
#include "faifa.h"
#include "faifa_compat.h"
#include "faifa_priv.h"
#ifndef FAIFA_PROG
#define FAIFA_PROG "faifa"
#endif

/* Command line arguments storing */
int opt_help = 0;
int opt_interactive = 0;
int opt_key = 0;
extern FILE *err_stream;
extern FILE *out_stream;
extern FILE *in_stream;
#ifdef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
pthread_mutex_t filelck = PTHREAD_MUTEX_INITIALIZER;
#define FAIFA_CHANNEL_PATH "/tmp/faifa_chan"
#endif
/**
 * error - display error message
 */
static void error(char *message)
{
    fprintf(stderr, "%s: %s\n", FAIFA_PROG, message);
}

/**
 * usage - show the program usage
 */
static void usage(void)
{
	fprintf(stderr, "-i : interface\n"
			"-m : show menu (no option required)\n"
			"-a : station MAC address\n"
			"-k : network key\n"
			"-v : be verbose (default: no)\n"
			"-e : error stream (default: stderr)\n"
			"-o : output stream (default: stdout)\n"
			"-s : input stream (default: stdin)\n"
			"-h : this help\n");
}
#ifdef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
void periodTask() 
{
    struct timeval tv, tv2;
    unsigned long long start_time, end_time;
    float total_time;
    int ret = 0,i,retry=0;
#ifdef SUPPORT_THREE_LAN_PORT //__MSTC__, Bruce for 3 LAN + 1 WAN
	faifa_t *faifa[3];
	char * LanIfname[] = {"eth1", "eth2", "eth3"};
#else
    faifa_t *faifa[4];
    char * LanIfname[] = {"eth0", "eth1", "eth2", "eth3"};
#endif
    u_int8_t opt_srcmacaddr[ETHER_ADDR_LEN] = {0};
    u_int8_t srcaddr[ETHER_ADDR_LEN] = {0};

    unlink(HPAV_RES_FILE);

#ifdef SUPPORT_THREE_LAN_PORT //__MSTC__, Bruce for 3 LAN + 1 WAN
    for(i=0; i<3;i++) {
#else
	for(i=0; i<4;i++) {
#endif
        faifa[i] = faifa_init();
        if (faifa == NULL) {
            error("can't initialize Faifa library");
            return ;
        }

        strncpy(faifa[i]->ifname,LanIfname[i],4);
        if (faifa_open(faifa[i], LanIfname[i]) == -1) {
            //error(faifa_error(faifa[i]));
            faifa_free(faifa[i]);
            return ;
        }

        faifa_get_hwaddr(faifa[i], srcaddr);
        if(strlen(srcaddr)>0)
            faifa_set_src_addr(faifa[i], srcaddr);
            menu(faifa[i]);
    }
	while((pthread_mutex_trylock(&filelck)!=0) && retry<3) {
		nanosleep((struct timespec[]){{0, 300000000}}, NULL);
		retry ++;
	}
		pthread_mutex_unlock(&filelck);
		if(retry < 3)
	    	nanosleep((struct timespec[]){{0, 500000000}}, NULL);
out_error:
#ifdef SUPPORT_THREE_LAN_PORT //__MSTC__, Bruce for 3 LAN + 1 WAN
	for(i=0; i<3;i++) {
#else
    for(i=0; i<4;i++) {
#endif
        ret = cancel_thread(faifa[i]);
        if(ret !=0) {
            faifa_printf(out_stream, "Closing receive thread fail,%d\n",i);
        }

        faifa_close(faifa[i]);
        faifa_free(faifa[i]);
    }

}

int d_main(){
    int n, maxfd=0,rfd;
	socklen_t srv_len, cli_len;
	struct sockaddr_un srv_addr, cli_addr;
    fd_set readfds, readfd;
    FD_ZERO(&readfds);
    struct timeval tm;
    struct timeval tv, tv2;
    unsigned long long start_time, end_time;
    tm.tv_sec = 15;
    tm.tv_usec = 0;

	rfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(rfd < 0) {
		rfd = -1;
		faifa_printf(out_stream, "socket error\n");
	}
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, FAIFA_CHANNEL_PATH, sizeof(FAIFA_CHANNEL_PATH));
	srv_len = sizeof(srv_addr);
	if (bind(rfd, (struct sockaddr*)&srv_addr, srv_len)<0) {
		faifa_printf(out_stream, "bind error\n");
	}
	listen(rfd, 1);
	FD_SET(rfd, &readfds);
	maxfd = rfd;
    while(1) {
		readfd = readfds;
        n = select(maxfd+1, &readfd, NULL, NULL, &tm);
        switch(n) {
            case 0:
                gettimeofday(&tv,NULL);
                start_time = tv.tv_sec * 1000000 + tv.tv_usec;
                periodTask();
                gettimeofday(&tv2,NULL);
                end_time = tv2.tv_sec * 1000000 + tv2.tv_usec;
                tm.tv_sec = 0;
                tm.tv_usec = 15000000-(end_time-start_time);
                break;
            default:
				usleep(300000);
				if(FD_ISSET(rfd, &readfds)) {
					gettimeofday(&tv,NULL);
	                start_time = tv.tv_sec * 1000000 + tv.tv_usec;
					accept(rfd,(struct sockaddr *)&cli_addr, &cli_len);
					periodTask();
	                gettimeofday(&tv2,NULL);
    	            end_time = tv2.tv_sec * 1000000 + tv2.tv_usec;
    	            tm.tv_sec = 0;
	                tm.tv_usec = 15000000-(end_time-start_time);

					
				}
                break;
        }
            
    }
	close(rfd);
    return 0;
}
#endif
extern void menu(faifa_t *faifa);
extern void set_key(char *macaddr);
extern void cancel_thread(faifa_t *faifa);

/**
 * main - main function of faifa
 * @argc:   number of arguments
 * @argv:   array of arguments
 */
int main(int argc, char **argv)
{
    struct timeval tv, tv2;
    unsigned long long start_time, end_time;
    float total_time;
    faifa_t *faifa;
    char *opt_ifname = NULL;
    char *opt_macaddr = NULL;
    char *opt_err_stream = NULL;
    char *opt_out_stream = NULL;
    char *opt_in_stream = NULL;
    int opt_verbose = 0;
    int c;
    int ret = 0;
    u_int8_t addr[ETHER_ADDR_LEN] = { 0 };

	#ifdef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
	fprintf(stderr, "Faifa for HomePlug AV\n\n");
	#else
    fprintf(stderr, "Faifa for HomePlug AV (SVN revision %d)\n\n", SVN_REV);
	#endif
	#ifdef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
	unlink(FAIFA_CHANNEL_PATH);
    unlink(HPAV_RES_FILE);
	#endif
    if (argc < 2) {
        usage();
        return -1;
    }

    while ((c = getopt(argc, argv, "i:ma:k:ve:o:s:h")) != -1) {
        switch (c) {
            case 'i':
                opt_ifname = optarg;
                break;
            case 'm':
                opt_interactive = 1;
                break;
            case 'a':
                opt_macaddr = optarg;
                break;
            case 'k':
                opt_key = 1;
                break;
            case 'v':
                opt_verbose = 1;
                break;
            case 'e':
                opt_err_stream = optarg;
                break;
            case 'o':
                opt_out_stream = optarg;
                break;
            case 's':
                opt_in_stream = optarg;
                break;
            case 'h':
            default:
                opt_help = 1;
                break;
        }
    }

    if (opt_help) {
        usage();
        return -1;
    }
	#ifndef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
    if (opt_ifname == NULL)
		opt_ifname = "eth0";
	#endif
    if (opt_err_stream == NULL)
        err_stream = stderr;
    else {
        err_stream = fopen(opt_err_stream, "w+");
        if (!err_stream) {
            perror("err_stream");
            return -1;
        }
    }

    if (opt_out_stream == NULL)
        out_stream = stdout;
    else {
        out_stream = fopen(opt_out_stream, "w+");
        if (!out_stream) {
            perror("out_stream");
            return -1;
        }
    }

    if (opt_in_stream == NULL)
        in_stream = stdin;
    else {
        in_stream = fopen(opt_in_stream, "rb");
        if (!in_stream) {
            perror("in_stream");
            return -1;
        }
    }
	#ifdef ZYXEL_HOMEPLUG_DEVICE_DISCOVER
	d_main();
	return ret;
	#endif
	faifa = faifa_init();
	if (faifa == NULL) {
            error("can't initialize Faifa library");
            return -1;
        }

	if (faifa_open(faifa, opt_ifname) == -1) {
		error(faifa_error(faifa));
		faifa_free(faifa);
		return -1;
	}

	faifa_set_verbose(faifa, opt_verbose);

	if (opt_macaddr) {
		ret = faifa_parse_mac_addr(faifa, opt_macaddr, addr);
		if (ret < 0) {
			error(faifa_error(faifa));
			goto out_error;
		}

		faifa_set_dst_addr(faifa, addr);
	}

	if (opt_interactive)
		menu(faifa);

out_error:
	faifa_close(faifa);
	faifa_free(faifa);

	return ret;
}

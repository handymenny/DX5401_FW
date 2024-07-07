#!/bin/sh

set -e
p_zyxel=package/private/zyxel
pu_zyxel=package/public-zyxel

rm -r $p_zyxel/acts_script/acts
rm -r $p_zyxel/cdbg/patches-1.0
rm -r $p_zyxel/dalcmd/patches
mv  $p_zyxel/data_model_show/src $p_zyxel/data_model_show/datamodel_show-1.0
rm -r $p_zyxel/esmd/patches
rm -r $p_zyxel/fw_upgrade/patches
rm -r $p_zyxel/hostpolling/patches-1.0
rm -r $p_zyxel/libzyplatform/src
rm -r $p_zyxel/libzywwan/patches
rm -r $p_zyxel/redirect_console/src
rm -r $p_zyxel/SpeedTest/patches-1.0
rm -r $p_zyxel/tty_log_echo/src
rm -r $p_zyxel/vcautohunt/patches
rm -r $p_zyxel/vlanautohunt/patches
rm -r $p_zyxel/zcfg_be/416L01-patches
rm -r $p_zyxel/zcfg_be/416L02-patches
rm -r $p_zyxel/zcfg_be/416L02A-patches
rm -r $p_zyxel/zcfg_be/Econet-patches
rm -r $p_zyxel/zcfg_be/patches-2.0
rm -r $p_zyxel/zcmd/416L01-patches
rm -r $p_zyxel/zcmd/416L02-patches
rm -r $p_zyxel/zcmd/416L02A-patches
rm -r $p_zyxel/zcmd/Econet-patches
rm -r $p_zyxel/zcmd/patches-2.0
rm -r $p_zyxel/zhttpd/patches
rm -r $p_zyxel/zhttpd_rest/src
rm -r $p_zyxel/zipq/patches
rm -r $p_zyxel/ztr69/patches
rm -r $p_zyxel/zycfgfilter/patches
rm -r $p_zyxel/zycli/patches
rm -r $p_zyxel/zylicense/src
rm -r $p_zyxel/zymultiwan/src

rm -r $pu_zyxel/librest-c/src
rm -r $pu_zyxel/libzcfg_dbg/src
rm -r $pu_zyxel/libzcfg_fe_dal/patches
rm -r $pu_zyxel/libzcfg_fe_rdm_access/patches
rm -r $pu_zyxel/libzcfg_fe_rdm_string/patches
rm -r $pu_zyxel/libzcfg_fe_rdm_struct/patches
rm -r $pu_zyxel/libzcfg_fe_schema/patches
rm -r $pu_zyxel/libzcfg_fe_tr98/patches
rm -r $pu_zyxel/libzcfg_msg/patches
rm -r $pu_zyxel/libzcmd_tool/patches
rm -r $pu_zyxel/libzyuitimer/patches
rm -r $pu_zyxel/libzyutil/patches




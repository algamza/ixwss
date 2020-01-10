//4.1 Network - excel, pdf
//var network_Event_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.event\", \"params\":{\"notification\": \"network.event\"}, \"id\":101}";
var network_saveConfig_JSON  =  "{\"jsonrpc\": \"2.0\", \"method\": \"network.save_config\", \"params\": [],\"id\":40}";
var network_getInterfaceList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_interface_list\", \"params\": {\"host\": \"host-mainfff\"},\"id\":10}";
var network_getInterface_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_interface\", \"params\": {\"id-interface\": 3,\"host\": \"host-main\"},\"id\":12}";
var network_setInterface_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_interface\", \"params\": {\"id-interface\": 3,\"host\": \"host-main\",\"interface\": {\"enable\": true,\"multicast\": true,\"ipv4\": true,\"ipv6\": true}},\"id\":12}";
var network_getAddressList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_address_list\", \"params\": {\"id-interface\": 3,\"host\": \"host-main\"},\"id\":14}";
var network_addAddress_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.add_address\", \"params\": {\"id-interface\": 3,\"host\": \"host-main\",\"address\":{\"enable\": true,\"domain\": \"ipv4\",\"config\": \"static\",\"ip\": \"192.168.1.33\", \"netmask\": \"255.255.255.0\",\"broadcast\": \"192.168.1.255\"}},\"id\":14}";
var network_delAddress_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.del_address\", \"params\": {\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-mainDel\"},\"id\":14}";
var network_setAddress_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_address\",\"params\": {\"id-interface\": 55,\"id-address\": 2,\"host\": \"host-main\",\"address\": {\"enable\": true,\"domain\": \"ipv4\",\"config\": \"static\",\"ip\": \"192.168.5.1\",\"netmask\": \"255.255.255.0\",\"broadcast\": \"192.168.5.255\"}},\"id\":14}";
var network_getDhcpRequestedOptionList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_dhcp_requested_option_list\", \"params\":{\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-main\"}, \"id\":101}";

var network_setDhcpRequestedOptionList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_dhcp_requested_option\", \"params\":{\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-main\", \"option\": {\"enable\": true,\"tag\": 3}}, \"id\":101}";
var network_unsetDhcpRequestedOptionList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.unset_dhcp_requested_option\", \"params\":{\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-main\", \"option\": {\"tag\": 3}}, \"id\":105}";
var network_getDhcpSentOptionList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_dhcp_sent_option_list\", \"params\":{\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-main\"}, \"id\":106}";
var network_setDhcpSentOption_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_dhcp_sent_option\", \"params\":{\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-main\", \"option\": {\"enable\": true,\"tag\": 60,\"value\": \"SAGEMCOMSTB\"}}, \"id\":106}";
var network_unsetDhcpSentOption_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.unset_dhcp_sent_option\", \"params\":{\"id-interface\": 3,\"id-address\": 2,\"host\": \"host-main\", \"option\": {\"tag\": 3}}, \"id\":105}";
var network_getDhcpsConfig_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_dhcps_config\", \"params\":{\"id-interface\": 3,\"host\": \"host-main\"}, \"id\":107}";
var network_setDhcpsConfig_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_dhcps_config\", \"params\":{\"id-interface\": 3,\"host\": \"host-main\",\"dhcps\": {\"enable\": true}}, \"id\":107}";
var network_setDhcpsPool_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_dhcps_pool\", \"params\":{\"id-interface\": 3,\"id-pool\": 1,\"host\": \"host-main\", \"pool\": {\"enable\": true,\"start\": \"192.168.1.100\",\"end\": \"192.168.1.150\", \"options\": [{\"enable\": true,\"tag\": 1,\"value\": \"255.255.255.0\"}, {\"enable\": true,\"tag\": 28,\"value\": \"192.168.1.255\"}, {\"enable\": true,\"tag\": 6,\"value\": \"192.168.1.1\"}]}}, \"id\":107}";
var network_getDhcpsLeaseList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_dhcps_lease_list\", \"params\":{\"id-interface\": 3,\"host\": \"host-main\"}, \"id\":109}";
var network_getDnsServerList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_dns_server_list\", \"params\":{\"host\": \"host-main\"}, \"id\":109}";

var network_addDnsServer_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.add_dns_server\", \"params\":{\"host\": \"host-main\",\"server\": {\"enable\": true,\"server\": \"192.168.1.1\"}}, \"id\":109}";
var network_delDnsServer_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.del_dns_server\", \"params\":{\"id-server\": 3,\"host\": \"host-main\"}, \"id\":109}";
var network_setDnsServer_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_dns_server\", \"params\":{\"id-server\": 4,\"host\": \"host-main\",\"server\": {\"enable\": true,\"server\": \"192.168.1.1\"}}, \"id\":120}";
var network_getRoutesList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_routes_list\", \"params\":{\"host\": \"host-main\"}, \"id\":111}";
var network_addRoute_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.add_route\", \"params\":{\"host\": \"host-main\", \"route\": {\"enable\": true,\"id-interface\": 2,\"destination\": \"0.0.0.0/0\",\"gateway\": \"192.168.1.1\",\"metric\": 0}}, \"id\":113}";
var network_delRoute_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.del_route\", \"params\":{\"id-route\": 2,\"host\": \"host-main\"}, \"id\":113}";
var network_setRoute_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_route\", \"params\":{\"host\": \"host-main\", \"id-route\": 2, \"route\": {\"enable\": true,\"id-interface\": 2,\"destination\": \"5.5.5.5/0\",\"gateway\": \"192.168.1.1\",\"metric\": 0}}, \"id\":115}";
var network_getNtpServerList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_ntp_server_list\", \"params\":{\"host\": \"host-main\"}, \"id\":117}";
var network_addNtpServer_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.add_ntp_server\", \"params\":{\"host\": \"host-main\",\"server\": {\"enable\": true,\"server\": \"192.168.1.1\"}}, \"id\":117}";
var network_delNtpServer_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.del_ntp_server\", \"params\":{\"host\": \"host-main\",\"id-server\": 4}, \"id\":117}";

var network_setNtpServer_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_ntp_server\", \"params\":{\"host\": \"host-main\",\"id-server\": 4,\"server\": {\"enable\": true,\"server\": \"192.168.1.1\"}}, \"id\":117}";
var network_getWirelessStatus_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_wireless_status\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";
var network_startWirelessScan_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.start_wireless_scan\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";
var network_stopWirelessScan_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.stop_wireless_scan\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";
var network_getWirelessScanResult_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_wireless_scan_result\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";
var network_getWirelessType_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_wireless_type\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";
var network_setWirelessType_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_wireless_type\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3,\"type\": \"station\"}, \"id\":117}";
var network_getWirelessProfilList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_wireless_profil_list\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";
var network_addWirelessProfil_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.add_wireless_profil\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3,\"profil\": {\"enable\": true,\"ssid\": \"VSSID3\",\"bssid\": \"00:90:4c:1a:9f:00\",\"key\": \"azerty\"}}, \"id\":117}";
var network_delWirelessProfil_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.del_wireless_profil\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3, \"id-profil\":4}, \"id\":117}";

var network_setWirelessProfil_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_wireless_profil\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3, \"id-profil\":4,\"profil\": {\"enable\": true,\"ssid\": \"VSSID3\",\"key\": \"azerty\"}}, \"id\":117}";
var network_launchWps_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.launch_wps\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3, \"mode\": \"button\",\"bssid\": \"45:78:A2:4B:C5:FF\"}, \"id\":117}";
var network_cancelWps_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.cancel_wps\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";   //not in pdf

// [todo] network_getWirelessApConfig_JSON-making encryption array in json
var network_getWirelessApConfig_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_wireless_ap_config\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";

// [todo] network_setWirelessApConfig_JSON -parsing encryption array in json
var network_setWirelessApConfig_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.set_wireless_ap_config\", \"params\":{\"host\": \"host-main\",\"id-interface\":3,\"settings\":{\"band\":\"2.4GHz\",\"mode\":\"b,g\",\"bandwidth\": 20,\"channel\": 3,\"hidden\": \"no\",\"ssid\": \"VSSID\",\"psk_key\": \"azerty\",\"wep_key\": \"\",\"encryption\": [\"wpa_psk_tkip\",\"wpa2_psk_aes\"]}}, \"id\":117}";

var network_getWirelessApAssocList_JSON = "{\"jsonrpc\": \"2.0\", \"method\": \"network.get_wireless_ap_assoc_list\", \"params\":{\"host\": \"host-main\",\"id-interface\": 3}, \"id\":117}";


//4.2 EIT IPPV - pdf
var getEitIppvInfo = "{\"jsonrpc\": \"2.0\",\"method\": \"get_eit_ippv_info\",\"params\": { \"onid\" : 126,\"tsid\" : 50600, \"sid\" : 2309,\"eit_type\": 2,\"event_id\": 14654}, \"id\": 19}";

//4.3 HDD - pdf
var getHddStatus = "{\"jsonrpc\": \"2.0\",\"method\": \"get_hdd_status\", \"id\": 19}";
//expected full result event json
//var hddEvent  = "{\"jsonrpc\": \"2.0\",\"method\": \"hdd.event\", \"params\": {\"id-hdd\": \"/dev/sda\",\"event\": { \"id-event\": 0, \"name\": \"PLUGGED\" }}}";

//4.4 control setting - pdf
var getPinCodeMode = "{\"jsonrpc\": \"2.0\",\"method\": \"get_pincode_mode\", \"id\": 21}";
var setPinCodeMode = "{\"jsonrpc\": \"2.0\",\"method\": \"set_pincode_mode\", \"params\":{ \"PINCODE_MODE\":\"none\" } , \"id\": 21}";
var getFrontPanelLocked = "{\"jsonrpc\": \"2.0\",\"method\": \"get_front_panel_locked\", \"id\": 21}";
var setFrontPanelLocked = "{\"jsonrpc\": \"2.0\",\"method\": \"set_front_panel_locked\", \"params\":{ \"locked\":1 } ,\"id\": 21}";  //param not defined in document
var getMoralityLevel = "{\"jsonrpc\": \"2.0\",\"method\": \"get_morality_level\", \"id\": 21}";
var setMoralityLevel = "{\"jsonrpc\": \"2.0\",\"method\": \"set_morality_level\", \"params\":{ \"level\":13 } ,\"id\": 21}";
var getShowLockRec = "{\"jsonrpc\": \"2.0\",\"method\": \"get_show_lock_rec\", \"id\": 21}";
var setShowLockRec = "{\"jsonrpc\": \"2.0\",\"method\": \"set_show_lock_rec\", \"params\":{ \"show\":1 } ,\"id\": 21}";
var getUiAutoStandby = "{\"jsonrpc\": \"2.0\",\"method\": \"get_ui_auto_standby\", \"id\": 21}";
var setUiAutoStandby = "{\"jsonrpc\": \"2.0\",\"method\": \"set_ui_auto_standby\", \"params\":{ \"ui_auto_standby\":1 } ,\"id\": 21}";

//4.5 connection status - pdf
var getNetworkStatus = "{\"jsonrpc\": \"2.0\",\"method\": \"get_network_status\", \"id\": 21}";
var getInternetStatus = "{\"jsonrpc\": \"2.0\",\"method\": \"get_internet_status\", \"id\": 21}";


//4.6 Record Filter API - pdf
var getRecFilteredList1 = "{\"jsonrpc\": \"2.0\",\"method\": \"get_rec_filtered_list\", \"params\": { \"filter\" : \"All Recordings\",\"sort\" : \"byduration\"},\"id\": 21}";
var getRecFilteredList2 = "{\"jsonrpc\": \"2.0\",\"method\": \"get_rec_filtered_list\", \"params\": { \"filter\" : \"Series Recordings\",\"sort\" : \"byname\"},\"id\": 21}";
var getRecFilteredList3 = "{\"jsonrpc\": \"2.0\",\"method\": \"get_rec_filtered_list\", \"params\": { \"filter\" : \"Episode Recordings\",\"sort\" : \"bydate\",\"order\" : \"DESC\",\"param\" : \"PS0000000298\"},\"id\": 21}";
var getRecFilteredList_Error = "{\"jsonrpc\": \"2.0\",\"method\": \"get_rec_filtered_list\", \"params\": { \"filter\" : \"All Record\",\"sort\" : \"byduration\"},\"id\": 21}";

//4.7 Secure Login APIs - pdf
var generateSecurelogin = "{\"jsonrpc\": \"2.0\",\"method\": \"generate_securelogin\",\"params\": {\"request_type\":\"1\"}, \"id\": 21}";
var getFirstVodCns = "{\"jsonrpc\": \"2.0\",\"method\": \"get_first_vod_cnx\", \"id\": 21}";
var setFirstVodCns = "{\"jsonrpc\": \"2.0\",\"method\": \"set_first_vod_cnx\", \"params\":{\"VOD_PORTAL_FIRST_CNX\":1}, \"id\": 21}";

//4.8. casware - excel, pdf

var casware_addServiceListenerOn = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.addServiceListenerOn\", \"params\":{\"ServiceMonitoring\":{\"IrdetoSoftcell\":{\"Handle\":\"0x0000\"}}}, \"id\":\"11\"}";
var casware_removeServiceListenerOn = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.removeServiceListenerOn\", \"params\":{\"ServiceMonitoring\":{\"IrdetoSoftcell\":{\"Handle\":\"0x0000\"}}}, \"id\":\"5\"}";
var casware_getProducts = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.getProducts\", \"id\":\"5\"}";
var casware_getServiceList = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.getServiceList\", \"id\":\"89\"}";
var casware_getHomingChannel = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.getHomingChannel\", \"id\":\"7\"}";

//for makeup event json
var casware_onSystemInfoChange = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSystemInfoChange\", \"id\":\"test\"}";
var casware_onSmartcardChange = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardChange\", \"id\":\"test\"}";
var casware_onSmartcardInsertion = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardInsertion\", \"id\":\"test\"}";
var casware_onSmatcardError = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardError\", \"id\":\"test\"}";
var casware_onSmartcardRemoval = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardRemoval\", \"id\":\"test\"}";
var casware_onServiceChange = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onServiceChange\", \"id\":\"test\"}";
var casware_onFingerPrinting = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onFingerPrinting\", \"id\":\"test\"}";
var casware_onMessage = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onMessage\", \"id\":\"test\"}";
var casware_onPinReseted = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onPinReseted\", \"id\":\"test\"}";
/*  //expected full result event json
var casware_onSystemInfoChange = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSystemInfoChange\", \"params\":{{\"SystemInfo\":{\"Casware\": \"prj_board 5.0.0\",\"IrdetoSoftcell\": {\"ClientName\": \"DVB Client (PVR DRM)\",\"Version\": \"3.18.4\",\"Build\": \"Sagem_HW05 Official\",\"Device\": \"0040, 0005, 0041\",\"Key\": \"FFFF, 0000, 0000\", \"CSSN\": \"14550062\",\"L-\": \"0001\",\"FirmwareVersion\": \"5.0.0\", \"LoaderVersion\": \"05.00.0000\",\"SerialNumber\": \"123456789\"}}}}}";
var casware_onSmartcardChange = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardChange\", \"params\":{\"Smartcard\":{\"State\": \"OK\",\"IrdetoSoftcell\": {\"CaCode\": \"D000-4\",\"SerialNumber\": \"1022 4502 89 (7)\",\"Type\":\"0006\", \"Version\":\"7.0\",\"Build\":\"02\",\"Variant\":\"00\",\"PatchLevel\": \"0000\", \"OwnerId\":\"0002\",\"Nationality\":\"ZAF\"}}}}";
var casware_onSmartcardInsertion = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardInsertion\", \"params\":{\"Smartcard\":{\"State\":\"INSERTED\",\"IrdetoSoftcell\":{\"CaCode\":\"I007-4\"}}}}";
var casware_onSmatcardError = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmatcardError\", \"params\":{\"Smartcard\":{\"State\":\"MUTE\",\"IrdetoSoftcell\":{\"CaCode\": \"E005-4\"}}}}";
var casware_onSmartcardRemoval = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onSmartcardRemoval\", \"params\":{\"Smartcard\":{\"State\":\"REMOVED\",\"IrdetoSoftcell\":{\"CaCode\": \"E004-4\"}}}}";
//[TODO]monitoring array.. json
var casware_onServiceChange = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onServiceChange\", \"params\":{\"ServicesStatus\":{\"IrdetoSoftcell\":[{\"Handle\":\"0x0000\",\"Name\":\"EMM\", \"GlobalStatus\":\"D100-5\",\"InternalStatus\":\"D100-5\",\"SmartCardId\":\"0x0000\", \"SmartCardStatus\":\"D000-4\",\"EMMSourceId\":\"0x0000\",\"EMMSourceStatus\":\"D100-6\", \"Monitoring\":[\"EMM: (030) 0500 - 03 00 00 - 00 40 00\"]}]}}}";
var casware_onFingerPrinting = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onFingerPrinting\", \"params\":{\"FingerPrint\":{\"text\":\"New message\",\"style\":{\"left\":\"20%\",\"top\":\"10%\", \"backgroundColor\":\"rgba(0,0,0,0.2)\",\"color\":\"rgba(255,255,255,0.8)\"}\"duration_ms\": 2000 }, \"IrdetoSoftcell\":{\"CoverageCode\": 5}}}";
var casware_onMessage = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onMessage\", \"params\":{\"Message\":{\"total\":1,\"unread\":0,\"pop-up\":{\"msgid\":32769, \"text\":\"Ekim ay\u0131na ait faturan\u0131z\u0131n...\",\"IrdetoSoftcell\":{\"type\":\"mail\", \"duration_ms\":20000}}}}}";
var casware_onPinReseted = "{\"jsonrpc\":\"2.0\",\"method\":\"casware.onPinReseted\", \"params\":{\"PinInfo\":{\"Pins\":[{\"ADULT_CODE\":\"reseted\"},{\"TECHNICAL_CODE\":\"default\"}]}}}";
*/



//4.8 casware but json has not casware - pdf
var DDEMMSVCRC = "{\"jsonrpc\":\"2.0\",\"method\": \"DDEMM_SVCRC\", \"params\":{\"data\":0},\"id\": 21}";
var DDEMMSVCID = "{\"jsonrpc\": \"2.0\",\"method\": \"DDEMM_SVCID\", \"params\":{\"data\":{\"onid\":126,\"tsid\":50900,\"sid\":5310}},\"id\": 21}";
var generateIppvSmsCode = "{\"jsonrpc\": \"2.0\",\"method\": \"generateIppvSmsCode\", \"params\":{\"eventDate\": 1454612006,\"channelNumber\": 201},\"id\": 21}";;
var generateVODSSmsCode = "{\"jsonrpc\": \"2.0\",\"method\": \"generateVodSmsCode\", \"params\":{\"assetId\": \"XPPK0001236612691390\",\"id\": 21}}";
var getLoadSequenceNumber = "{\"jsonrpc\": \"2.0\",\"method\": \"getLoadSequenceNumber\",\"id\": 21}";


//only excel
var other_getFirwareDate = "{\"jsonrpc\": \"2.0\",\"method\": \"get_firmware_date\",\"id\": 21}";
var other_getRerunList = "{\"jsonrpc\": \"2.0\",\"method\": \"get_rerun_list\",\"params\": {\"onid\": 126,	\"tsid\": 50400,\"sid\": 1708,\"eit_type\": 2,\"event_id\": 11887,\"src_onid\": 126,\"src_tsid\": 50800,\"src_sid\": 4314,\"start_time\": 0,\"end_time\": 0,\"max_count\": 1000},\"id\": 25}";
var other_formatHdd = "{\"jsonrpc\": \"2.0\",\"method\": \"format_hdd\",\"id\": 21}";
var other_getProgrammeInfo = "{\"jsonrpc\": \"2.0\", \"method\" : \"get_programme_info\", \"params\" :{\"onid\":126, \"tsid\":21100, \"sid\":295, \"eit_type\":0, \"event_id\":0},\"id\":2}";
var other_getFilteredChannelList = "{\"jsonrpc\": \"2.0\", \"method\" : \"get_iltered_channel_list\", \"params\" :{\"filters\":{\"definition\":\"HD\",\"manualBlocked\":true}},\"id\":2}";




var AgingArray=[
network_saveConfig_JSON,
network_getInterfaceList_JSON,
network_getInterface_JSON,
network_setInterface_JSON,
network_getAddressList_JSON,
network_addAddress_JSON,
network_delAddress_JSON,
network_setAddress_JSON,
network_getDhcpRequestedOptionList_JSON,
network_setDhcpRequestedOptionList_JSON,
network_unsetDhcpRequestedOptionList_JSON,
network_getDhcpSentOptionList_JSON,
network_setDhcpSentOption_JSON,
network_unsetDhcpSentOption_JSON,
network_getDhcpsConfig_JSON,
network_setDhcpsConfig_JSON,
network_setDhcpsPool_JSON,
network_getDhcpsLeaseList_JSON,
network_getDnsServerList_JSON,
network_addDnsServer_JSON,
network_delDnsServer_JSON,
network_setDnsServer_JSON,
network_getRoutesList_JSON,
network_addRoute_JSON,
network_delRoute_JSON,
network_setRoute_JSON,
network_getNtpServerList_JSON,
network_addNtpServer_JSON,
network_delNtpServer_JSON,
network_setNtpServer_JSON,
network_getWirelessStatus_JSON,
network_startWirelessScan_JSON,
network_stopWirelessScan_JSON,
network_getWirelessScanResult_JSON,
network_getWirelessType_JSON,
network_setWirelessType_JSON,
network_getWirelessProfilList_JSON,
network_addWirelessProfil_JSON,
network_delWirelessProfil_JSON,
network_setWirelessProfil_JSON,
network_launchWps_JSON,
network_cancelWps_JSON,
network_getWirelessApConfig_JSON,
network_setWirelessApConfig_JSON,
network_getWirelessApAssocList_JSON,
getEitIppvInfo,
getHddStatus,
getPinCodeMode,
setPinCodeMode,
getFrontPanelLocked,
setFrontPanelLocked,
getMoralityLevel,
setMoralityLevel,
getShowLockRec,
setShowLockRec,
getUiAutoStandby,
setUiAutoStandby,
getNetworkStatus,
getInternetStatus,
getRecFilteredList1,
getRecFilteredList2,
getRecFilteredList3,
getRecFilteredList_Error,
generateSecurelogin,
getFirstVodCns,
setFirstVodCns,
casware_addServiceListenerOn,
casware_removeServiceListenerOn,
casware_getProducts,
casware_getServiceList,
casware_getHomingChannel,
DDEMMSVCRC,
DDEMMSVCID,
DDEMMSVCRC, DDEMMSVCID, 
generateIppvSmsCode, 
generateVODSSmsCode, 
getLoadSequenceNumber,
other_getFirwareDate,
other_getRerunList,
other_formatHdd,
other_getProgrammeInfo,
other_getFilteredChannelList]; 
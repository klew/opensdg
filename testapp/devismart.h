#define PROTOCOL_DEVISMART_CONFIG "dominion-configuration-1.0"

int devismart_config_connect(osdg_connection_t conn);

int devismart_receive_data(osdg_connection_t conn, const void *data, unsigned int size);

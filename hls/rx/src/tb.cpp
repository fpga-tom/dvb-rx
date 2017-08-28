
extern int sync_correlate_tb();
extern int sync_find_peak_tb();
extern int sync_update_tb();
extern int ofdm_ifft_tb();
extern int ofdm_ifo_tb();


int main(int argc, char **argv) {
	sync_correlate_tb();
	sync_find_peak_tb();
	sync_update_tb();
	ofdm_ifft_tb();
	ofdm_ifo_tb();
	return 0;
}

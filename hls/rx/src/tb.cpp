
extern int sync_correlate_tb();
extern int sync_find_peak_tb();
extern int sync_align_tb();

int main(int argc, char **argv) {
	sync_correlate_tb();
	sync_find_peak_tb();
	sync_align_tb();
	return 0;
}

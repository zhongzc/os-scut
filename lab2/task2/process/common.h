const int MAX_LEN = 7;

const char *const BARBER = "./barber";
const char *const CONSUMER = "./consumer";
const int CONSUMER_CNT = 20;

const int PERM = 0640;

const char *const FTOK_PATH = ".";
const int FTOK_ID = 42;

const char *const SEM_CONSUMERS_MUTEX_NAME = "/consumers_mutex";
const int SEM_CONSUMERS_MUTEX_INIT_VALUE = 1;
const char *const SEM_BARBER_WAKEUP_NAME = "/barber_wakeup";
const int SEM_BARBER_WAKEUP_INIT_VALUE = 0;

const int FREE = 0;

struct consumers {
  int size;
  int done;
};

const char *C_COME_MSG  = "Consumer %d: coming a new consumer, there are %d now\n";
const char *C_GONE_MSG  = "Consumer %d: too many consumers, gone\n";
const char *C_WAKE_MSG  = "Consumer %d: a new consumer to wake up the barber\n";
const char *B_SERVE_MSG = "Barber   %d: serving 1 consumer, remaining %d\n";
const char *B_SLEEP_MSG = "Barber   %d: go to sleep...\n";
const char *B_WAKE_MSG  = "Barber   %d: wake up\n";
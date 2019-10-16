const int MAX_LEN = 7;

const char *const BARBER = "./barber";
const char *const CONSUMER = "./consumer";
const int CONSUMER_CNT = 20;

const int PERM = 0640;

const char *const SEM_CONSUMERS_MUTEX_NAME = "/consumers_mutex";
const int SEM_CONSUMERS_MUTEX_INIT_VALUE = 1;
const char *const SEM_BARBER_WAKEUP_NAME = "/barber_wakeup";
const int SEM_BARBER_WAKEUP_INIT_VALUE = 0;

const int FREE = 0;

struct consumers {
  int size;
  int done;
};